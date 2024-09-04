#include <drivers/ahci.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::hardwarecommunication;

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);
void printfHex(uint8_t key, int32_t x, int32_t y, uint32_t color);

int AdvancedHostControllerInterface::check_type(HBA_PORT* port){
    uint32_t ssts = port->ssts;

    uint8_t ipm = (ssts >> 8) & 0x0F;
    uint8_t det = ssts & 0x0F;

    if(det != HBA_PORT_DET_PRESENT)
        return AHCI_DEV_NULL;
    if(ipm != HBA_PORT_IPM_ACTIVE)
        return AHCI_DEV_NULL;

    switch(port->sig){
        case SATA_SIG_ATAPI:
            return AHCI_DEV_SATAPI;
        case SATA_SIG_SEMB:
            return AHCI_DEV_SEMB;
        case SATA_SIG_PM:
            return AHCI_DEV_PM;
        default:
            return AHCI_DEV_SATA;
    }
}

void AdvancedHostControllerInterface::probe_port(HBA_MEM* abar){
    uint32_t pi = abar->pi;
    int i = 0;
    while(i < 32){
        if(pi & 1){
            int dt = check_type(&abar->ports[i]);
            if(dt == AHCI_DEV_SATA){
                printf((uint8_t*)"SATA drive found at port: ", i + 10, i + 10, 0xebdbb2);
                printfHex((uint8_t)i, i + 10, i + 20, 0xebdbb2);
            }else if(dt == AHCI_DEV_SATAPI){
                printf((uint8_t*)"SATAPI drive found at port: ", i + 20, i + 10, 0xebdbb2);
                printfHex((uint8_t)i, i + 20, i + 20, 0xebdbb2);
            }else if(dt == AHCI_DEV_SEMB){
                printf((uint8_t*)"SAEMB drive found at port: ", i + 30, i + 10, 0xebdbb2);
                printfHex((uint8_t)i, i + 30, i + 20, 0xebdbb2);
            }else if(dt == AHCI_DEV_PM){
                printf((uint8_t*)"PM drive found at port: ", i + 40, i + 10, 0xebdbb2);
                printfHex((uint8_t)i, i + 40, i + 20, 0xebdbb2);
            }else{
                printf((uint8_t*)"No drive found at port: ", i + 50, i + 10, 0xebdbb2);
                printfHex((uint8_t)i, i + 50, i + 20, 0xebdbb2);
            }
        }
        pi >>= 1;
        i++;
    }
}

void start_cmd(HBA_PORT* port){
    while(port->cmd & HBA_PxCMD_CR);

    port->cmd |= HBA_PxCMD_FRE;
    port->cmd |= HBA_PxCMD_ST;
}

void stop_cmd(HBA_PORT* port){
    port->cmd &= ~HBA_PxCMD_ST;

    port->cmd &= ~HBA_PxCMD_FRE;

    while(1){
        if(port->cmd & HBA_PxCMD_FR)
            continue;
        if(port->cmd & HBA_PxCMD_CR)
            continue;
        break;
    }
}

int find_cmdslot(HBA_PORT* port){
    uint32_t slots = (port->sact | port->ci);
    for(int i=0;i<32;i++){
        if((slots&1) == 0)
            return i;
        slots >>= 1;
    }
    return -1;
}

void AdvancedHostControllerInterface::port_rebase(HBA_PORT* port, int portno){
    stop_cmd(port);

    port->clb = AHCI_BASE + (portno << 10);
    port->clbu = 0;
    memset((void*)(port->clb), 0, 1024);

    port->fb = AHCI_BASE + (32 << 10) + (portno << 8);
    port->fbu = 0;
    memset((void*)(port->fb), 0, 256);

    HBA_CMD_HEADER* cmdHeader = (HBA_CMD_HEADER*)(port->clb);
    for(int i=0;i<32;i++){
        cmdHeader[i].prdtl = 8;
        cmdHeader[i].ctba = AHCI_BASE + (40 << 10) + (portno << 13) + (i << 8);
        cmdHeader[i].ctbau = 0;
        memset((void*)cmdHeader[i].ctba, 0, 256);
    }

    start_cmd(port);
}

