#ifndef __BLJOS__HARDWARECOMMUNICATION__PCI_H
#define __BLJOS__HARDWARECOMMUNICATION__PCI_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <hardwarecommunication/interrupts.h>
#include <memorymanagement.h>

namespace bljOS{
    namespace hardwarecommunication{

        enum BaseAddressRegisterType{
            MemoryMapping = 0,
            InputOuput = 1
        };

        class BaseAddressRegister{
        public:
            bool prefetchable;
            bljOS::common::uint8_t* address;
            bljOS::common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor{
        public:
            bljOS::common::uint32_t portBase;
            bljOS::common::uint32_t interrupt;

            bljOS::common::uint16_t bus;
            bljOS::common::uint16_t device;
            bljOS::common::uint16_t function;

            bljOS::common::uint16_t vendor_id;
            bljOS::common::uint16_t device_id;

            bljOS::common::uint8_t class_id;
            bljOS::common::uint8_t subclass_id;
            bljOS::common::uint8_t interface_id;

            bljOS::common::uint8_t revision;

            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();

        };

        class PeripheralComponentInterconnectController{
            Port32Bit dataPort;
            Port32Bit commandPort;

        public:
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            bljOS::common::uint32_t Read(bljOS::common::uint16_t bus, bljOS::common::uint16_t device, bljOS::common::uint16_t function, bljOS::common::uint32_t registeroffset);
            void Write(bljOS::common::uint16_t bus, bljOS::common::uint16_t device, bljOS::common::uint16_t function, bljOS::common::uint32_t registeroffset, bljOS::common::uint32_t value);
            bool DeviceHasFunctions(bljOS::common::uint16_t bus, bljOS::common::uint16_t device);
            void SelectDrivers(bljOS::drivers::DriverManager* driverManager, bljOS::hardwarecommunication::InterruptManager* interrupts);
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(bljOS::common::uint16_t bus, bljOS::common::uint16_t device, bljOS::common::uint16_t function);
            bljOS::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, bljOS::hardwarecommunication::InterruptManager* interrupts);
            BaseAddressRegister GetBaseAddressRegister(bljOS::common::uint16_t bus, bljOS::common::uint16_t device, bljOS::common::uint16_t function, bljOS::common::uint16_t bar);
        };
    }
}
#endif
