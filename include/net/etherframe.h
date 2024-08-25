#ifndef __BLJOS__NET__ETHERFRAME_H
#define __BLJOS__NET__ETHERFRAME_H

#include <common/types.h>
#include <drivers/amd_am79c973.h>
#include <memorymanagement.h>

namespace bljOS{
    namespace net{
        struct EtherFrameHeader{
            bljOS::common::uint64_t dstMAC_BE : 48;
            bljOS::common::uint64_t srcMAC_BE : 48;
            bljOS::common::uint16_t etherType_BE;
        }__attribute__((packed));

        typedef bljOS::common::uint32_t EtherFrameFooter;

        class EtherFrameProvider;

        class EtherFrameHandler{
        protected:
            EtherFrameProvider* backend;
            bljOS::common::uint16_t etherType_BE;
        public:
            EtherFrameHandler(EtherFrameProvider* backend,bljOS::common::uint16_t etherType);
            ~EtherFrameHandler();

            virtual bool OnEtherFrameReceived(bljOS::common::uint8_t* etherFramePayload, bljOS::common::uint32_t size);
            void Send(bljOS::common::uint64_t dstMAC_BE, bljOS::common::uint8_t* data, bljOS::common::uint32_t size);
            bljOS::common::uint32_t GetIPAddress();

        };

        class EtherFrameProvider : public bljOS::drivers::RawDataHandler{
            friend class EtherFrameHandler;
        protected:
            EtherFrameHandler* handlers[65535];
        public:
            EtherFrameProvider(bljOS::drivers::amd_am79c973* backend);
            ~EtherFrameProvider();

            bool OnRawDataReceived(bljOS::common::uint8_t* buffer, bljOS::common::uint32_t size);
            void Send(bljOS::common::uint64_t dstMAC_BE, bljOS::common::uint16_t etherType_BE, bljOS::common::uint8_t* buffer, bljOS::common::uint32_t size);

            bljOS::common::uint32_t GetIPAddress();
            bljOS::common::uint64_t GetMACAddress();
        };
    }
}

#endif
