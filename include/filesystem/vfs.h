#ifndef __BLJOS__FILESYSTEM__VFS_H
#define __BLJOS__FILESYSTEM__VFS_H

#include <common/types.h>
#include <drivers/disk.h>
#include <common/linux.h>
#include <multitasking.h>
#include <memorymanagement.h>
#include <spinlock.h>

#define FS_MODE_READ 0x01
#define FS_MODE_WRITE 0x02
#define FS_MODE_OPEN_EXISTING 0x00
#define FS_MODE_CREATE_NEW 0x04
#define FS_MODE_CREATE_ALWAYS 0x08
#define FS_MODE_OPEN_ALWAYS 0x10
#define FS_MODE_OPEN_APPEND 0x30

#define O_ACCMODE 00000003
#define O_RDONLY 00000000
#define O_WRONLY 00000001
#define O_RDWR 00000002
#define O_CREAT 00000100  /* not fcntl */
#define O_EXCL 00000200   /* not fcntl */
#define O_NOCTTY 00000400 /* not fcntl */
#define O_TRUNC 00001000  /* not fcntl */
#define O_APPEND 00002000
#define O_NONBLOCK 00004000
#define O_DSYNC 00010000  /* used to be O_SYNC, see below */
#define FASYNC 00020000   /* fcntl, for BSD compatibility */
#define O_DIRECT 00040000 /* direct disk access hint */
#define O_LARGEFILE 00100000
#define O_DIRECTORY 00200000 /* must be a directory */
#define O_NOFOLLOW 00400000  /* don't follow links */
#define O_NOATIME 01000000
#define O_CLOEXEC 02000000 /* set close_on_exec */
#define __O_SYNC 04000000
#define O_SYNC (__O_SYNC | O_DSYNC)
#define O_PATH 010000000
#define __O_TMPFILE 020000000
#define O_TMPFILE (__O_TMPFILE | O_DIRECTORY)
#define O_NDELAY O_NONBLOCK

#define SEEK_SET 0  // start + offset
#define SEEK_CURR 1 // current + offset
#define SEEK_END 2  // end + offset

namespace bljOS{
    namespace filesystem{

        typedef enum FS {FS_FATFS, FS_EXT2, FS_DEV, FS_SYS} FS;

        typedef enum CONNECTOR{
            CONNECTOR_AHCI,
            CONNECTOR_DEV,
            CONNECTOR_SYS
        } CONNECTOR;

        struct OpenFile;
        struct MountPoint;

        typedef int (*SpecialReadHandler)(OpenFile* fd, bljOS::common::uint8_t* out, bljOS::common::size_t limit);
        typedef int (*SpecialWriteHandler)(OpenFile* fd, bljOS::common::uint8_t* in, bljOS::common::size_t limit);
        typedef bljOS::common::size_t (*SpecialSeekHandler)(OpenFile* file, bljOS::common::size_t target, long int offset, int whence);
        typedef int (*SpecialIoctlHandler)(OpenFile* fd, bljOS::common::uint64_t request, void* arg);
        typedef int (*SpecialStatHandler)(OpenFile* fd, bljOS::common::stat* stat);
        typedef bljOS::common::size_t (*SpecialMmapHandler)(bljOS::common::size_t addr, bljOS::common::size_t length, int prot, int flags, OpenFile* fd, bljOS::common::size_t pgoffset);
        typedef bool (*SpecialDuplicate)(OpenFile* original, OpenFile* orphan);
        typedef int (*SpecialGetdents64)(OpenFile* fd, struct bljOS::common::linux_dirent64* dirp, unsigned int count);
        typedef bool (*SpecialOpen)(char* filename, OpenFile* fd, char** symlinkResolve);
        typedef bool (*SpecialClose)(OpenFile* fd);
        typedef bljOS::common::size_t (*SpecialGetFilesize)(OpenFile* fd);

        struct VfsHandlers{
            SpecialReadHandler read;
            SpecialWriteHandler write;
            SpecialSeekHandler seek;
            SpecialIoctlHandler ioctl;
            SpecialStatHandler stat;
            SpecialMmapHandler mmap;
            SpecialGetdents64 getdents64;
            SpecialGetFilesize getFilesize;

            SpecialDuplicate duplicate;
            SpecialOpen open;
            SpecialClose close;
        };

        typedef bool (*MntStat)(MountPoint* mnt, char* filename, struct bljOS::common::stat* target, char** symlinkResolve);
        typedef bool (*MntLstat)(MountPoint* mnt, char* filename, struct bljOS::common::stat* target, char** symlinkResolve);
        typedef int (*MntMkdir)(MountPoint *mnt, char *path, bljOS::common::uint32_t mode, char **symlinkResolve);

        struct MountPoint{
            MountPoint* next;

            char* prefix;

            bljOS::common::uint32_t disk;
            bljOS::common::uint8_t partition;
            CONNECTOR connector;

            FS filesystem;

            VfsHandlers* handlers;
            MntStat stat;
            MntLstat lstat;
            MntMkdir mkdir;

            bljOS::drivers::MBR_PARTITION mbr;
            void* fsInfo;
        };

        struct OpenFile{
            OpenFile* next;

            int id;
            int flags;
            int mode;

            bool closeOnExec;

            char* dirname;

            bljOS::common::size_t pointer;
            bljOS::common::size_t tmp1;

            VfsHandlers* handlers;

            MountPoint* mountPoint;
            void* dir;
            void* fakefs;
        };

        class VirtualFileSystemController{
            MountPoint* firstMountPoint;
        public:
            VirtualFileSystemController();

            OpenFile* fsKernelOpen(char* filename, int flags, bljOS::common::uint32_t mode);
            bool fsKernelClose(OpenFile* file);
            int fsUserOpen(void* task, char* path, char* buf, int size);
            int fsUserClose(void* task, int fd);
            int fsUserSeek(void* task, bljOS::common::uint32_t fs, int offset, int whence);
            OpenFile* fsUserGetNode(void* task, int fd);
            OpenFile* fsUserDuplicateNode(void* taskPtr, OpenFile* original);
            OpenFile* fsUserDuplicateNodeUnsafe(OpenFile* original);
            bljOS::common::uint32_t fsRead(OpenFile* file, bljOS::common::uint8_t* out, bljOS::common::uint32_t limit);
            bljOS::common::uint32_t fsWrite(OpenFile* file, bljOS::common::uint8_t* in, bljOS::common::uint32_t limit);
            void fsReadFullFile(OpenFile* file, bljOS::common::uint8_t* out);
            int fsReadLink(void* task, char* path, char* buf, int size);
            int fsMkdir(void *task, char *path,  bljOS::common::uint32_t mode);
            bljOS::common::size_t fsGetFilesize(OpenFile* file);

            char* fsStripMountPoint(const char* filename, MountPoint* mnt);
            char* fsSanitize(char* prefix, char* filename);

            bool fsStat(OpenFile* fd, bljOS::common::stat* target);
            bool fsStatByFilename(void* task, char* filename, bljOS::common::stat* target);
            bool fsLStatByFilename(void* task, char* filename, bljOS::common::stat* target);

            MountPoint* fsMount(char* prefix, CONNECTOR connector, bljOS::common::uint32_t disk, bljOS::common::uint8_t partition);
            bool fsUnmount(MountPoint* mnt);
            MountPoint* fsDetermineMountPoint(char* filename);
            char* fsResolveSymlink(MountPoint* mnt, char* symlink);
        };
    }
}

#endif
