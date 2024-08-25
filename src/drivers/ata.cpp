#include <drivers/ata.h>

using namespace bljOS;
using namespace bljOS::drivers;
using namespace bljOS::hardwarecommunication;
using namespace bljOS::common;

void printf(char*);
void printfHex(uint8_t);

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(uint16_t portBase, bool master):dataPort(portBase),errorPort(portBase + 0x1),sectorCountPort(portBase + 0x2),lbaLowPort(portBase + 0x3),lbaMidPort(portBase + 0x4),lbaHiPort(portBase + 0x5),devicePort(portBase + 0x6),commandPort(portBase + 0x7),controlPort(portBase + 0x206){
    bytesPerSector = 512;
    this->master = master;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment(){

}

void AdvancedTechnologyAttachment::Identify(){
    devicePort.write(master ? 0xA0 : 0xB0);
    controlPort.write(0);
    devicePort.write(0xA0);
    uint8_t status = commandPort.read();
    if(status == 0xFF)
        return;
    devicePort.write(master ? 0xA0 : 0xB0);
    sectorCountPort.write(0);
    lbaLowPort.write(0);
    lbaMidPort.write(0);
    lbaLowPort.write(0);
    commandPort.write(0xEC);

    status = commandPort.read();
    if(status == 0x00)
        return;
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.read();
    if(status & 0x01){
        printf("ERROR");
        return;
    }
    for(int i=0;i<256;i++){
        uint16_t data = dataPort.read();
        char* foo = " \0";
        foo[0] = (data >> 8) & 0xFF;
        foo[1] = data & 0xFF;
        printf(foo);
    }
    printf("\n");
}

void AdvancedTechnologyAttachment::Read28(uint32_t sector, uint8_t* data, int count){
    if(sector > 0x0FFFFFFF)
        return;
    devicePort.write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    errorPort.write(0);
    sectorCountPort.write(1);

    lbaLowPort.write(sector & 0x000000FF);
    lbaMidPort.write((sector & 0x0000FF00) >> 8);
    lbaLowPort.write((sector & 0x00FF0000) >> 16);
    commandPort.write(0x20);

    uint8_t status = commandPort.read();
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.read();
    if(status & 0x01){
        printf("ERROR");
        return;
    }
     printf("Reading from ATA: ");

    for(int i=0;i<count;i+=2){
        uint16_t wdata = dataPort.read();

        /*
        char *text = "  \0";
        text[0] = wdata & 0xFF;

        if(i+1 < count)
            text[1] = (wdata >> 8) & 0xFF;
        else
            text[1] = '\0';

        printf(text);
        */
    }
    for(int i=count + (count%2);i<512;i+=2)
        dataPort.read();
}

void AdvancedTechnologyAttachment::Write28(uint32_t sector,uint8_t* data, int count){
   if(sector > 0x0FFFFFFF)
        return;
    if(count > 512)
        return;
    devicePort.write((master ? 0xE0 : 0xF0) | ((sector & 0x0F000000) >> 24));
    errorPort.write(0);
    sectorCountPort.write(1);

    lbaLowPort.write(sector & 0x000000FF);
    lbaMidPort.write((sector & 0x0000FF00) >> 8);
    lbaLowPort.write((sector & 0x00FF0000) >> 16);
    commandPort.write(0x30);

    printf("Writing to ATA: ");

    for(int i=0;i<count;i+=2){
        uint16_t wdata = data[i];
        if(i + 1 < count)
            wdata |= ((uint16_t)data[i+1]) << 8;
        dataPort.write(wdata);

        char* foo = " \0";
        foo[0] = (wdata >> 8) & 0xFF;
        foo[1] = wdata & 0xFF;
        printf(foo);
    }
    for(uint16_t i=count + (count%2);i<512;i+=2)
        dataPort.write(0x0000);
}

void AdvancedTechnologyAttachment::Flush(){
    devicePort.write(master ? 0xE0 : 0xF0);
    commandPort.write(0xE7);
    uint8_t status = commandPort.read();
    if(status == 0x00)
        return;
    status = commandPort.read();
    while(((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.read();
    if(status & 0x01){
        printf("ERROR");
        return;
    }
}
