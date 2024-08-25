#include <hardwarecommunication/interrupts.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::hardwarecommunication;

void printf(char*);
void printfHex(uint8_t);

InterruptHandler::InterruptHandler(InterruptManager* interruptManager, uint8_t interruptNumber){
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler(){
     if(interruptManager->handlers[interruptNumber] == this)
        interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::handleInterrupt(uint32_t esp){
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];
InterruptManager* InterruptManager::activeInterruptManager = 0;

void InterruptManager::setInterruptDescriptorTableEntry(uint8_t interruptNumber, uint16_t codeSegmentSelectorOffset, void (*handler)(), uint8_t descriptorPrivilegeLevel, uint8_t descriptorType){
    interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].handlerAddressHighBits = (((uint32_t)handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interruptNumber].gdtCodeSegmentSelector = codeSegmentSelectorOffset;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | descriptorType | ((descriptorPrivilegeLevel&3) << 5);
    interruptDescriptorTable[interruptNumber].reserved = 0;
}

InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, TaskManager* taskManager):picMasterCommand(0x20),picMasterData(0x21),picSlaveCommand(0xA0),picSlaveData(0xA1){
    this->taskManager = taskManager;
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint32_t codeSegment = gdt->getCodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint8_t i = 255; i > 0; --i){
        setInterruptDescriptorTableEntry(i, codeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
        handlers[i] = 0;
    }
    setInterruptDescriptorTableEntry(0, codeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    handlers[0] = 0;

    setInterruptDescriptorTableEntry(0x00, codeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x01, codeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x02, codeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x03, codeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x04, codeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x05, codeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x06, codeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x07, codeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x08, codeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x09, codeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0A, codeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0B, codeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0C, codeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0D, codeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0E, codeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x0F, codeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x10, codeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x11, codeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x12, codeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(0x13, codeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x02, codeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x03, codeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x04, codeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x05, codeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x06, codeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x07, codeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x08, codeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x09, codeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0A, codeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0B, codeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, codeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0D, codeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0E, codeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    setInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0F, codeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

    setInterruptDescriptorTableEntry(0x80, codeSegment, &HandleInterruptRequest0x80, 0, IDT_INTERRUPT_GATE);

    picMasterCommand.write(0x11);
    picSlaveCommand.write(0x11);

    picMasterData.write(hardwareInterruptOffset);
    picSlaveData.write(hardwareInterruptOffset+8);

    picMasterData.write(0x04);
    picSlaveData.write(0x02);

    picMasterData.write(0x01);
    picSlaveData.write(0x01);

    picMasterData.write(0x00);
    picSlaveData.write(0x00);

    InterruptDescriptorTablePointer idt;
    idt.size = 256 * sizeof(GateDescriptor) - 1;
    idt.base = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt));
}

InterruptManager::~InterruptManager(){
    deactivate();
}

uint16_t InterruptManager::getHardwareInterruptOffset(){
    return hardwareInterruptOffset;
}

void InterruptManager::activate(){
    if(activeInterruptManager!=0)
        activeInterruptManager->deactivate();
    activeInterruptManager = this;
    asm("sti");
}

void InterruptManager::deactivate(){
    if(activeInterruptManager==this){
        activeInterruptManager=0;
        asm("cli");
    }
}

uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp){
    if(activeInterruptManager!=0){
        return activeInterruptManager->doHandleInterrupt(interruptNumber, esp);
    }
    return esp;
}

uint32_t InterruptManager::doHandleInterrupt(uint8_t interruptNumber, uint32_t esp){
        if(handlers[interruptNumber]!=0){
            esp = handlers[interruptNumber]->handleInterrupt(esp);
        }else if(interruptNumber!=hardwareInterruptOffset){
            printf("UNHANDLED INTERRUPT 0x");
            printfHex(interruptNumber);
        }

        if(interruptNumber == hardwareInterruptOffset){
            esp = (uint32_t)taskManager->Schedule((CPUState*)esp);
        }

        if(hardwareInterruptOffset <= interruptNumber && interruptNumber < hardwareInterruptOffset + 16){
            picMasterCommand.write(0x20);
            if(hardwareInterruptOffset + 8 <= interruptNumber)
                picSlaveCommand.write(0x20);
        }
        return esp;
}
