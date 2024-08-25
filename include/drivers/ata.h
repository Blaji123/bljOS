#ifndef __BLJOS__DRIVERS__ATA_H
#define __BLJOS__DRIVERS__ATA_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>

namespace bljOS{
    namespace drivers{
        class AdvancedTechnologyAttachment{
        protected:
            bljOS::hardwarecommunication::Port16Bit dataPort;
            bljOS::hardwarecommunication::Port8Bit errorPort;
            bljOS::hardwarecommunication::Port8Bit sectorCountPort;
            bljOS::hardwarecommunication::Port8Bit lbaLowPort;
            bljOS::hardwarecommunication::Port8Bit lbaMidPort;
            bljOS::hardwarecommunication::Port8Bit lbaHiPort;
            bljOS::hardwarecommunication::Port8Bit devicePort;
            bljOS::hardwarecommunication::Port8Bit commandPort;
            bljOS::hardwarecommunication::Port8Bit controlPort;
            bool master;
            bljOS::common::uint16_t bytesPerSector;
        public:
            AdvancedTechnologyAttachment(bljOS::common::uint16_t portBase, bool master);
            ~AdvancedTechnologyAttachment();

            void Identify();
            void Read28(bljOS::common::uint32_t sector, bljOS::common::uint8_t* data, int count);
            void Write28(bljOS::common::uint32_t sector, bljOS::common::uint8_t* data, int count);
            void Flush();
        };
    }
}

#endif
