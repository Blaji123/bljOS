#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/widget.h>
#include <gui/window.h>
#include <memorymanagement.h>
#include <multitasking.h>
#include <drivers/amd_am79c973.h>
#include <drivers/ata.h>
#include <syscalls.h>
#include <net/etherframe.h>
#include <net/arp.h>
#include <net/ipv4.h>
#include <net/icmp.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <filesystem/msdospart.h>
#include <common/multiboot.h>
#include <drivers/rtc.h>
#include <gui/button.h>
#include <gui/toolbar.h>
#include <drivers/ahci.h>
#include <drivers/disk.h>
#include <filesystem/vfs.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::hardwarecommunication;
using namespace bljOS::gui;
using namespace bljOS::net;
using namespace bljOS::filesystem;

VideoGraphicsArray *vgap;

void printf(char* str){
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i){
        switch(str[i]){
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80){
            x = 0;
            y++;
        }

        if(y >= 25){
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color){
    vgap->putStr(str, x, y, color);
}

void printfHex(uint8_t key){
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

void printfDec(uint8_t value, int32_t x, int32_t y, uint32_t color){
    char buffer[4] = "000"; // A buffer to hold the decimal string representation (max value 255 requires 3 digits + null terminator)
    int index = 2;          // Start filling from the last digit

    do {
        buffer[index--] = '0' + (value % 10);  // Get the last digit and place it in the buffer
        value /= 10;                            // Remove the last digit from the value
    } while (value > 0);

    // Print the string, skipping any leading '0's
    vgap->putStr((uint8_t*)&buffer[index + 1], x, y, color);
}

void printfHex(uint8_t key, int32_t x, int32_t y, uint32_t color){
    char* foo = "00";
    char* hex = "0123456789ABCDEF";

    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    vgap->putStr((uint8_t*)foo, x, y, color);
}

void printfHex32(uint32_t key)
{
    printfHex((key >> 24) & 0xFF);
    printfHex((key >> 16) & 0xFF);
    printfHex((key >> 8) & 0xFF);
    printfHex( key & 0xFF);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler{
public:
    void onKeyDown(char c){
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MyRTCEventHandler : public RTCEventHandler {
public:
    void onTimeChange(const DateTime& time){
        // Handle time change
        printfHex(time.minute, 20, 20, 0xebdbb2);
    }
};

class MouseToConsole : public MouseEventHandler{
    int8_t x,y;
public:
    MouseToConsole(){
        static uint16_t* videoMemory = (uint16_t*)0xb8000;
        x=40;
        y=12;

        videoMemory[80*y+x] = ((videoMemory[80*y+x] & 0xF000) >> 4) | ((videoMemory[80*y+x] & 0x0F00) << 4) | ((videoMemory[80*y+x] & 0x00FF));
    }

    void onMouseMove(int xoffset, int yoffset){
        static uint16_t* videoMemory = (uint16_t*)0xb8000;
        videoMemory[80*y+x] = ((videoMemory[80*y+x] & 0xF000) >> 4) | ((videoMemory[80*y+x] & 0x0F00) << 4) | ((videoMemory[80*y+x] & 0x00FF));

        x += xoffset;

        if(x<0) x=0;
        if(x>=80) x=79;

        y += yoffset;

        if(y<0) y=0;
        if(y>=25) y=24;

        videoMemory[80*y+x] = ((videoMemory[80*y+x] & 0xF000) >> 4) | ((videoMemory[80*y+x] & 0x0F00) << 4) | ((videoMemory[80*y+x] & 0x00FF));
    }
};

class PrintfUDPHandler : public UserDatagramProtocolHandler{
public:
    void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, uint8_t* data, uint16_t size){
        char* foo = " ";
        for(int i = 0; i < size; i++){
            foo[0] = data[i];
            printf(foo);
        }
    }
};

class PrintfTCPHandler : public TransmissionControlProtocolHandler{
public:
    bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket* socket, uint8_t* data, uint16_t size){
        char* foo = " ";
        for(int i = 0; i < size; i++){
            foo[0] = data[i];
            printf(foo);
        }

        if(size > 9 && data[0] == 'G' && data[1] == 'E' && data[2] == 'T' && data[3] == ' ' && data[4] == '/' && data[5] == ' ' && data[6] == 'H' && data[7] == 'T' && data[8] == 'T' && data[9] == 'P'){
            socket->Send((uint8_t*)"HTTP/1.1 200 OK\r\nServer: MyOS\r\nContent-Type: text/html\r\n\r\n<html><head><title>My Operating System</title></head><body><b>My Operating System</b> http://www.AlgorithMan.de</body></html>\r\n",184);
            socket->Disconnect();
        }

        return true;
    }
};

void sysprintf(char* str){
    asm("int $0x80" : : "a" (4), "b" (str));
}

void taskA(){
    while(true)
        sysprintf("A");
}

void taskB(){
    while(true)
        sysprintf("B");
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors(){
    for(constructor* i=&start_ctors;i != &end_ctors;i++)
        (*i)();
}

extern "C" void kernelMain(void* multibootStructure, uint32_t magicNumber){
    /*testing printf*/
    printf("Hello world!\n");

    MultibootInfo* mbInfo = (MultibootInfo*)multibootStructure;
    GlobalDescriptorTable gdt;

    VideoGraphicsArray vga(mbInfo);
    Desktop desktop(1024, 768, 0x83a598);
    Toolbar toolbar(&desktop, 0, 735, 1024, 34, 0x3c3836);
    desktop.addChild(&toolbar);
    TimeButton timeButton(&toolbar, 980, 735, 42, 34, 0x3c3836, 0xebdbb2);
    toolbar.addChild(&timeButton);
    desktop.draw(&vga);
    vgap = &vga;

    uint32_t* memupper = (uint32_t*)((size_t)mbInfo->mem_upper);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("Multiboot flags: 0x");
    printfHex32(mbInfo->flags);
    printf("\n");

    /*testing memorymanager*/
    /*
    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8) & 0xFF);
    printfHex((heap) & 0xFF);
    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8) & 0xFF);
    printfHex(((size_t)allocated) & 0xFF);
    printf("\n");
    */
    TaskManager taskManager;
    /*testing multitasking*/
    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */

    InterruptManager interrupts(0x20, &gdt, &taskManager);



    SyscallHandler syscalls(&interrupts, 0x80);

    DriverManager drvManager;

    KeyboardDriver keyboard(&interrupts, &desktop);
    MouseDriver mouse(&interrupts, &desktop);
    RealTimeClock rtc(&interrupts, &timeButton);

    drvManager.AddDriver(&rtc);

//     PrintfKeyboardEventHandler kbhandler;
//     KeyboardDriver keyboard(&interrupts, &kbhandler);

    drvManager.AddDriver(&keyboard);


//     MouseToConsole mousehandler;
//     MouseDriver mouse(&interrupts, &mousehandler);

    drvManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

    drvManager.ActivateAll();

    MBR_PARTITION_TABLE((AdvancedHostControllerInterface*)(drvManager.drivers[4]));

    /*testing ata & fat32 */
    /*
    AdvancedTechnologyAttachment ata0m(0x1F0, true);
    ata0m.Identify();
    AdvancedTechnologyAttachment ata0s(0x1F0, false);
    ata0s.Identify();

    MSDOSPartitionTable::ReadPartitions(&ata0s);

    AdvancedTechnologyAttachment ata1m(0x170, true);
    AdvancedTechnologyAttachment ata1s(0x170, false);
    */

    uint8_t ip1 = 10, ip2 = 0, ip3 = 2, ip4 = 15;
    uint32_t ip_be = ((uint32_t)ip4 << 24) | ((uint32_t)ip3 << 16) | ((uint32_t)ip2 << 8) | ((uint32_t)ip1);

    uint8_t gip1 = 10, gip2 = 0, gip3 = 2, gip4 = 2;
    uint32_t gip_be = ((uint32_t)gip4 << 24) | ((uint32_t)gip3 << 16) | ((uint32_t)gip2 << 8) | ((uint32_t)gip1);

    uint8_t subnet1 = 255, subnet2 = 255, subnet3 = 255, subnet4 = 0;
    uint32_t subnet_be = ((uint32_t)subnet4 << 24) | ((uint32_t)subnet3 << 16) | ((uint32_t)subnet2 << 8) | ((uint32_t)subnet1);

    amd_am79c973* eth0 = (amd_am79c973*)(drvManager.drivers[3]);
    eth0->SetIPAddress(ip_be);

    EtherFrameProvider etherFrame(eth0);

    AddressResolutionProtocol arp(&etherFrame);
    InternetProtocolProvider ipv4(&etherFrame, &arp, gip_be, subnet_be);

    InternetControlMessageProtocol icmp(&ipv4);
    UserDatagramProtocolProvider udp(&ipv4);
    TransmissionControlProtocolProvider tcp(&ipv4);

    interrupts.activate();

    /*testing network stack*/
    /*
    printf("\n\n\n\n\n\n\n");
    arp.Resolve(gip_be);
    ipv4.Send(gip_be, 0x01, (uint8_t*)"foobar", 6);
    arp.BroadcastMACAddress(gip_be);

    PrintfTCPHandler tcphandler;
    TransmissionControlProtocolSocket* tcpsocket = tcp.Listen(1234);
    tcp.Bind(tcpsocket, &tcphandler);
    tcpsocket->Send((uint8_t*)"Hello TCP!", 10);
    icmp.RequestEchoReply(gip_be);

    PrintfUDPHandler udphandler;
    UserDatagramProtocolSocket* udpsocket = udp.Connect(gip_be, 1234);
    udp.Bind(udpsocket, &udphandler);
    udpsocket->Send((uint8_t*)"Hello UDP!", 10);

    UserDatagramProtocolSocket* udpsocket = udp.Listen(1234);
    udp.Bind(udpsocket, &udphandler);
    */

    while(1){
        if(desktop.getRedraw()){
            desktop.draw(&vga);
            desktop.setRedraw(false);
        }
        if(timeButton.getRedraw()){
            timeButton.draw(&vga);
            timeButton.setRedraw(false);
        }
    }
}
