#ifndef __BLJOS__FILESYSTEM__FAT32_H
#define __BLJOS__FILESYSTEM__FAT32_H

#include <common/types.h>
#include <drivers/ata.h>

namespace bljOS{
    namespace filesystem{
        struct BiosParameterBlock32{
            bljOS::common::uint8_t jump[3];
            bljOS::common::uint8_t softName[8];
            bljOS::common::uint16_t bytesPerSector;
            bljOS::common::uint8_t sectorsPerCluster;
            bljOS::common::uint16_t reservedSectors;
            bljOS::common::uint8_t fatCopies;
            bljOS::common::uint16_t rootDirEntries;
            bljOS::common::uint16_t totalSectors;
            bljOS::common::uint8_t mediaType;
            bljOS::common::uint16_t fatSectorCount;
            bljOS::common::uint16_t sectorsPerTrack;
            bljOS::common::uint16_t headCount;
            bljOS::common::uint32_t hiddenSectors;
            bljOS::common::uint32_t totalSectorCount;

            bljOS::common::uint32_t tableSize;
            bljOS::common::uint16_t extFlags;
            bljOS::common::uint16_t fatVersion;
            bljOS::common::uint32_t rootCluster;
            bljOS::common::uint16_t fatInfo;
            bljOS::common::uint16_t backupSector;
            bljOS::common::uint8_t reserved0[12];
            bljOS::common::uint8_t driveNumber;
            bljOS::common::uint8_t reserved;
            bljOS::common::uint8_t bootSignature;
            bljOS::common::uint32_t volumeId;
            bljOS::common::uint8_t volumeLabel[11];
            bljOS::common::uint8_t fatTypeLabel[8];
        }__attribute__((packed));

        struct DirectoryEntryFAT32{
            bljOS::common::uint8_t name[8];
            bljOS::common::uint8_t ext[3];
            bljOS::common::uint8_t attributes;
            bljOS::common::uint8_t reserved;
            bljOS::common::uint8_t cTimeTenth;
            bljOS::common::uint16_t cTIme;
            bljOS::common::uint16_t cDate;
            bljOS::common::uint16_t aTime;

            bljOS::common::uint16_t firstClusterHi;
            bljOS::common::uint16_t wTime;
            bljOS::common::uint16_t wDate;
            bljOS::common::uint16_t firstClusterLow;
            bljOS::common::uint32_t size;
        }__attribute__((packed));

        void ReadBiosBlock(bljOS::drivers::AdvancedTechnologyAttachment* hd, bljOS::common::uint32_t partitionOffset);
    }
}

#endif
