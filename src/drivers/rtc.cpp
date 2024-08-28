#include <drivers/rtc.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;

RealTimeClock::RealTimeClock():rtcPort(0x70),cmosPort(0x71){
    asm("cli");
    rtcPort.write(0x8A);
    cmosPort.write(0x20);
    asm("sti");

    asm("cli");
    rtcPort.write(0x8B);
    uint8_t prev = cmosPort.read();
    rtcPort.write(0x8B);
    cmosPort.write(prev | 0x40);
    asm("sti");

    rtcPort.write(0x0C);
    cmosPort.read();
}

uint8_t RealTimeClock::readRTC(uint8_t reg){
    rtcPort.write(reg);
    return cmosPort.read();
}

int RealTimeClock::getBCDValue(int value){
    return ((value / 16) * 10) + (value & 0x0F);
}

DateTime RealTimeClock::readCurrentTime(){
    DateTime dateTime;
    dateTime.second = getBCDValue(readRTC(0x00));
    dateTime.minute = getBCDValue(readRTC(0x02));
    dateTime.hour = getBCDValue(readRTC(0x04));
    dateTime.day = getBCDValue(readRTC(0x07));
    dateTime.month = getBCDValue(readRTC(0x08));
    dateTime.year = getBCDValue(readRTC(0x09));
    dateTime.year += 2000;
    return dateTime;
}

void RealTimeClock::writeRTC(uint8_t reg, uint8_t value){
    rtcPort.write(reg);
    cmosPort.write(value);
}

ProgrammableIntervalTimer::ProgrammableIntervalTimer(uint32_t frequency):dataPort(0x40), commandPort(0x43){
    uint32_t divisor = 1193180 / frequency;
    commandPort.write(0x36);
    dataPort.write(divisor & 0xFF);
    dataPort.write(divisor >> 8);
}

void ProgrammableIntervalTimer::updateSystemTime(){
    systemTime.seconds++;
    if(systemTime.seconds >= 60){
        systemTime.seconds = 0;
        systemTime.minutes++;
        if(systemTime.minutes >= 60){
            systemTime.minutes = 0;
            systemTime.hours++;
            if(systemTime.hours >= 24){
                systemTime.hours = 0;
                systemTime.days++;
                if(systemTime.days >= 32){
                    systemTime.days = 1;
                    systemTime.months++;
                    if(systemTime.months >= 13){
                        systemTime.months = 1;
                        systemTime.years++;
                    }
                }
            }
        }
    }
}

void ProgrammableIntervalTimer::timerHandler(){
    ticks++;
    if(ticks%100 == 0){
        updateSystemTime();
    }
}

RTCHandler::RTCHandler(RealTimeClock* rtc, ProgrammableIntervalTimer* pit){
    this->rtc = rtc;
    this->pit = pit;

    synchronizeTime();
}

void RTCHandler::synchronizeTime(){
    DateTime rtcTime = rtc->readCurrentTime();
    pit->systemTime.seconds = rtcTime.second;
    pit->systemTime.minutes = rtcTime.minute;
    pit->systemTime.hours = rtcTime.hour;
    pit->systemTime.days = rtcTime.day;
    pit->systemTime.months = rtcTime.month;
    pit->systemTime.years = rtcTime.year;
}

SystemTime RTCHandler::getSystemTime(){
    return pit->systemTime;
}

DateTime RTCHandler::getCurrentDateTime(){
    return rtc->readCurrentTime();
}






