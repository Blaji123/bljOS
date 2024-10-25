// #include <filesystem/fat32.h>
//
// void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);
//
// using namespace bljOS;
// using namespace bljOS::common;
// using namespace bljOS::filesystem;
//
// FileAllocationTable::FileAllocationTable(MBR_PARTITION_TABLE* mbr){
//     this->mbr = mbr;
// }
//
// bool FileAllocationTable::fat32Mount(MountPoint* mount){
//     mount->handlers = &fat32Handlers;
//     mount->stat = fat32Stat;
//     mount->lstat = fat32Stat;
//
//     mount->fsInfo = MemoryManager::activeMemoryManager->malloc(sizeof(FAT32));
//     memset(mount->fsInfo, 0, sizeof(FAT32));
//     FAT32* fat = FAT_PTR(mount->fsInfo);
//
//     fat->offsetBase = mount->mbr.lba_first_sector;
//
//     uint8_t firstSec[SECTOR_SIZE] = {0};
//     mbr->getDiskBytes(firstSec, fat->offsetBase, 1);
//
//     memcpy(&fat->bootsec, firstSec, sizeof(FAT32BootSector));
//
//     if(fat->bootsec.bytes_per_sector != SECTOR_SIZE)
//         printf((uint8_t*)"error", 20, 20, 0xebdbb2);
//
//     fat->offsetFats = fat->offsetBase + fat->bootsec.reserved_sector_count;
//     fat->offsetClusters = fat->offsetFats + fat->bootsec.table_count * fat->bootsec.extended_section.table_size_32;
//
//     for(int i=0;i<FAT32_CACHE_MAX;i++){
//         fat->cacheBase[i] = FAT32_CACHE_BAD;
//         fat->cache[i] = MemoryManager::activeMemoryManager->malloc(LBA_TO_OFFSET(fat->bootsec.sectors_per_cluster));
//         memset(fat->cache[i], 0, LBA_TO_OFFSET(fat->bootsec.sectors_per_cluster));
//     }
//
//     return true;
// }
//
// int FileAllocationTable::fat32Open(char* filename, int flags, int mode, OpenFile* fd, char** symlinkResolve){
//     if(flags & O_CREAT || flags & O_WRONLY || flags & O_RDWR)
//         return -EROFS;
//
//     FAT32* fat = FAT_PTR(fd->MountPoint->fsInfo);
//
//     FAT32TraverseResult res = fat32TraversePath(fat, filename, fat->bootsec.extended_section.root_cluster);
//     if(!res.directory)
//         return -ENOENT;
//
//     fd->dir = MemoryManager::activeMemoryManager->malloc(sizeof(FAT32OpenFd));
//     FAT32OpenFd* dir = FAT_DIR_PTR(fd->dir);
//     memset(dir, 0, sizeof(FAT32OpenFd));
//
//     dir->ptr = 0;
//     dir->index = res.index;
//     dir->directoryStarting = res.directory;
//     dir->directoryCurr = FAT_COMB_HIGH_LOW(res.dirEntry.clusterHigh, res.dirEntry.clusterLow);
//     memcpy(&dir->dirEnt, &res->dirEntry, sizeof(FAT32DirectoryEntry));
//
//     if(res.dirEntry.attrib & FAT_ATTRIB_DIRECTORY){
//         size_t len = String::strlength(filename) + 1;
//         fd->dirname = (char*)MemoryManager::activeMemoryManager->malloc(len);
//         memcpy(fd->dirname, filename, len);
//     }
//
//     return 0;
// }
//
// int FileAllocationTable::fat32Read(OpenFile* fd, uint8_t* buff, size_t limit){
//
// }
//
// size_t FileAllocationTable::fat32Seek(OpenFile* fd, size_t target, long int offset, int whence){
//
// }
//
// size_t FileAllocationTable::fat32GetFilesize(OpenFile* fd){
//
// }
//
// bool FileAllocationTable::fat32Close(OpenFile* fd){
//
// }
//
// size_t FileAllocationTable::fat32ClusterToLBA(FAT32* fat, uint32_t cluster){
//
// }
//
// int FileAllocationTable::fat32IsShortFilenamePossible(char* filename, size_t len){
//
// }
//
// unsigned long FileAllocationTable::fat32UnixTime(unsigned short fat_date, unsigned short fat_time){
//
// }
//
// uint32_t FileAllocationTable::fat32FATtraverse(FAT32* fat, uint32_t offset){
//
// }
//
// uint8_t* FileAllocationTable::fat32FATchain(FAT32* fat, uint32_t offsetStart, uint32_t amount){
//
// }
//
// FAT32TraverseResult FileAllocationTable::fat32Traverse(FAT32* fat, uint32_t initDirectory, char* search, size_t searchLength){
//
// }
//
// FAT32TraverseResult FileAllocationTable::fat32TraversePath(FAT32* fat, char* path, uint32_t directoryStarting){
//
// }
//
// bool FileAllocationTable::fat32Stat(MountPoint* mnt, char* filename, struct stat* target, char** symlinkResolve){
//
// }
//
// int FileAllocationTable::fat32StatFd(OpenFile* fd, struct stat* target){
//
// }
//
// int FileAllocationTable::fat32Getdents64(OpenFile* file, struct linux_dirent64* start, unsigned int hardlimit){
//
// }
//
// int FileAllocationTable::fat32FNtoNormal(uint8_t* target, FAT32DirectoryEntry* dirent){
//
// }
//
