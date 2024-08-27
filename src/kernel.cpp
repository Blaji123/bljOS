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

// #define GRAPHICSMODE

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::hardwarecommunication;
using namespace bljOS::gui;
using namespace bljOS::net;
using namespace bljOS::filesystem;

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

void printfHex(uint8_t key){
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
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
    void OnKeyDown(char c){
        char* foo = " ";
        foo[0] = c;
        printf(foo);
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

    void OnMouseMove(int xoffset, int yoffset){
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

    uint32_t* memupper = (uint32_t*)((size_t)mbInfo->mem_upper);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);

    printf("Multiboot flags: 0x");
    printfHex32(mbInfo->flags);
    printf("\n");
    if(mbInfo->flags & (1<<12)){
        VideoGraphicsArray vga(mbInfo);
        vga.putStr((uint8_t*)"hello world!", 2, 2, 0xd79921);
    }

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

#ifdef GRAPHICSMODE
    KeyboardDriver keyboard(&interrupts, &desktop);
#else
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
#endif
    drvManager.AddDriver(&keyboard);

#ifdef GRAPHICSMODE
    MouseDriver mouse(&interrupts, &desktop);
#else
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
#endif
    drvManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrivers(&drvManager, &interrupts);

    drvManager.ActivateAll();

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

    amd_am79c973* eth0 = (amd_am79c973*)(drvManager.drivers[2]);
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
    }
}
