#include <hardwarecommunication/port.h>
using namespace bljOS::common;
using namespace bljOS::hardwarecommunication;

Port::Port(uint16_t portNumber){
    this->portNumber = portNumber;
}

Port8Bit::Port8Bit(uint16_t portNumber):Port(portNumber){}

void Port8Bit::write(uint8_t data){
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portNumber));
}

uint8_t Port8Bit::read(){
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}

Port8BitSlow::Port8BitSlow(uint16_t portNumber):Port8Bit(portNumber){}

void Port8BitSlow::write(uint8_t data){
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portNumber));
}

Port16Bit::Port16Bit(uint16_t portNumber):Port(portNumber){}

void Port16Bit::write(uint16_t data){
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portNumber));
}

uint16_t Port16Bit::read(){
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}

Port32Bit::Port32Bit(uint16_t portNumber):Port(portNumber){}

void Port32Bit::write(uint32_t data){
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portNumber));
}

uint32_t Port32Bit::read(){
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portNumber));
    return result;
}
