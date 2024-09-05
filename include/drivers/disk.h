#ifndef __BLJOS__DRIVERS__DISK_H
#define __BLJOS__DRIVERS__DISK_H

#define SECTOR_SIZE 512
#define LBA_TO_OFFSET(a) ((a) * SECTOR_SIZE)

#define MBR_PARTITION_1 0x01BE
#define MBR_PARTITION_2 0x01CE
#define MBR_PARTITION_3 0x01DE
#define MBR_PARTITION_4 0x01EE

#define MBR_BOOTABLE 0x80
#define MBR_REGULAR 0x00


#include <memorymanagement.h>
#include <drivers/ahci.h>
#include <common/types.h>
#include <hardwarecommunication/pci.h>

namespace bljOS{
    namespace drivers{
        struct MBR_PARTITION{
            bljOS::common::uint8_t status;
            bljOS::common::uint8_t chs_first_sector[3];
            bljOS::common::uint8_t type;
            bljOS::common::uint8_t chs_last_sector[3];
            bljOS::common::uint32_t lba_first_sector;
            bljOS::common::uint32_t sector_count;
        }__attribute__((packed));

        class MBR_PARTITION_TABLE{
            AdvancedHostControllerInterface* ahci;
            void diskBytes(bljOS::common::uint8_t *target_address, bljOS::common::uint32_t LBA, bljOS::common::uint32_t sector_count, bool write);
        public:
            MBR_PARTITION_TABLE(AdvancedHostControllerInterface* ahci);

            bool openDisk(bljOS::common::uint32_t disk, bljOS::common::uint8_t partition, MBR_PARTITION* out);
            bool validateMbr(bljOS::common::uint8_t* mbrSector);
            void getDiskBytes(bljOS::common::uint8_t* target_address, bljOS::common::uint32_t LBA, bljOS::common::size_t sector_count);
            void setDiskBytes(const bljOS::common::uint8_t* target_address, bljOS::common::uint32_t LBA, bljOS::common::uint8_t sector_count);
        };
    }
}

#endif
