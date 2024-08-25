#ifndef __BLJOS__SYSCALLS_H
#define __BLJOS__SYSCALLS_H

#include <hardwarecommunication/interrupts.h>
#include <common/types.h>
#include <multitasking.h>

namespace bljOS{
    class SyscallHandler : public bljOS::hardwarecommunication::InterruptHandler{
    public:
        SyscallHandler(bljOS::hardwarecommunication::InterruptManager* interruptManager, bljOS::common::uint8_t interruptNumber);
        ~SyscallHandler();
        bljOS::common::uint32_t handleInterrupt(bljOS::common::uint32_t esp);
    };
}

#endif
