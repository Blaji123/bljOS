#ifndef __BLJOS__MULTITASKING_H
#define __BLJOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

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

    class Task{
        friend class TaskManager;
    private:
        bljOS::common::uint8_t stack[4096];
        CPUState* cpustate;
    public:
        Task(bljOS::GlobalDescriptorTable* gdt, void entrypoint());
        ~Task();
    };

    class TaskManager{
    private:
        Task* tasks[256];
        int numTasks;
        int currentTask;
    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task);
        CPUState* Schedule(CPUState* cpustate);

    };
}

#endif
