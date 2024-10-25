#include <filesystem/vfs.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::filesystem;
using namespace bljOS::datastructures;

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);

OpenFile* fsRegisterNode(Task* task){
    spinlockCntWriteAcquire(&task->WLOCK_FILES);
    OpenFile* ret = (OpenFile*)LinkedList::LinkedListAllocate((void**)&task->firstFile, sizeof(OpenFile));
    spinlockCntWriteRelease(&task->WLOCK_FILES);
    return ret;
}

bool fsUnregisterNode(Task* task, OpenFile* file){
    spinlockCntWriteAcquire(&task->WLOCK_FILES);
    bool ret = LinkedList::LinkedListUnregister((void**)&task->firstFile, file);
    spinlockCntWriteRelease(&task->WLOCK_FILES);
    return ret;
}

bool fsCloseGeneric(OpenFile* file, Task* task){
    fsUnregisterNode(task, file);

    bool res = file->handlers->close ? file->handlers->close(file) : true;
    MemoryManager::activeMemoryManager->free(file);
    return res;
}

char* prefix = "/";
int openId = 3;
OpenFile* fsOpenGeneric(VirtualFileSystemController* vfs, char* filename, Task* task, int flags, int mode){
    char* safeFilename = vfs->fsSanitize(task ? task->cwd : prefix, filename);

    OpenFile* target = fsRegisterNode(task);
    target->id = openId++;
    target->mode = mode;
    target->flags = flags;

    target->pointer = 0;
    target->tmp1 = 0;

    MountPoint* mnt = vfs->fsDetermineMountPoint(safeFilename);
    if(!mnt){
        fsUnregisterNode(task, target);
        MemoryManager::activeMemoryManager->free(target);
        MemoryManager::activeMemoryManager->free(safeFilename);
        return 0;
    }
    target->mountPoint = mnt;
    target->handlers = mnt->handlers;

    if(flags & O_CLOEXEC)
        target->closeOnExec = true;

    char* strippedFilename = vfs->fsStripMountPoint(safeFilename, mnt);
    if(target->handlers->open){
        char* symlink = 0;
        int ret = target->handlers->open(strippedFilename, flags, mode, target, &symlink);
        if(ret < 0){
            fsUnregisterNode(task, target);
            MemoryManager::activeMemoryManager->free(target);
            MemoryManager::activeMemoryManager->free(safeFilename);

            if(symlink && ret != -ELOOP){
                char* symlinkResolved = vfs->fsResolveSymlink(mnt, symlink);
                MemoryManager::activeMemoryManager->free(symlink);
                OpenFile* res = fsOpenGeneric(vfs, symlinkResolved, task, flags, mode);
                MemoryManager::activeMemoryManager->free(symlinkResolved);
                return res;
            }

            return (OpenFile*)((size_t)(-ret));
        }
        MemoryManager::activeMemoryManager->free(safeFilename);
    }
    return target;
}

VirtualFileSystemController::VirtualFileSystemController(TaskManager* taskManager, MBR_PARTITION_TABLE* mbr){
    this->taskManager = taskManager;
    this->mbr = mbr;
}

OpenFile* VirtualFileSystemController::fsKernelOpen(char* filename, int flags, uint32_t mode){
    Task* target = taskManager->taskGet(KERNEL_TASK_ID);
    OpenFile* ret = fsOpenGeneric(this, filename, target, flags, mode);
    if((size_t)(ret) < 1024)
        return 0;
    return ret;
}

bool VirtualFileSystemController::fsKernelClose(OpenFile* file){
    Task* target = taskManager->taskGet(KERNEL_TASK_ID);
    return fsCloseGeneric(file, target);
}

int VirtualFileSystemController::fsUserOpen(void* task, char* filename, int flags, int mode){
    if(flags & FASYNC){
        printf((uint8_t*)"error", 20, 20, 0xebdbb2);
        return -ENOSYS;
    }

    OpenFile* file = fsOpenGeneric(this, filename, (Task*)task, flags, mode);
    if((size_t)(file) < 1024)
        return -((size_t)file);

    return file->id;
}

int VirtualFileSystemController::fsUserClose(void* task, int fd){
    OpenFile* file = fsUserGetNode(task, fd);
    if(!file)
        return -EBADF;
    bool res = fsCloseGeneric(file, (Task*)task);
    if(res)
        return 0;
    else
        return -1;
}

int VirtualFileSystemController::fsUserSeek(void* task, uint32_t fd, int offset, int whence){
    OpenFile* file = fsUserGetNode(task, fd);
    if(!file)
        return -1;
    int target = offset;
    if(whence == SEEK_SET)
        target += 0;
    else if(whence == SEEK_CURR)
        target += file->pointer;
    else if(whence == SEEK_END)
        target += fsGetFilesize(file);

    if(!file->handlers->seek)
        return -ESPIPE;

    return file->handlers->seek(file, target, offset, whence);

}

