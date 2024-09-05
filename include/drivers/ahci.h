#ifndef __BLJOS__DRIVERS__AHCI_H
#define __BLJOS__DRIVERS__AHCI_H

#include <common/types.h>
#include <memorymanagement.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>

#define	SATA_SIG_ATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	0x96690101	// Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

#define	AHCI_BASE	0x400000	// 4M

#define HBA_PxCMD_ST    0x0001
#define HBA_PxCMD_FRE   0x0010
#define HBA_PxCMD_FR    0x4000
#define HBA_PxCMD_CR    0x8000
#define HBA_PxIS_TFES   (1 << 30)

#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define ATA_CMD_READ_DMA 0xC8
#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA 0xCA
#define ATA_CMD_WRITE_DMA_EX 0x35

namespace bljOS{
    namespace drivers{

        typedef enum{
            FIS_TYPE_REG_H2D = 0x27,
            FIS_TYPE_REG_D2H = 0x34,
            FIS_TYPE_DMA_ACT = 0x39,
            FIS_TYPE_DMA_SETUP = 0x41,
            FIS_TYPE_DATA = 0x46,
            FIS_TYPE_BIST = 0x58,
            FIS_TYPE_BIO_SETUP = 0x5F,
            FIS_TYPE_DEV_BITS = 0xA1,
        } FIS_TYPE;

        typedef struct tagFIS_REG_H2D{
            bljOS::common::uint8_t fis_type;

            bljOS::common::uint8_t pmport : 4;
            bljOS::common::uint8_t rsv0 : 3;
            bljOS::common::uint8_t c : 1;

            bljOS::common::uint8_t command;
            bljOS::common::uint8_t featurel;

            bljOS::common::uint8_t lba0;
            bljOS::common::uint8_t lba1;
            bljOS::common::uint8_t lba2;
            bljOS::common::uint8_t device;

            bljOS::common::uint8_t lba3;
            bljOS::common::uint8_t lba4;
            bljOS::common::uint8_t lba5;
            bljOS::common::uint8_t featureh;

            bljOS::common::uint8_t countl;
            bljOS::common::uint8_t counth;
            bljOS::common::uint8_t icc;
            bljOS::common::uint8_t control;

            bljOS::common::uint8_t rsv1[4];
        }FIS_REG_H2D;

        typedef struct tagFIS_REG_D2H{
            bljOS::common::uint8_t fis_type;

            bljOS::common::uint8_t pmport : 4;
            bljOS::common::uint8_t rsv0 : 2;
            bljOS::common::uint8_t i : 1;
            bljOS::common::uint8_t rsv1 : 1;

            bljOS::common::uint8_t status;
            bljOS::common::uint8_t error;

            bljOS::common::uint8_t lba0;
            bljOS::common::uint8_t lba1;
            bljOS::common::uint8_t lba2;
            bljOS::common::uint8_t device;

            bljOS::common::uint8_t lba3;
            bljOS::common::uint8_t lba4;
            bljOS::common::uint8_t lba5;
            bljOS::common::uint8_t rsv2;

            bljOS::common::uint8_t countl;
            bljOS::common::uint8_t counth;
            bljOS::common::uint8_t rsv3[2];

            bljOS::common::uint8_t rsv4[4];
        }FIS_REG_D2H;

        typedef struct tagFIS_DATA{
            bljOS::common::uint8_t fis_type;

            bljOS::common::uint8_t pmport : 4;
            bljOS::common::uint8_t rsv0 : 4;

            bljOS::common::uint8_t rsv1[2];

            bljOS::common::uint32_t data[1];
        }FIS_DATA;

        typedef struct tagFIS_PIO_SETUP{
            bljOS::common::uint8_t fis_type;

            bljOS::common::uint8_t pmport : 4;
            bljOS::common::uint8_t rsv0 : 1;
            bljOS::common::uint8_t d : 1;
            bljOS::common::uint8_t i : 1;
            bljOS::common::uint8_t rsv1 : 1;

            bljOS::common::uint8_t status;
            bljOS::common::uint8_t error;

            bljOS::common::uint8_t lba0;
            bljOS::common::uint8_t lba1;
            bljOS::common::uint8_t lba2;
            bljOS::common::uint8_t device;

            bljOS::common::uint8_t lba3;
            bljOS::common::uint8_t lba4;
            bljOS::common::uint8_t lba5;
            bljOS::common::uint8_t rsv2;

            bljOS::common::uint8_t countl;
            bljOS::common::uint8_t counth;
            bljOS::common::uint8_t rsv3;
            bljOS::common::uint8_t e_status;

            bljOS::common::uint8_t tc;
            bljOS::common::uint8_t rsv4[2];
        }FIS_PIO_SETUP;

