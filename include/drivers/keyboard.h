#ifndef __BLJOS__DRIVERS__KEYBOARD_H
#define __BLJOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

namespace bljOS{
    namespace drivers{
        class KeyboardEventHandler;

        class KeyboardDriver : public bljOS::hardwarecommunication::InterruptHandler, public bljOS::drivers::Driver{
            bljOS::hardwarecommunication::Port8Bit dataport;
            bljOS::hardwarecommunication::Port8Bit commandport;
            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(bljOS::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual bljOS::common::uint32_t handleInterrupt(bljOS::common::uint32_t esp);
            virtual void Activate();
        };

        class KeyboardEventHandler{
        public:
            KeyboardEventHandler();
            virtual void onKeyDown(char);
            virtual void onKeyUp(char);
        };
    }
}
#endif
