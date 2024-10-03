#ifndef __BLJOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __BLJOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <gdt.h>
#include <multitasking.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace bljOS{
    namespace hardwarecommunication{

        class TaskManager;
        class InterruptManager;

        class InterruptHandler{
        protected:
            bljOS::common::uint8_t interruptNumber;
            InterruptManager* interruptManager;

            InterruptHandler(InterruptManager* interruptManager, bljOS::common::uint8_t interruptNumber);
            ~InterruptHandler();

        public:
            virtual bljOS::common::uint32_t handleInterrupt(bljOS::common::uint32_t esp);
        };

        class InterruptManager{
            friend class InterruptHandler;
        protected:
            static InterruptManager* activeInterruptManager;
            InterruptHandler* handlers[256];
            bljOS::TaskManager* taskManager;

            struct GateDescriptor{
                bljOS::common::uint16_t handlerAddressLowBits;
                bljOS::common::uint16_t gdtCodeSegmentSelector;
                bljOS::common::uint8_t reserved;
                bljOS::common::uint8_t access;
                bljOS::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];
            struct InterruptDescriptorTablePointer{
                bljOS::common::uint16_t size;
                bljOS::common::uint32_t base;
            }__attribute__((packed));

            bljOS::common::uint16_t hardwareInterruptOffset;

            static void setInterruptDescriptorTableEntry(bljOS::common::uint8_t interruptNumber, bljOS::common::uint16_t codeSegmentSelectorOffset, void (*handler)(), bljOS::common::uint8_t descriptorPrivilegeLevel, bljOS::common::uint8_t descriptorType);

            static bljOS::common::uint32_t handleInterrupt(bljOS::common::uint8_t interruptNumber, bljOS::common::uint32_t esp);
            bljOS::common::uint32_t doHandleInterrupt(bljOS::common::uint8_t interruptNumber, bljOS::common::uint32_t esp);

            static void InterruptIgnore();
            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();
            static void HandleInterruptRequest0x80();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            Port8BitSlow picMasterCommand;
            Port8BitSlow picMasterData;
            Port8BitSlow picSlaveCommand;
            Port8BitSlow picSlaveData;

        public:
            InterruptManager(bljOS::common::uint16_t hardwareInterruptOffset, bljOS::GlobalDescriptorTable* gdt, bljOS::TaskManager* taskManager);
            ~InterruptManager();
            bljOS::common::uint16_t getHardwareInterruptOffset();
            void activate();
            void deactivate();
        };
    }
}
#endif