        typedef struct tagFIS_DMA_SETUP{
            bljOS::common::uint8_t fis_type;

            bljOS::common::uint8_t pmport : 4;
            bljOS::common::uint8_t rsv0 : 1;
            bljOS::common::uint8_t d : 1;
            bljOS::common::uint8_t i : 1;
            bljOS::common::uint8_t a : 1;

            bljOS::common::uint8_t rsved[2];

            bljOS::common::uint64_t DMAbufferID;

            bljOS::common::uint32_t rsvd;

            bljOS::common::uint32_t DMAbufOffset;

            bljOS::common::uint32_t transferCount;

            bljOS::common::uint32_t resvd;
        }FIS_DMA_SETUP;

        typedef volatile struct tagHBA_PORT{
            bljOS::common::uint32_t clb;
            bljOS::common::uint32_t clbu;
            bljOS::common::uint32_t fb;
            bljOS::common::uint32_t fbu;
            bljOS::common::uint32_t is;
            bljOS::common::uint32_t ie;
            bljOS::common::uint32_t cmd;
            bljOS::common::uint32_t rsv0;
            bljOS::common::uint32_t tfd;
            bljOS::common::uint32_t sig;
            bljOS::common::uint32_t ssts;
            bljOS::common::uint32_t sctl;
            bljOS::common::uint32_t serr;
            bljOS::common::uint32_t sact;
            bljOS::common::uint32_t ci;
            bljOS::common::uint32_t sntf;
            bljOS::common::uint32_t fbs;
            bljOS::common::uint32_t rsv1[11];
            bljOS::common::uint32_t vendor[4];
        }HBA_PORT;

        typedef volatile struct tagHBA_MEM{
            bljOS::common::uint32_t cap;
            bljOS::common::uint32_t ghc;
            bljOS::common::uint32_t is;
            bljOS::common::uint32_t pi;
            bljOS::common::uint32_t vs;
            bljOS::common::uint32_t ccc_ctl;
            bljOS::common::uint32_t ccc_pts;
            bljOS::common::uint32_t em_loc;
            bljOS::common::uint32_t em_ctl;
            bljOS::common::uint32_t cap2;
            bljOS::common::uint32_t bohc;

            bljOS::common::uint8_t rsv[0xA0 - 0x2C];

            bljOS::common::uint8_t vendor[0x100 - 0xA0];

            HBA_PORT ports[1];
        }HBA_MEM;

        typedef volatile struct tagHBA_FIS{
            FIS_DMA_SETUP dsfis;
            bljOS::common::uint8_t pad0[4];

            FIS_PIO_SETUP psfis;
            bljOS::common::uint8_t pad1[12];

            FIS_REG_D2H rfis;
            bljOS::common::uint8_t pad2[4];

            FIS_DATA sdbfis;

            bljOS::common::uint8_t ufis[64];

            bljOS::common::uint8_t rsv[0x100-0xA0];
        }HBA_FIS;

        typedef struct tagHBA_CMD_HEADER{
            bljOS::common::uint8_t cfl : 5;
            bljOS::common::uint8_t a : 1;
            bljOS::common::uint8_t w : 1;
            bljOS::common::uint8_t p : 1;

            bljOS::common::uint8_t r : 1;
            bljOS::common::uint8_t b : 1;
            bljOS::common::uint8_t c : 1;
            bljOS::common::uint8_t rsv0 : 1;
            bljOS::common::uint8_t pmp : 4;

            bljOS::common::uint16_t prdtl;

            volatile bljOS::common::uint32_t prdbc;

            bljOS::common::uint32_t ctba;
            bljOS::common::uint32_t ctbau;

            bljOS::common::uint32_t rsv1[4];
        }HBA_CMD_HEADER;

        typedef struct tagHBA_PRDT_ENTRY{
            bljOS::common::uint32_t dba;
            bljOS::common::uint32_t dbau;
            bljOS::common::uint32_t rsv0;

            bljOS::common::uint32_t dbc : 22;
            bljOS::common::uint32_t rsv1 : 9;
            bljOS::common::uint32_t i : 1;
        }HBA_PRDT_ENTRY;

        typedef struct tagHBA_CMD_TBL{
            bljOS::common::uint8_t cfis[64];

            bljOS::common::uint8_t acmd[16];

            bljOS::common::uint8_t rsv[48];

            HBA_PRDT_ENTRY prdt_entry[1];
        }HBA_CMD_TBL;

        class AdvancedHostControllerInterface : public Driver{
        public:
            bljOS::common::uint32_t sata;
            bljOS::common::uint32_t cmdSlotsPreping;
            HBA_MEM* abar;
            AdvancedHostControllerInterface(bljOS::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor* dev);

            static int check_type(HBA_PORT* port);
            void probe_port(HBA_MEM* abar);
            void port_rebase(HBA_PORT* port, int portno);
            bool read(HBA_PORT* port, bljOS::common::uint32_t startl, bljOS::common::uint32_t starth, bljOS::common::uint32_t count, bljOS::common::uint8_t* buf);
            bool write(HBA_PORT* port, bljOS::common::uint32_t startl, bljOS::common::uint32_t starth, bljOS::common::uint32_t count, bljOS::common::uint8_t* buf);
        };
    }
}

#endif
