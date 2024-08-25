#include <syscalls.h>

using namespace bljOS;
using namespace bljOS::hardwarecommunication;
using namespace bljOS::common;

void printf(char*);
void printfHex(uint8_t);

SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t interruptNumber):InterruptHandler(interruptManager, interruptNumber+interruptManager->getHardwareInterruptOffset()){

}

SyscallHandler::~SyscallHandler(){

}

uint32_t SyscallHandler::handleInterrupt(uint32_t esp){
    CPUState* cpu = (CPUState*)esp;
    switch(cpu->eax){
        case 4:
            printf((char*)cpu->ebx);
            break;
        default:
            break;
    }
    return esp;
}