bool AdvancedHostControllerInterface::read(HBA_PORT* port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t* buf){
    port->is = (uint32_t) -1;
    int spin = 0;
    int slot = find_cmdslot(port);
    if(slot == -1)
        return false;

    HBA_CMD_HEADER* cmdHeader = (HBA_CMD_HEADER*)port->clb;
    cmdHeader += slot;
    cmdHeader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);
    cmdHeader->w = 0;
    cmdHeader->prdtl = (uint16_t)((count - 1)>>4) + 1;

    HBA_CMD_TBL* cmdTbl = (HBA_CMD_TBL*)(cmdHeader->ctba);
    memset(cmdTbl, 0, sizeof(HBA_CMD_TBL) + (cmdHeader->prdtl - 1)*sizeof(HBA_PRDT_ENTRY));

    int i=0;
    for(i=0;i<cmdHeader->prdtl-1;i++){
        cmdTbl->prdt_entry[i].dba = (uint32_t)buf;
        cmdTbl->prdt_entry[i].dbc = 8*1024-1;
        cmdTbl->prdt_entry[i].i = 1;
        buf += 4*1024;
        count -= 16;
    }

    cmdTbl->prdt_entry[i].dba = (uint32_t)buf;
    cmdTbl->prdt_entry[i].dbc = (count << 9) - 1;
    cmdTbl->prdt_entry[i].i = 1;

    FIS_REG_H2D* cmdFis = (FIS_REG_H2D*)(&cmdTbl->cfis);

    cmdFis->fis_type = FIS_TYPE_REG_H2D;
    cmdFis->c = 1;
    cmdFis->command = ATA_CMD_READ_DMA_EX;

    cmdFis->lba0 = (uint8_t)startl;
    cmdFis->lba1 = (uint8_t)(startl >> 8);
    cmdFis->lba2 = (uint8_t)(startl >> 16);
    cmdFis->device = 1<<6;

    cmdFis->lba3 = (uint8_t)(startl >> 24);
    cmdFis->lba4 = (uint8_t)starth;
    cmdFis->lba5 = (uint8_t)(starth >> 8);

    cmdFis->countl = count & 0xFF;
    cmdFis->counth = (count >> 8) & 0xFF;

    while((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
        spin++;

    if(spin == 1000000){
        printf((uint8_t*)"Port is hung", 60, 60, 0xebdbb2);
        return false;
    }

    port->ci = 1<<slot;

    while(1){
        if((port->ci & (1<<slot)) == 0)
            break;
        if(port->is & HBA_PxIS_TFES){
            printf((uint8_t*)"Read disk error", 60, 60, 0xebdbb2);
            return false;
        }
    }

    if(port->is & HBA_PxIS_TFES){
        printf((uint8_t*)"Read disk error", 60, 60, 0xebdbb2);
        return false;
    }

    return true;
}

bool AdvancedHostControllerInterface::write(HBA_PORT* port, uint32_t startl, uint32_t starth, uint32_t count, uint16_t* buf){
    port->is = (uint32_t) -1;
    int spin = 0;
    int slot = find_cmdslot(port);
    if(slot == -1)
        return false;

    HBA_CMD_HEADER* cmdHeader = (HBA_CMD_HEADER*)port->clb;
    cmdHeader += slot;
    cmdHeader->cfl = sizeof(FIS_REG_H2D)/sizeof(uint32_t);
    cmdHeader->w = 1;
    cmdHeader->prdtl = (uint16_t)((count - 1)>>4) + 1;

    HBA_CMD_TBL* cmdTbl = (HBA_CMD_TBL*)(cmdHeader->ctba);
    memset(cmdTbl, 0, sizeof(HBA_CMD_TBL) + (cmdHeader->prdtl - 1)*sizeof(HBA_PRDT_ENTRY));

    int i=0;
    for(i=0;i<cmdHeader->prdtl-1;i++){
        cmdTbl->prdt_entry[i].dba = (uint32_t)buf;
        cmdTbl->prdt_entry[i].dbc = 8*1024-1;
        cmdTbl->prdt_entry[i].i = 1;
        buf += 4*1024;
        count -= 16;
    }

    cmdTbl->prdt_entry[i].dba = (uint32_t)buf;
    cmdTbl->prdt_entry[i].dbc = (count << 9) - 1;
    cmdTbl->prdt_entry[i].i = 1;

    FIS_REG_H2D* cmdFis = (FIS_REG_H2D*)(&cmdTbl->cfis);

    cmdFis->fis_type = FIS_TYPE_REG_H2D;
    cmdFis->c = 1;
    cmdFis->command = ATA_CMD_WRITE_DMA_EX;

    cmdFis->lba0 = (uint8_t)startl;
    cmdFis->lba1 = (uint8_t)(startl >> 8);
    cmdFis->lba2 = (uint8_t)(startl >> 16);
    cmdFis->device = 1<<6;

    cmdFis->lba3 = (uint8_t)(startl >> 24);
    cmdFis->lba4 = (uint8_t)starth;
    cmdFis->lba5 = (uint8_t)(starth >> 8);

    cmdFis->countl = count & 0xFF;
    cmdFis->counth = (count >> 8) & 0xFF;

    while((port->tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
        spin++;

    if(spin == 1000000){
        printf((uint8_t*)"Port is hung", 60, 60, 0xebdbb2);
        return false;
    }

    port->ci = 1<<slot;

    while(1){
        if((port->ci & (1<<slot)) == 0)
            break;
        if(port->is & HBA_PxIS_TFES){
            printf((uint8_t*)"Read disk error", 60, 60, 0xebdbb2);
            return false;
        }
    }

    if(port->is & HBA_PxIS_TFES){
        printf((uint8_t*)"Read disk error", 60, 60, 0xebdbb2);
        return false;
    }

    return true;
}

AdvancedHostControllerInterface::AdvancedHostControllerInterface(PeripheralComponentInterconnectDeviceDescriptor* dev):Driver(){
    dev->command |= 0x04;

    uint32_t abarAddress = dev->BAR5 & ~0xF;
    this->abar = (HBA_MEM*)abarAddress;

    probe_port(this->abar);
}



