#include <filesystem/vfs.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::filesystem;

VirtualFileSystemController::VirtualFileSystemController(){

}

OpenFile* VirtualFileSystemController::fsKernelOpen(char* filename, int flags, uint32_t mode){

}

bool VirtualFileSystemController::fsKernelClose(OpenFile* file){

}

int VirtualFileSystemController::fsUserOpen(void* task, char* path, char* buf, int size){

}

int VirtualFileSystemController::fsUserClose(void* task, int fd){

}

int VirtualFileSystemController::fsUserSeek(void* task, uint32_t fs, int offset, int whence){

}

OpenFile* VirtualFileSystemController::fsUserGetNode(void* task, int fd){

}

OpenFile* VirtualFileSystemController::fsUserDuplicateNode(void* taskPtr, OpenFile* original){

}

OpenFile* VirtualFileSystemController::fsUserDuplicateNodeUnsafe(OpenFile* original){
    OpenFile* orphan = (OpenFile*)MemoryManagement::activeMemoryManager->malloc(sizeof(OpenFile));
    orphan->next = 0;
    memcpy((void*)((size_t)orphan + sizeof(orphan->next)), (void*)((size_t)original + sizeof(original->next)), sizeof(OpenFile) - sizeof(orphan->next));

    if(original->handlers->duplicate && !original->handlers->duplicate(original, orphan)){
        MemoryManagement::activeMemoryManager->free(orphan);
        return 0;
    }

    return orphan;
}

uint32_t VirtualFileSystemController::fsRead(OpenFile* file, uint8_t* out, uint32_t limit){

}

uint32_t VirtualFileSystemController::fsWrite(OpenFile* file, uint8_t* in, uint32_t limit){

}

void VirtualFileSystemController::fsReadFullFile(OpenFile* file, uint8_t* out){

}

int VirtualFileSystemController::fsReadLink(void* task, char* path, char* buf, int size){

}

int VirtualFileSystemController::fsMkdir(void *task, char *path, uint32_t mode){

}

size_t VirtualFileSystemController::fsGetFilesize(OpenFile* file){

}

char* VirtualFileSystemController::fsStripMountPoint(const char* filename, MountPoint* mnt){

}

char* VirtualFileSystemController::fsSanitize(char* prefix, char* filename){

}

bool VirtualFileSystemController::fsStat(OpenFile* fd, stat* target){

}

bool VirtualFileSystemController::fsStatByFilename(void* task, char* filename, stat* target){


}

bool VirtualFileSystemController::fsLStatByFilename(void* task, char* filename, stat* target){

}

MountPoint* VirtualFileSystemController::fsMount(char* prefix, CONNECTOR connector, uint32_t disk, uint8_t partition){

}

bool VirtualFileSystemController::fsUnmount(MountPoint* mnt){

}

MountPoint* VirtualFileSystemController::fsDetermineMountPoint(char* filename){

}

char* VirtualFileSystemController::fsResolveSymlink(MountPoint* mnt, char* symlink){

}



