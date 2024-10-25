#ifndef __BLJOS__FILESYSTEM__FAT32_H
#define __BLJOS__FILESYSTEM__FAT32_H

#include <common/types.h>
#include <common/linux.h>
#include <filesystem/vfs.h>
#include <memorymanagement.h>

#define LFN_MAX_CHARS_PER 13
#define LFN_MAX 20
#define LFN_MAX_TOTAL_CHARS 256
#define LFN_ORDER_FINAL 0x40

#define FAT_ATTRIB_READ_ONLY 0x01
#define FAT_ATTRIB_HIDDEN 0x02
#define FAT_ATTRIB_SYSTEM 0x04
#define FAT_ATTRIB_VOLUME_ID 0x08
#define FAT_ATTRIB_DIRECTORY 0x10
#define FAT_ATTRIB_ARCHIVE 0x20
#define FAT_ATTRIB_LFN                                                         \
  (FAT_ATTRIB_READ_ONLY | FAT_ATTRIB_HIDDEN | FAT_ATTRIB_SYSTEM |              \
   FAT_ATTRIB_VOLUME_ID)

#define FAT_PTR(a) ((FAT32 *)(a))
#define FAT_DIR_PTR(a) ((FAT32OpenFd *)(a))
#define FAT_COMB_HIGH_LOW(clusterhigh, clusterlow)                             \
  (((uint32_t)clusterhigh << 16) | clusterlow)

#define FAT_INODE_GEN(directory, index) ((directory) * 100 + (index))

#define FAT32_CACHE_BAD 0xFFFFFFFF
#define FAT32_CACHE_MAX 32

namespace bljOS{
    namespace filesystem{

        typedef struct FAT32DirectoryEntry{
            char name[8];
            char ext[3];
            bljOS::common::uint8_t attrib;
            bljOS::common::uint8_t userattrib;

            char undelete;
            bljOS::common::uint16_t createtime;
            bljOS::common::uint16_t createdate;
            bljOS::common::uint16_t accessdate;
            bljOS::common::uint16_t clusterhigh;

            bljOS::common::uint16_t modifiedtime;
            bljOS::common::uint16_t modifieddate;
            bljOS::common::uint16_t clusterlow;
            bljOS::common::uint32_t filesize;
        }__attribute__((packed)) FAT32DirectoryEntry;


        typedef struct FAT32ExtendedSector{
            bljOS::common::uint32_t table_size_32;
            bljOS::common::uint16_t extended_flags;
            bljOS::common::uint16_t fat_version;
            bljOS::common::uint32_t root_cluster;
            bljOS::common::uint16_t fat_info;
            bljOS::common::uint16_t backup_BS_sector;
            bljOS::common::uint8_t reserved_0[12];
            bljOS::common::uint8_t drive_number;
            bljOS::common::uint8_t reserved_1;
            bljOS::common::uint8_t boot_signature;
            bljOS::common::uint32_t volume_id;
            bljOS::common::uint8_t volume_label[11];
            bljOS::common::uint8_t fat_type_label[8];
        }__attribute__((packed)) FAT32ExtendedSector;

        typedef struct FAT32BootSector{
            bljOS::common::uint8_t bootjmp[3];
            bljOS::common::uint8_t oem_name[8];
            bljOS::common::uint16_t bytes_per_sector;
            bljOS::common::uint8_t sectors_per_cluster;
            bljOS::common::uint16_t reserved_sector_count;
            bljOS::common::uint8_t table_count;
            bljOS::common::uint16_t root_entry_count;
            bljOS::common::uint16_t total_sectors_16;
            bljOS::common::uint8_t media_type;
            bljOS::common::uint16_t table_size_16;
            bljOS::common::uint16_t sectors_per_track;
            bljOS::common::uint16_t head_side_count;
            bljOS::common::uint32_t hidden_sector_count;
            bljOS::common::uint32_t total_sectors_32;

            FAT32ExtendedSector extended_section;

        }__attribute__((packed)) FAT32BootSector;

        typedef struct FAT32LFN{
            bljOS::common::uint8_t order;
            bljOS::common::uint8_t first_five[10];
            bljOS::common::uint8_t attributes;
            bljOS::common::uint8_t type;
            bljOS::common::uint8_t checksum;
            bljOS::common::uint8_t next_six[12];
            bljOS::common::uint16_t zero;
            bljOS::common::uint8_t last_two[4];
        }__attribute__((packed)) FAT32LFN;

        typedef struct FAT32{
            bljOS::common::size_t offsetBase;
            bljOS::common::size_t offsetFats;
            bljOS::common::uint8_t offsetClusters;

            FAT32BootSector bootsec;

            bljOS::common::uint8_t* cache[FAT32_CACHE_MAX];
            bljOS::common::uint32_t cacheBase[FAT32_CACHE_MAX];
            int cacheCurr;
        }FAT32;

        typedef struct FAT32OpenFd{
            bljOS::common::uint32_t ptr;

            bljOS::common::uint8_t index;
            bljOS::common::uint32_t directoryStarting;
            bljOS::common::uint32_t directoryCurr;

            FAT32DirectoryEntry dirEnt;
        }FAT32OpenFd;


        typedef struct FAT32TraverseResult{
            bljOS::common::uint32_t directory;
            bljOS::common::uint8_t index;
            FAT32DirectoryEntry dirEntry;
        }FAT32TraverseResult;

        class FileAllocationTable{
            VfsHandlers fat32Handlers;
            MBR_PARTITION_TABLE* mbr;
        public:
            FileAllocationTable(MBR_PARTITION_TABLE* mbr);

            bool fat32Mount(MountPoint* mount);
            int fat32Open(char* filename, int flags, int mode, OpenFile* fd, char** symlinkResolve);
            int fat32Read(OpenFile* fd, bljOS::common::uint8_t* buff, bljOS::common::size_t limit);
            bljOS::common::size_t fat32Seek(OpenFile* fd, bljOS::common::size_t target, long int offset, int whence);
            bljOS::common::size_t fat32GetFilesize(OpenFile* fd);
            bool fat32Close(OpenFile* fd);
            bljOS::common::size_t fat32ClusterToLBA(FAT32* fat, bljOS::common::uint32_t cluster);
            int fat32IsShortFilenamePossible(char* filename, bljOS::common::size_t len);
            void fat32LFNmemcpy(bljOS::common::uint8_t* lfnName, FAT32LFN* lfn, int index);
            unsigned long fat32UnixTime(unsigned short fat_date, unsigned short fat_time);
            bljOS::common::uint32_t fat32FATtraverse(FAT32* fat, bljOS::common::uint32_t offset);
            bljOS::common::uint8_t* fat32FATchain(FAT32* fat, bljOS::common::uint32_t offsetStart, bljOS::common::uint32_t amount);
            FAT32TraverseResult fat32Traverse(FAT32* fat, bljOS::common::uint32_t initDirectory, char* search, bljOS::common::size_t searchLength);
            FAT32TraverseResult fat32TraversePath(FAT32* fat, char* path, bljOS::common::uint32_t directoryStarting);
            bool fat32Stat(MountPoint* mnt, char* filename, struct stat* target, char** symlinkResolve);
            int fat32StatFd(OpenFile* fd, struct stat* target);
            int fat32Getdents64(OpenFile* file, struct linux_dirent64* start, unsigned int hardlimit);
            int fat32FNtoNormal(bljOS::common::uint8_t* target, FAT32DirectoryEntry* dirent);
        }



    }
}

#endif
