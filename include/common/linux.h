#ifndef __BLJOS__COMMON__LINUX_H
#define __BLJOS__COMMON__LINUX_H

#include <common/types.h>

typedef bljOS::common::int32_t   pid_t;
typedef bljOS::common::int32_t   uid_t;
typedef bljOS::common::int32_t   sigval_t;
typedef bljOS::common::uint32_t  timer_t;
typedef bljOS::common::uint32_t  mqd_t;

// /usr/include/x86_64-linux-gnu/bits/typesizes.h
#define __SYSCALL_SLONG_TYPE __SLONGWORD_TYPE
#define __SYSCALL_ULONG_TYPE __ULONGWORD_TYPE

#define __DEV_T_TYPE __UQUAD_TYPE
#define __UID_T_TYPE __U32_TYPE
#define __GID_T_TYPE __U32_TYPE
#define __INO_T_TYPE __SYSCALL_ULONG_TYPE
#define __INO64_T_TYPE __UQUAD_TYPE
#define __MODE_T_TYPE __U32_TYPE

#define __NLINK_T_TYPE __SYSCALL_ULONG_TYPE
#define __FSWORD_T_TYPE __SYSCALL_SLONG_TYPE

#define __OFF_T_TYPE __SYSCALL_SLONG_TYPE
#define __OFF64_T_TYPE __SQUAD_TYPE
#define __PID_T_TYPE __S32_TYPE
#define __RLIM_T_TYPE __SYSCALL_ULONG_TYPE
#define __RLIM64_T_TYPE __UQUAD_TYPE
#define __BLKCNT_T_TYPE __SYSCALL_SLONG_TYPE
#define __BLKCNT64_T_TYPE __SQUAD_TYPE
#define __FSBLKCNT_T_TYPE __SYSCALL_ULONG_TYPE
#define __FSBLKCNT64_T_TYPE __UQUAD_TYPE
#define __FSFILCNT_T_TYPE __SYSCALL_ULONG_TYPE
#define __FSFILCNT64_T_TYPE __UQUAD_TYPE
#define __ID_T_TYPE __U32_TYPE
#define __CLOCK_T_TYPE __SYSCALL_SLONG_TYPE
#define __TIME_T_TYPE __SYSCALL_SLONG_TYPE
#define __USECONDS_T_TYPE __U32_TYPE
#define __SUSECONDS_T_TYPE __SYSCALL_SLONG_TYPE
#define __SUSECONDS64_T_TYPE __SQUAD_TYPE
#define __DADDR_T_TYPE __S32_TYPE
#define __KEY_T_TYPE __S32_TYPE
#define __CLOCKID_T_TYPE __S32_TYPE
#define __TIMER_T_TYPE void *
#define __BLKSIZE_T_TYPE __SYSCALL_SLONG_TYPE
#define __FSID_T_TYPE                                                          \
  struct {                                                                     \
    int __val[2];                                                              \
  }
#define __SSIZE_T_TYPE __SWORD_TYPE
#define __CPU_MASK_TYPE __SYSCALL_ULONG_TYPE

// /usr/include/x86_64-linux-gnu/bits/types.h
#define __S16_TYPE short int
#define __U16_TYPE unsigned short int
#define __S32_TYPE int
#define __U32_TYPE unsigned int
#define __SLONGWORD_TYPE long int
#define __ULONGWORD_TYPE unsigned long int

#define __SQUAD_TYPE long int
#define __UQUAD_TYPE unsigned long int
#define __SWORD_TYPE long int
#define __UWORD_TYPE unsigned long int
#define __SLONG32_TYPE int
#define __ULONG32_TYPE unsigned int
#define __S64_TYPE long int
#define __U64_TYPE unsigned long int
/* No need to mark the typedef with __extension__.   */
#define __STD_TYPE typedef

