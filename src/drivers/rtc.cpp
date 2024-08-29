#include <drivers/rtc.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::hardwarecommunication;
using namespace bljOS::drivers;

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);
void printfHex(uint8_t key, int32_t x, int32_t y, uint32_t color);

RTCEventHandler::RTCEventHandler(){}

RealTimeClock::RealTimeClock(InterruptManager* manager, RTCEventHandler* handler):InterruptHandler(manager, 0x28),handler(handler),rtcPort(0x70),cmosPort(0x71),lastMinute(0){

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

uint32_t RealTimeClock::handleInterrupt(uint32_t esp){
    rtcPort.write(0x0C);
    cmosPort.read();

    DateTime currentTime = readCurrentTime();

    if(handler!=0 && currentTime.minute != lastMinute){
        handler->onTimeChange(currentTime);
    }

    lastMinute = currentTime.minute;
    return esp;
}

void RealTimeClock::Activate(){
    rtcPort.write(0x8A);
    cmosPort.write(0x20);

    rtcPort.write(0x8B);
    uint8_t prev = cmosPort.read();
    rtcPort.write(0x8B);
    cmosPort.write(prev | 0x40);

    uint8_t rate = 6;
    rate &= 0x0F;

    rtcPort.write(0x8A);
    prev = cmosPort.read();
    rtcPort.write(0x8A);
    cmosPort.write((prev & 0xF0) | rate);

    rtcPort.write(0x0C);
    cmosPort.read();
}






