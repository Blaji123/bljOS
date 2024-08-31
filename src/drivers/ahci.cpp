#include <drivers/ahci.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;

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