__STD_TYPE __DEV_T_TYPE   __dev_t;   /* Type of device numbers.  */
__STD_TYPE __UID_T_TYPE   __uid_t;   /* Type of user identifications.  */
__STD_TYPE __GID_T_TYPE   __gid_t;   /* Type of group identifications.  */
__STD_TYPE __INO_T_TYPE   __ino_t;   /* Type of file serial numbers.  */
__STD_TYPE __INO64_T_TYPE __ino64_t; /* Type of file serial numbers (LFS).*/
__STD_TYPE __MODE_T_TYPE  __mode_t;  /* Type of file attribute bitmasks.  */
__STD_TYPE __NLINK_T_TYPE __nlink_t; /* Type of file link counts.  */
__STD_TYPE __OFF_T_TYPE   __off_t;   /* Type of file sizes and offsets.  */
__STD_TYPE __OFF64_T_TYPE __off64_t; /* Type of file sizes and offsets (LFS). */
__STD_TYPE __PID_T_TYPE   __pid_t;   /* Type of process identifications.  */
__STD_TYPE __FSID_T_TYPE  __fsid_t;  /* Type of file system IDs.  */
__STD_TYPE __CLOCK_T_TYPE __clock_t; /* Type of CPU usage counts.  */
__STD_TYPE __RLIM_T_TYPE  __rlim_t;  /* Type for resource measurement.  */
__STD_TYPE __RLIM64_T_TYPE
                         __rlim64_t; /* Type for resource measurement (LFS).  */
__STD_TYPE __ID_T_TYPE   __id_t;     /* General type for IDs.  */
__STD_TYPE __TIME_T_TYPE __time_t;   /* Seconds since the Epoch.  */
__STD_TYPE __USECONDS_T_TYPE  __useconds_t;  /* Count of microseconds.  */
__STD_TYPE __SUSECONDS_T_TYPE __suseconds_t; /* Signed count of microseconds. */
__STD_TYPE __SUSECONDS64_T_TYPE __suseconds64_t;
__STD_TYPE __DADDR_T_TYPE __daddr_t; /* The type of a disk address.  */
__STD_TYPE __KEY_T_TYPE   __key_t;   /* Type of an IPC key.  */

/* Clock ID used in clock and timer functions.  */
__STD_TYPE __CLOCKID_T_TYPE __clockid_t;

/* Timer ID returned by `timer_create'.  */
__STD_TYPE __TIMER_T_TYPE __timer_t;

/* Type to represent block size.  */
__STD_TYPE __BLKSIZE_T_TYPE __blksize_t;

/* Types from the Large File Support interface.  */

/* Type to count number of disk blocks.  */
__STD_TYPE __BLKCNT_T_TYPE   __blkcnt_t;
__STD_TYPE __BLKCNT64_T_TYPE __blkcnt64_t;

/* Type to count file system blocks.  */
__STD_TYPE __FSBLKCNT_T_TYPE   __fsblkcnt_t;
__STD_TYPE __FSBLKCNT64_T_TYPE __fsblkcnt64_t;

/* Type to count file system nodes.  */
__STD_TYPE __FSFILCNT_T_TYPE   __fsfilcnt_t;
__STD_TYPE __FSFILCNT64_T_TYPE __fsfilcnt64_t;

/* Type of miscellaneous file system fields.  */
__STD_TYPE __FSWORD_T_TYPE __fsword_t;

__STD_TYPE __SSIZE_T_TYPE __ssize_t; /* Type of a byte count, or error.  */

/* Signed long type used in system calls.  */
__STD_TYPE __SYSCALL_SLONG_TYPE __syscall_slong_t;
/* Unsigned long type used in system calls.  */
__STD_TYPE __SYSCALL_ULONG_TYPE __syscall_ulong_t;

namespace bljOS{
    namespace common{

        typedef struct timespec {
            int64_t tv_sec;  // seconds
            int64_t tv_nsec; // nanoseconds
        } timespec;

