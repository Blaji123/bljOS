#ifndef __BLJOS__MULTITASKING_H
#define __BLJOS__MULTITASKING_H

#define NCCS 32
#define KERNEL_TASK_ID 0
#define DPL_USER 3
#define USER_MMAP_START 0x700000000000
#define USER_HEAP_START 0x600000000000
#define USER_STACK_BOTTOM 0x800000000000
#define VINTR     0
#define VQUIT     1
#define VERASE    2
#define VKILL     3
#define VEOF      4
#define VTIME     5
#define VMIN      6
#define VSWTC     7
#define VSTART    8
#define VSTOP     9
#define VSUSP    10
#define VEOL     11
#define VREPRINT 12
#define VDISCARD 13
#define VWERASE  14
#define VLNEXT   15
#define VEOL2    16

#define IGNBRK  0000001
#define BRKINT  0000002
#define IGNPAR  0000004
#define PARMRK  0000010
#define INPCK   0000020
#define ISTRIP  0000040
#define INLCR   0000100
#define IGNCR   0000200
#define ICRNL   0000400
#define IUCLC   0001000
#define IXON    0002000
#define IXANY   0004000
#define IXOFF   0010000
#define IMAXBEL 0020000
#define IUTF8   0040000

#define OPOST  0000001
#define OLCUC  0000002
#define ONLCR  0000004
#define OCRNL  0000010
#define ONOCR  0000020
#define ONLRET 0000040
#define OFILL  0000100
#define OFDEL  0000200
#define NLDLY  0000400
#define NL0    0000000
#define NL1    0000400
#define CRDLY  0003000
#define CR0    0000000
#define CR1    0001000
#define CR2    0002000
#define CR3    0003000
#define TABDLY 0014000
#define TAB0   0000000
#define TAB1   0004000
#define TAB2   0010000
#define TAB3   0014000
#define BSDLY  0020000
#define BS0    0000000
#define BS1    0020000
#define FFDLY  0100000
#define FF0    0000000
#define FF1    0100000

#define VTDLY  0040000
#define VT0    0000000
#define VT1    0040000

#define B0       0000000
#define B50      0000001
#define B75      0000002
#define B110     0000003
#define B134     0000004
#define B150     0000005
#define B200     0000006
#define B300     0000007
#define B600     0000010
#define B1200    0000011
#define B1800    0000012
#define B2400    0000013
#define B4800    0000014
#define B9600    0000015
#define B19200   0000016
#define B38400   0000017

#define B57600   0010001
#define B115200  0010002
#define B230400  0010003
#define B460800  0010004
#define B500000  0010005
#define B576000  0010006
#define B921600  0010007
#define B1000000 0010010
#define B1152000 0010011
#define B1500000 0010012
#define B2000000 0010013
#define B2500000 0010014
#define B3000000 0010015
#define B3500000 0010016
#define B4000000 0010017

#define CSIZE  0000060
#define CS5    0000000
#define CS6    0000020
#define CS7    0000040
#define CS8    0000060
#define CSTOPB 0000100
#define CREAD  0000200
#define PARENB 0000400
#define PARODD 0001000
#define HUPCL  0002000
#define CLOCAL 0004000

#define ISIG   0000001
#define ICANON 0000002
#define ECHO   0000010
#define ECHOE  0000020
#define ECHOK  0000040
#define ECHONL 0000100
#define NOFLSH 0000200
#define TOSTOP 0000400
#define IEXTEN 0100000

#define TCOOFF 0
#define TCOON  1
#define TCIOFF 2
#define TCION  3

#define TCIFLUSH  0
#define TCOFLUSH  1
#define TCIOFLUSH 2

#define TCSANOW   0
#define TCSADRAIN 1
#define TCSAFLUSH 2

#define EXTA    0000016
#define EXTB    0000017
#define CBAUD   0010017
#define CBAUDEX 0010000
#define CIBAUD  002003600000
#define CMSPAR  010000000000
#define CRTSCTS 020000000000

#define XCASE   0000004
#define ECHOCTL 0001000
#define ECHOPRT 0002000
#define ECHOKE  0004000
#define FLUSHO  0010000
#define PENDIN  0040000
#define EXTPROC 0200000

#define XTABS  0014000

#include <common/types.h>
#include <gdt.h>
// #include <filesystem/vfs.h>
#include <spinlock.h>
#include <memorymanagement.h>

namespace bljOS{
    struct CPUState{
        bljOS::common::uint32_t eax;
        bljOS::common::uint32_t ebx;
        bljOS::common::uint32_t ecx;
        bljOS::common::uint32_t edx;

        bljOS::common::uint32_t esi;
        bljOS::common::uint32_t edi;
        bljOS::common::uint32_t ebp;

//         bljOS::common::uint32_t gs;
//         bljOS::common::uint32_t fs;
//         bljOS::common::uint32_t es;
//         bljOS::common::uint32_t ds;

