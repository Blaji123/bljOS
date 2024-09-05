#include <hardwarecommunication/pci.h>
#include <drivers/amd_am79c973.h>
#include <drivers/ahci.h>
using namespace bljOS;
using namespace bljOS::drivers;
using namespace bljOS::common;
using namespace bljOS::hardwarecommunication;

void printf(char*);
void printfHex(uint8_t);
void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);
void printfHex(uint8_t key, int32_t x, int32_t y, uint32_t color);

int i = 0;

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor(){}

PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor(){}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController():dataPort(0xCFC),commandPort(0xCF8){}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController(){}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset){
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device  & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);

    commandPort.write(id);
    uint32_t result = dataPort.read();
    return result >>  (8* (registeroffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value){
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device  & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    commandPort.write(id);
    dataPort.write(value);
}

bool PeripheralComponentInterconnectController::DeviceHasFunctions(uint16_t bus, uint16_t device){
    return Read(bus, device, 0, 0x0E) & (1<<7);
}

void PeripheralComponentInterconnectController::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts){
    for(int bus=0;bus<8;bus++){
        for(int device=0;device<32;device++){
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function=0;function<numFunctions;function++){
                PeripheralComponentInterconnectDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;

                for(int barNum=0;barNum<6;barNum++){
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOuput))
                        dev.portBase = (uint32_t)bar.address;
                }

                Driver* driver = GetDriver(dev, interrupts);
                if(driver!=0){
                    i++;
                    driverManager->AddDriver(driver);
                }

                printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(function & 0xFF);

                printf(" = VENDOR ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);
                printf(", DEVICE ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar){
    BaseAddressRegister result;
    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;
    uint32_t bar_value = Read(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x01) ? InputOuput : MemoryMapping;
    uint32_t temp;
    if(result.type == MemoryMapping){
        switch((bar_value >> 1) & 0x3){
            case 0x0:
                result.address = (uint8_t*)(bar_value & ~0xF);
                break;
            case 0x1:
                break;
            case 0x2:
                temp = Read(bus, device, function, 0x10 + 4 * (bar + 1));
                result.address = (uint8_t*)(((uint64_t)temp << 32) | (bar_value & ~0xF));
                break;
            default:
                return result;
        }
        Write(bus, device, function, 0x10 + 4 * bar, 0xFFFFFFFF);
        uint32_t size_mask = Read(bus, device, function, 0x10 + 4*bar);
        Write(bus, device, function, 0x10 + 4 * bar, bar_value);
        result.size = ~(size_mask & ~0xF) + 1;
        result.prefetchable = (bar_value & 0x8) != 0;
    }else{
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    return result;
}

Driver* PeripheralComponentInterconnectController::GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, InterruptManager* interrupts){
    Driver* driver = 0;
    switch(dev.vendor_id){
        case 0x1022: //AMD
            switch(dev.device_id){
                case 0x2000: //am79c973
                    driver = (amd_am79c973*)MemoryManager::activeMemoryManager->malloc(sizeof(amd_am79c973));
                    if(driver!=0)
                        new (driver)amd_am79c973(&dev, interrupts);
                    return driver;
                    break;
            }
            break;
        case 0x8086: //Intel
            break;
    }
    switch(dev.class_id){
        case 0x01:
            switch(dev.subclass_id){
                case 0x06:
                    driver = (AdvancedHostControllerInterface*)MemoryManager::activeMemoryManager->malloc(sizeof(AdvancedHostControllerInterface));
                    if(driver!=0)
                        new (driver)AdvancedHostControllerInterface(&dev);
                    return driver;
                    break;
            }
            break;

        case 0x03: // graphics
            switch(dev.subclass_id){
                case 0x00: // VGA
                    break;
            }
            break;
    }
    return driver;
}


PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function){
    PeripheralComponentInterconnectDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0b);
    result.subclass_id = Read(bus, device, function, 0x0a);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3c);

    result.command = Read(bus, device, function, 0x04);

    // Reading the Base Address Registers (BARs)
    result.BAR0 = Read(bus, device, function, 0x10);
    result.BAR1 = Read(bus, device, function, 0x14);
    result.BAR2 = Read(bus, device, function, 0x18);
    result.BAR3 = Read(bus, device, function, 0x1C);
    result.BAR4 = Read(bus, device, function, 0x20);
    result.BAR5 = Read(bus, device, function, 0x24);  //

    return result;
}