        typedef struct stat {
            #ifdef __USE_TIME_BITS64
            #include <bits/struct_stat_time64_helper.h>
            #else
            __dev_t st_dev; /* Device.  */
            #ifndef __x86_64__
            unsigned short int __pad1;
            #endif
            #if defined __x86_64__ || !defined __USE_FILE_OFFSET64
            __ino_t st_ino; /* File serial number.	*/
            #else
            __ino_t __st_ino; /* 32bit file serial number.	*/
            #endif
            #ifndef __x86_64__
            __mode_t  st_mode;  /* File mode.  */
            __nlink_t st_nlink; /* Link count.  */
            #else
            __nlink_t st_nlink; /* Link count.  */
            __mode_t  st_mode;  /* File mode.  */
            #endif
            __uid_t st_uid; /* User ID of the file's owner.	*/
            __gid_t st_gid; /* Group ID of the file's group.*/
            #ifdef __x86_64__
            int __pad0;
            #endif
            __dev_t st_rdev; /* Device number, if device.  */
            #ifndef __x86_64__
            unsigned short int __pad2;
            #endif
            #if defined __x86_64__ || !defined __USE_FILE_OFFSET64
            __off_t st_size; /* Size of file, in bytes.  */
            #else
            __off64_t st_size; /* Size of file, in bytes.  */
            #endif
            __blksize_t st_blksize; /* Optimal block size for I/O.  */
            #if defined __x86_64__ || !defined __USE_FILE_OFFSET64
            __blkcnt_t st_blocks; /* Number 512-byte blocks allocated. */
            #else
            __blkcnt64_t st_blocks; /* Number 512-byte blocks allocated. */
            #endif
            #ifdef __USE_XOPEN2K8
            /* Nanosecond resolution timestamps are stored in a format
                equivalent to 'struct timespec'.  This is the type used
                whenever possible but the Unix namespace rules do not allow the
                identifier 'timespec' to appear in the <sys/stat.h> header.
                Therefore we have to handle the use of this header in strictly
                standard-compliant sources special.  */
            struct timespec st_atim; /* Time of last access.  */
            struct timespec st_mtim; /* Time of last modification.  */
            struct timespec st_ctim; /* Time of last status change.  */
            #define st_atime st_atim.tv_sec /* Backward compatibility.  */
            #define st_mtime st_mtim.tv_sec
            #define st_ctime st_ctim.tv_sec
            #else
            __time_t          st_atime;     /* Time of last access.  */
            __syscall_ulong_t st_atimensec; /* Nscecs of last access.  */
            __time_t          st_mtime;     /* Time of last modification.  */
            __syscall_ulong_t st_mtimensec; /* Nsecs of last modification.  */
            __time_t          st_ctime;     /* Time of last status change.  */
            __syscall_ulong_t st_ctimensec; /* Nsecs of last status change.  */
            #endif
            #ifdef __x86_64__
            __syscall_slong_t __glibc_reserved[3];
            #else
            #ifndef __USE_FILE_OFFSET64
            unsigned long int __glibc_reserved4;
            unsigned long int __glibc_reserved5;
            #else
            __ino64_t st_ino; /* File serial number.	*/
            #endif
            #endif
            #endif /* __USE_TIME_BITS64  */
        } stat;

        struct linux_dirent64 {
            uint64_t       d_ino;
            int64_t        d_off;
            unsigned short d_reclen;
            unsigned char  d_type;
            char           d_name[];
                // unsigned long  d_ino;    /* Inode number */
                // unsigned long  d_off;    /* Offset to next linux_dirent */
                // unsigned short d_reclen; /* Length of this linux_dirent */
                // char           d_name[]; /* Filename (null-terminated) */
                /* length is actually (d_reclen - 2 -
                    offsetof(struct linux_dirent, d_name) */
                /*
                char           pad;       // Zero padding byte
                char           d_type;    // File type (only since Linux 2.6.4;
                                            // offset is (d_reclen - 1))
                */
        };
    }
}

#endif