        bljOS::common::uint32_t error;

        bljOS::common::uint32_t eip;
        bljOS::common::uint32_t cs;
        bljOS::common::uint32_t eflags;
        bljOS::common::uint32_t esp;
        bljOS::common::uint32_t ss;
    } __attribute__((packed));

    typedef struct{
        bljOS::common::uint32_t ds; // & es

        bljOS::common::uint32_t edi;
        bljOS::common::uint32_t esi;
        bljOS::common::uint32_t ebp;
        bljOS::common::uint32_t esp;
        bljOS::common::uint32_t ebx;
        bljOS::common::uint32_t edx;
        bljOS::common::uint32_t ecx;
        bljOS::common::uint32_t eax;

        bljOS::common::uint32_t interrupt;
        bljOS::common::uint32_t error;

        bljOS::common::uint32_t eip;
        bljOS::common::uint32_t cs;
        bljOS::common::uint32_t eflags;
        bljOS::common::uint32_t usermode_esp;
        bljOS::common::uint32_t usermode_ss;
    }AsmPassedInterrupt;

    typedef struct termios{
        bljOS::common::uint32_t c_iflag;    /* input mode flags */
        bljOS::common::uint32_t c_oflag;    /* output mode flags */
        bljOS::common::uint32_t c_cflag;    /* control mode flags */
        bljOS::common::uint32_t c_lflag;    /* local mode flags */
        bljOS::common::uint8_t  c_line;     /* line discipline */
        bljOS::common::uint8_t  c_cc[NCCS]; /* control characters */
    }termios;

    typedef struct KilledInfo{
        struct KilledInfo *next;

        bljOS::common::uint32_t pid;
        bljOS::common::uint16_t ret;
    }KilledInfo;

    typedef enum TASK_STATE{
        TASK_STATE_DEAD = 0,
        TASK_STATE_READY = 1,
        TASK_STATE_IDLE = 2,
        TASK_STATE_WAITING_INPUT = 3,
        TASK_STATE_CREATED = 4, // just made by taskCreate()
        TASK_STATE_WAITING_CHILD = 5,
        TASK_STATE_WAITING_CHILD_SPECIFIC = 6, // task->waitingForPid
        TASK_STATE_WAITING_VFORK = 7,
        TASK_STATE_DUMMY = 69,
    }TASK_STATE;

      class Task{
        friend class TaskManager;
    private:
        bljOS::common::uint8_t stack[4096];
        CPUState* cpustate;

        bljOS::common::uint32_t id;
        int pgid;
        bool kernel_task;
        bljOS::common::uint8_t state;

        bljOS::common::uint32_t waitingForPid;

        AsmPassedInterrupt registers;
        bljOS::common::uint32_t* pagedir;
        bljOS::common::uint32_t whileTssRsp;
        bljOS::common::uint32_t whilwSyscallRsp;

        bool systemCallInProgress;
        bool schedPageFault;

        AsmPassedInterrupt* syscallRegs;
        bljOS::common::uint32_t syscallRsp;

        bljOS::common::uint32_t fsbase;
        bljOS::common::uint32_t gsbase;

        bljOS::common::uint32_t heap_start;
        bljOS::common::uint32_t heap_end;

        bljOS::common::uint32_t mmap_start;
        bljOS::common::uint32_t mmap_end;

        termios term;
        bljOS::common::uint32_t tmpRecV;

        char* cwd;
        bljOS::common::uint32_t umask;

        SpinlockCnt WLOCK_FILES;
//         bljOS::filesystem::OpenFile* firstFile;

        __attribute__((aligned(16))) bljOS::common::uint8_t fpuenv[512];
        bljOS::common::uint32_t mxcsr;

        bool noInformParent;

        Spinlock LOCK_CHILD_TERM;
        KilledInfo* firstChildTerminated;
        int childrenTerminatedAmnt;

        Task* parent;
        Task* next;
    public:
        Task(bljOS::GlobalDescriptorTable* gdt, bljOS::common::uint32_t rip, bool kernel_task, bljOS::common::uint32_t* pagedir, bljOS::common::uint32_t argc, char** argv);
        ~Task();

        void taskAttachDefTermios(Task *task);
    };

    SpinlockCnt TASK_LL_MODIFY;


    class TaskManager{
    protected:
        Task* firstTask;
        Task* currentTask;
    public:
        TaskManager();
        ~TaskManager();
        Task* addTask(bljOS::GlobalDescriptorTable* gdt, bljOS::common::uint32_t rip, bool kernel_task, bljOS::common::uint32_t* pagedir, bljOS::common::uint32_t argc, char** argv);
        CPUState* Schedule(CPUState* cpustate);
    };
}

#endif
