#ifndef __BLJOS__DRIVERS__RTC_H
#define __BLJOS__DRIVERS__RTC_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>

namespace bljOS{
    namespace drivers{

        struct DateTime{
            bljOS::common::uint8_t second;
            bljOS::common::uint8_t minute;
            bljOS::common::uint8_t hour;
            bljOS::common::uint8_t day;
            bljOS::common::uint8_t month;
            bljOS::common::uint8_t year;
        };

        class RTCEventHandler{
        public:
            RTCEventHandler();
            virtual void onTimeChange(const DateTime& time) = 0;
        };

        class RealTimeClock : public bljOS::hardwarecommunication::InterruptHandler, public Driver{
        private:
            bljOS::hardwarecommunication::Port8Bit rtcPort;
            bljOS::hardwarecommunication::Port8Bit cmosPort;
            RTCEventHandler* handler;
            int lastMinute;
        public:
            RealTimeClock(bljOS::hardwarecommunication::InterruptManager* manager, RTCEventHandler* handler);

            virtual bljOS::common::uint32_t handleInterrupt(bljOS::common::uint32_t esp) ;
            virtual void Activate();
            bljOS::common::uint8_t readRTC(bljOS::common::uint8_t reg);
            bljOS::common::uint8_t getBCDValue(bljOS::common::uint8_t value);
            DateTime readCurrentTime();
        };
    }
}

#endif