OpenFile* VirtualFileSystemController::fsUserGetNode(void* task, int fd){
    Task* target = (Task*)task;
    spinlockCntWriteAcquire(&target->WLOCK_FILES);
    OpenFile* browse = target->firstFile;
    while(browse){
        if(browse->id == fd)
            break;
        browse = browse->next;
    }
    spinlockCntWriteRelease(&target->WLOCK_FILES);
    return browse;
}

OpenFile* VirtualFileSystemController::fsUserDuplicateNode(void* taskPtr, OpenFile* original){
    Task* task = (Task*)taskPtr;

    OpenFile* target = fsUserDuplicateNodeUnsafe(original);
    target->id = openId++;

    spinlockCntWriteAcquire(&task->WLOCK_FILES);
    LinkedList::LinkedListPushFrontUnsafe((void**)(&task->firstFile), target);
    spinlockCntWriteRelease(&task->WLOCK_FILES);

    return target;
}

OpenFile* VirtualFileSystemController::fsUserDuplicateNodeUnsafe(OpenFile* original){
    OpenFile* orphan = (OpenFile*)MemoryManager::activeMemoryManager->malloc(sizeof(OpenFile));
    orphan->next = 0;
    memcpy((void*)((size_t)orphan + sizeof(orphan->next)), (void*)((size_t)original + sizeof(original->next)), sizeof(OpenFile) - sizeof(orphan->next));

    if(original->handlers->duplicate && !original->handlers->duplicate(original, orphan)){
        MemoryManager::activeMemoryManager->free(orphan);
        return 0;
    }

    return orphan;
}

uint32_t VirtualFileSystemController::fsRead(OpenFile* file, uint8_t* out, uint32_t limit){
    if(!file->handlers->read)
        return -EBADF;
    return file->handlers->read(file, out, limit);
}

uint32_t VirtualFileSystemController::fsWrite(OpenFile* file, uint8_t* in, uint32_t limit){
    if(!(file->flags & O_RDWR) && !(file->flags & O_WRONLY))
        return -EBADF;
    if(!file->handlers->write)
        return -EBADF;
    return file->handlers->write(file, in, limit);
}

void VirtualFileSystemController::fsReadFullFile(OpenFile* file, uint8_t* out){
    fsRead(file, out, fsGetFilesize(file));
}

int VirtualFileSystemController::fsReadLink(void* task, char* path, char* buf, int size){
    Task* target = (Task*)task;
    char* safeFilename = fsSanitize(target->cwd, path);
    MountPoint* mnt = fsDetermineMountPoint(safeFilename);
    int ret = -1;

    char* symlink = 0;
    switch(mnt->filesystem){
        case FS_FATFS:
            ret = -EINVAL;
            break;
        case FS_EXT2:
            break;
        default:
            printf((uint8_t*)"bad filesystem", 20, 20, 0xebdbb2);
            break;
    }

    MemoryManager::activeMemoryManager->free(safeFilename);

    if(symlink){
        char* symlinkResolved = fsResolveSymlink(mnt, symlink);
        MemoryManager::activeMemoryManager->free(symlink);
        ret = fsReadLink(task, symlinkResolved, buf, size);
        MemoryManager::activeMemoryManager->free(symlinkResolved);
    }

    return ret;
}

int VirtualFileSystemController::fsMkdir(void *task, char *path, uint32_t mode){
    Task* target = (Task*)task;
    char* safeFilename = fsSanitize(target->cwd, path);
    MountPoint* mnt = fsDetermineMountPoint(safeFilename);
    int ret = 0;

    char* symlink = 0;
    if(mnt->mkdir)
        ret = mnt->mkdir(mnt, safeFilename, mode, &symlink);
    else
        ret = -EROFS;

    MemoryManager::activeMemoryManager->free(safeFilename);

    if(symlink){
        char* symlinkResolved = fsResolveSymlink(mnt, symlink);
        MemoryManager::activeMemoryManager->free(symlink);
        ret = fsMkdir(task, symlinkResolved, mode);
        MemoryManager::activeMemoryManager->free(symlinkResolved);
    }

    return ret;
}

size_t VirtualFileSystemController::fsGetFilesize(OpenFile* file){
    return file->handlers->getFilesize(file);
}

bool VirtualFileSystemController::isFat(MBR_PARTITION* mbr_partition){
    uint8_t* rawArr = (uint8_t*)MemoryManager::activeMemoryManager->malloc(SECTOR_SIZE);
    mbr->getDiskBytes(rawArr, mbr_partition->lba_first_sector, 1);

    bool ret = (rawArr[66] == 0x28 || rawArr[66] == 0x29);

    MemoryManager::activeMemoryManager->free(rawArr);
    return ret;

}

bool isExt2(MBR_PARTITION* mbr_partition){
    return mbr_partition->type == 0x83;
}

char root[] = "/";
char* VirtualFileSystemController::fsStripMountPoint(const char* filename, MountPoint* mnt){
    char* out = (char*)((size_t)filename + String::strlength(mnt->prefix) - 1);

    if(out[0] != '\0')
        return out;
    else
        return root;
}

void fsSanitizeCopySafe(char* filename, char* safeFilename){
    int i, j;
    for (i = 0, j = 0; filename[i] != '\0'; i++) {
        // double slashes
        if (filename[i] == '/' && filename[i + 1] == '/')
        continue;
        // slashes at the end
        if (filename[i] == '/' && filename[i + 1] == '\0')
        continue;
        if (filename[i] == '/' && filename[i + 1] == '.' && (filename[i + 2] == '/' || filename[i + 2] == '\0')){
            i++; // skip two!
            continue;
        }
        safeFilename[j] = filename[i];
        j++;
    }
    safeFilename[j] = '\0'; // null terminator
    if (j == 0){
        // whoops
        safeFilename[0] = '/';
        safeFilename[1] = '\0';
    }
}

char* VirtualFileSystemController::fsSanitize(char* prefix, char* filename){
    char* safeFilename = 0;
    size_t filenameSize = String::strlength(filename);
    if(filename[0] != '/'){
        size_t cwdLen = String::strlength(prefix);

        safeFilename = (char*)MemoryManager::activeMemoryManager->malloc(cwdLen + 1 + filenameSize + 1);

        int offset = 0;
        memcpy(safeFilename, prefix, cwdLen);
        if(prefix[0] == '/' && prefix[1] != '\0'){
            safeFilename[cwdLen] = '/';
            offset++;
        }

        fsSanitizeCopySafe(filename, safeFilename + offset + cwdLen);
        fsSanitizeCopySafe(safeFilename, safeFilename);
        safeFilename[cwdLen + offset + filenameSize] = '\0';
    }else{
        safeFilename = (char*)MemoryManager::activeMemoryManager->malloc(filenameSize + 1);
        fsSanitizeCopySafe(filename, safeFilename);
    }

    size_t finalSize = String::strlength(safeFilename);

    for (int i = 0; i < finalSize; i++) {
        if (safeFilename[i] == '/' && safeFilename[i + 1] == '.' && safeFilename[i + 2] == '.' && (safeFilename[i + 3] == '/' || safeFilename[i + 3] == '\0')) {
            int indexToCopyInto = 0;
            for (indexToCopyInto = i - 1; indexToCopyInto >= 0; indexToCopyInto--) {
                if (safeFilename[indexToCopyInto] == '/')
                    break;
            }
            if (indexToCopyInto < 0)
                indexToCopyInto = 0;
            // +1 for the null terminator
            size_t lentocopy = finalSize - indexToCopyInto - 3 + 1;
            memcpy(safeFilename + indexToCopyInto, safeFilename + i + 3, lentocopy);
            finalSize -= (i + 3) - indexToCopyInto;
            i = indexToCopyInto - 1; // we copied there...
        }
    }

    if (finalSize == 0) {
        // whoops
        safeFilename[0] = '/';
        safeFilename[1] = '\0';
    }

    return safeFilename;

}

bool VirtualFileSystemController::fsStat(OpenFile* fd, stat* target){
    if(!fd->handlers->stat)
        printf((uint8_t*)"vfs lacks stat handler fd", 20, 20, 0xebdbb2);

    return fd->handlers->stat(fd, target) == 0;
}

bool VirtualFileSystemController::fsStatByFilename(void* task, char* filename, stat* target){
    char* safeFilename = fsSanitize(((Task*)task)->cwd, filename);

    MountPoint* mnt = fsDetermineMountPoint(safeFilename);
    bool ret = false;
    char* strippedFilename = fsStripMountPoint(safeFilename, mnt);

    char* symlink = 0;
    if(!mnt->stat)
        goto cleanup;

    ret = mnt->stat(mnt, strippedFilename, target, &symlink);

cleanup:
    MemoryManager::activeMemoryManager->free(safeFilename);

    if(!ret && symlink){
        char* symlinkResolved = fsResolveSymlink(mnt, symlink);
        MemoryManager::activeMemoryManager->free(symlink);
        bool ret = fsStatByFilename(task, symlinkResolved, target);
        MemoryManager::activeMemoryManager->free(symlinkResolved);
        return ret;
    }

    return ret;
}

