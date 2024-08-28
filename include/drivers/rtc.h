#ifndef __BLJOS__DRIVERS__RTC_H
#define __BLJOS__DRIVERS__RTC_H

#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace bljOS{
    namespace drivers{

        struct DateTime{
            int second;
            int minute;
            int hour;
            int day;
            int month;
            int year;
        };

        struct SystemTime{
            bljOS::common::uint32_t seconds;
            bljOS::common::uint32_t minutes;
            bljOS::common::uint32_t hours;
            bljOS::common::uint32_t days;
            bljOS::common::uint32_t months;
            bljOS::common::uint32_t years;
        };

        class RealTimeClock : {
        private:
            bljOS::hardwarecommunication::Port8Bit rtcPort;
            bljOS::hardwarecommunication::Port8Bit cmosPort;
        public:
            RealTimeClock();

            bljOS::common::uint8_t readRTC(bljOS::common::uint8_t reg);
            int getBCDValue(int value);
            DateTime readCurrentTime();
            void writeRTC(bljOS::common::uint8_t reg, bljOS::common::uint8_t value);
        };

        class ProgrammableIntervalTimer{
        private:
            bljOS::hardwarecommunication::Port8Bit commandPort;
            bljOS::hardwarecommunication::Port8Bit dataPort;
            volatile bljOS::common::uint32_t ticks = 0;
        public:
            SystemTime systemTime = {0, 0, 0, 0, 0, 0};
            ProgrammableIntervalTimer(bljOS::common::uint32_t frequency);

            void timerHandler();
            void updateSystemTime();
        };

        class RTCHandler{
        private:
            RealTimeClock* rtc;
            ProgrammableIntervalTimer* pit;
        public:
            RTCHandler(RealTimeClock* rtc, ProgrammableIntervalTimer* pit);

            void synchronizeTime();
            SystemTime getSystemTime();
            DateTime getCurrentDateTime();
        };
    }
}

#endif
