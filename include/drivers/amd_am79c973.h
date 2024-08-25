#ifndef __BLJOS__DRIVERS__AMD_AM79C973_H
#define __BLJOS__DRIVERS__AMD_AM79C973_H

#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/pci.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>

namespace bljOS{
    namespace drivers{
        class amd_am79c973;

        class RawDataHandler{
        protected:
            amd_am79c973* backend;
        public:
            RawDataHandler(amd_am79c973* backend);
            ~RawDataHandler();

            virtual bool OnRawDataReceived(bljOS::common::uint8_t* buffer, bljOS::common::uint32_t size);
            void Send(bljOS::common::uint8_t* buffer, bljOS::common::uint32_t size);
        };

        class amd_am79c973 : public bljOS::drivers::Driver, public bljOS::hardwarecommunication::InterruptHandler{

            struct InitializationBlock{
                bljOS::common::uint16_t mode;
                unsigned reserved1 : 4;
                unsigned numSendBuffers : 4;
                unsigned reserved2 : 4;
                unsigned numRecvBuffers : 4;
                bljOS::common::uint64_t physicalAddress : 48;
                bljOS::common::uint16_t reserved3;
                bljOS::common::uint64_t logicalAddress;
                bljOS::common::uint32_t recvBufferDescrAddress;
                bljOS::common::uint32_t sendBufferDescrAddress;
            } __attribute__((packed));

            struct BufferDescriptor{
                bljOS::common::uint32_t address;
                bljOS::common::uint32_t flags;
                bljOS::common::uint32_t flags2;
                bljOS::common::uint32_t avail;
            } __attribute__((packed));

            bljOS::hardwarecommunication::Port16Bit MACAddress0Port;
            bljOS::hardwarecommunication::Port16Bit MACAddress2Port;
            bljOS::hardwarecommunication::Port16Bit MACAddress4Port;
            bljOS::hardwarecommunication::Port16Bit registerDataPort;
            bljOS::hardwarecommunication::Port16Bit registerAddressPort;
            bljOS::hardwarecommunication::Port16Bit resetPort;
            bljOS::hardwarecommunication::Port16Bit busControlRegisterDataPort;

            InitializationBlock initBlock;

            BufferDescriptor* sendBufferDescr;
            bljOS::common::uint8_t sendBufferDescrMemory[2048+15];
            bljOS::common::uint8_t sendBuffers[2*1024+15][8];
            bljOS::common::uint8_t currentSendBuffer;

            BufferDescriptor* recvBufferDescr;
            bljOS::common::uint8_t recvBufferDescrMemory[2048+15];
            bljOS::common::uint8_t recvBuffers[2*1024+15][8];
            bljOS::common::uint8_t currentRecvBuffer;

            RawDataHandler* handler;
        public:
            amd_am79c973(bljOS::hardwarecommunication::PeripheralComponentInterconnectDeviceDescriptor *dev,bljOS::hardwarecommunication::InterruptManager* interrupts);
            ~amd_am79c973();

            void Activate();
            int Reset();
            bljOS::common::uint32_t handleInterrupt(bljOS::common::uint32_t esp);

            void Send(bljOS::common::uint8_t* buffer, int size);
            void Receive();
            void SetHandler(RawDataHandler* handler);
            bljOS::common::uint64_t GetMACAddress();
            bljOS::common::uint32_t GetIPAddress();
            void SetIPAddress(bljOS::common::uint32_t IP);
        };
    }
}

#endif