bool VirtualFileSystemController::fsLStatByFilename(void* task, char* filename, stat* target){
    char* safeFilename = fsSanitize(((Task*)task)->cwd, filename);

    MountPoint* mnt = fsDetermineMountPoint(safeFilename);
    bool ret = false;
    char* strippedFilename = fsStripMountPoint(safeFilename, mnt);

    char* symlink = 0;
    if(!mnt->lstat)
        goto cleanup;

    ret = mnt->lstat(mnt, strippedFilename, target, &symlink);

cleanup:
    MemoryManager::activeMemoryManager->free(safeFilename);

    if(!ret && symlink){
        char* symlinkResolved = fsResolveSymlink(mnt, symlink);
        MemoryManager::activeMemoryManager->free(symlink);
        bool ret = fsLStatByFilename(task, symlinkResolved, target);
        MemoryManager::activeMemoryManager->free(symlinkResolved);
        return ret;
    }

    return ret;
}

MountPoint* VirtualFileSystemController::fsMount(char* prefix, CONNECTOR connector, uint32_t disk, uint8_t partition){
    MountPoint* mount = (MountPoint*)LinkedList::LinkedListAllocate((void**)&firstMountPoint, sizeof(MountPoint));

    uint32_t strlen = String::strlength(prefix);
    mount->prefix = (char*)(MemoryManager::activeMemoryManager->malloc(strlen + 1));
    memcpy(mount->prefix, prefix, strlen);
    mount->prefix[strlen] = '\0';

    mount->disk = disk;
    mount->partition = partition;
    mount->connector = connector;

    bool ret = false;
    switch(connector){
        case CONNECTOR_AHCI:
            if(!mbr->openDisk(disk, partition, &mount->mbr)){
                fsUnmount(mount);
                return 0;
            }

            if(isFat(&mount->mbr)){
                mount->filesystem = FS_FATFS;
//                 ret = fat32Mount(mount);

            }else if(isExt2(&mount->mbr)){
                mount->filesystem = FS_EXT2;
//                 ret = ext2Mount(mount);
            }
            break;
        case CONNECTOR_DEV:
            mount->filesystem = FS_DEV;
//             ret = devMount(mount);
            break;
        case CONNECTOR_SYS:
            mount->filesystem = FS_SYS;
//             ret = sysMount(mount);
            break;
        default:
            printf((uint8_t*)"Tried to mount with wrong connector", 20, 20, 0xebdbb2);
            ret = 0;
            break;
    }

    if(!ret){
        fsUnmount(mount);
        return 0;
    }

    if(!systemDiskInit && String::strlength(prefix) == 1 && prefix[0] == '/')
        systemDiskInit = true;

    return mount;
}

bool VirtualFileSystemController::fsUnmount(MountPoint* mnt){
    printf((uint8_t*)"vfs tried to unmount", 20, 20, 0xebdbb2);
    LinkedList::LinkedListUnregister((void**)&firstMountPoint, mnt);

    MemoryManager::activeMemoryManager->free(mnt->prefix);
    MemoryManager::activeMemoryManager->free(mnt);

    return true;
}

MountPoint* VirtualFileSystemController::fsDetermineMountPoint(char* filename){
    MountPoint* largestAddr = 0;
    uint32_t largestLen = 0;

    MountPoint* browse = firstMountPoint;
    while(browse){
        size_t len = String::strlength(browse->prefix) - 1;
        if(len >= largestLen && memcmp(filename, browse->prefix, len) == 0 && (filename[len]=='/' || filename[len]=='\0')){
            largestAddr = browse;
            largestLen = len;
        }
        browse = browse->next;
    }

    return largestAddr;
}

char* VirtualFileSystemController::fsResolveSymlink(MountPoint* mnt, char* symlink){
    int symlinkLength = String::strlength(symlink);
    int prefixLength = String::strlength(mnt->prefix) - 1;

    char* ret = 0;
    if(symlink[0] == '/'){
        ret =(char*)MemoryManager::activeMemoryManager->malloc(prefixLength + symlinkLength + 1);
        memcpy(ret, mnt->prefix, prefixLength);
        memcpy(&ret[prefixLength], symlink, symlinkLength);
        ret[prefixLength + symlinkLength] = '\0';
    }else{
        ret =(char*) MemoryManager::activeMemoryManager->malloc(symlinkLength + 1);
        memcpy(ret, &symlink[1], symlinkLength);
        ret[symlinkLength] = '\0';
    }
    return ret;
}



