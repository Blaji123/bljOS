#ifndef __BLJOS__DRIVERS__MOUSE_H
#define __BLJOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>

namespace bljOS{
    namespace drivers{
        class MouseEventHandler{
        public:

            MouseEventHandler();
            virtual void OnActivate();
            virtual void onMouseDown(bljOS::common::uint8_t buttons);
            virtual void onMouseUp(bljOS::common::uint8_t buttons);
            virtual void onMouseMove(int x, int y);
        };


        class MouseDriver : public bljOS::hardwarecommunication::InterruptHandler, public Driver{
            bljOS::hardwarecommunication::Port8Bit dataport;
            bljOS::hardwarecommunication::Port8Bit commandport;

            bljOS::common::uint8_t buffer[3];
            bljOS::common::uint8_t offset;
            bljOS::common::uint8_t buttons;
            MouseEventHandler* handler;
        public:
            MouseDriver(bljOS::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual bljOS::common::uint32_t handleInterrupt(bljOS::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif
