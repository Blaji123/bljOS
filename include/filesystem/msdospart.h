#ifndef __BLJOS__FILESYSTEM__MSDOSPART_H
#define __BLJOS__FILESYSTEM__MSDOSPART_H

#include <common/types.h>
#include <drivers/ata.h>

namespace bljOS{
    namespace filesystem{
        struct PartitionTableEntry{
            bljOS::common::uint8_t bootable;

            bljOS::common::uint8_t start_head;
            bljOS::common::uint8_t start_sector : 6;
            bljOS::common::uint16_t start_cylinder : 10;

            bljOS::common::uint8_t partition_id;

            bljOS::common::uint8_t end_head;
            bljOS::common::uint8_t end_sector : 6;
            bljOS::common::uint16_t end_cylinder : 10;

            bljOS::common::uint32_t start_lba;
            bljOS::common::uint32_t length;
        }__attribute__((packed));

        struct MasterBootRecord{
            bljOS::common::uint8_t bootloader[440];
            bljOS::common::uint32_t signature;
            bljOS::common::uint16_t unused;

            PartitionTableEntry primaryPartition[4];
            bljOS::common::uint16_t magicNumber;
        }__attribute__((packed));

        class MSDOSPartitionTable{
        public:
            static void ReadPartitions(bljOS::drivers::AdvancedTechnologyAttachment* hd);
        };
    }
}

#endif
