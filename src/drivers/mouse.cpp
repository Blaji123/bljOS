#include <drivers/mouse.h>

using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::hardwarecommunication;

void printf(char*);
void printfHex(uint8_t);

MouseEventHandler::MouseEventHandler(){}

void MouseEventHandler::OnActivate(){}

void MouseEventHandler::onMouseDown(uint8_t buttons){}

void MouseEventHandler::onMouseUp(uint8_t buttons){}

void MouseEventHandler::onMouseMove(int x, int y){}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler):InterruptHandler(manager, 0x2C), dataport(0x60), commandport(0x64){
    this->handler = handler;
}

MouseDriver::~MouseDriver(){}

void MouseDriver::Activate(){
    offset = 0;
    buttons = 0;

    commandport.write(0xA8);
    commandport.write(0x20);
    uint8_t status = dataport.read() | 2;
    commandport.write(0x60);
    dataport.write(status);

    commandport.write(0xD4);
    dataport.write(0xF4);
    dataport.read();
}

uint32_t MouseDriver::handleInterrupt(uint32_t esp){

    uint8_t status = commandport.read();
    if(!(status & 0x20))
        return esp;

    buffer[offset] = dataport.read();

    if(handler == 0)
        return esp;

    offset = (offset + 1) % 3;
    if(offset == 0){
        handler->onMouseMove((int8_t)buffer[1], -((int8_t)buffer[2]));

        for(uint8_t i=0;i<3;i++){
            if((buffer[0] & (0x01 << i))!= (buttons & (0x01<<i))){
                if(buttons & (0x01<<i))
                    handler->onMouseUp(i+1);
                else
                    handler->onMouseDown(i+1);
            }

        }
        buttons = buffer[0];
    }
    return esp;
}
