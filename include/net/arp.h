#ifndef __BLJOS__NET__ARP_H
#define __BLJOS__NET__ARP_H

#include <common/types.h>
#include <net/etherframe.h>

namespace bljOS{
    namespace net{
        struct AddressResolutionProtocolMessage{
            bljOS::common::uint16_t hardwareType;
            bljOS::common::uint16_t protocol;
            bljOS::common::uint8_t hardwareAddressSize; // 6
            bljOS::common::uint8_t protocolAddressSize; // 4
            bljOS::common::uint16_t command;

            bljOS::common::uint64_t srcMAC : 48;
            bljOS::common::uint32_t srcIP;
            bljOS::common::uint64_t dstMAC : 48;
            bljOS::common::uint32_t dstIP;
        } __attribute__((packed));

        class AddressResolutionProtocol : public bljOS::net::EtherFrameHandler{
            bljOS::common::uint32_t IPcache[128];
            bljOS::common::uint32_t MACcache[128];
            int numCacheEntries;
        public:
            AddressResolutionProtocol(bljOS::net::EtherFrameProvider* backend);
            ~AddressResolutionProtocol();

            bool OnEtherFrameReceived(bljOS::common::uint8_t* etherFramePayload, bljOS::common::uint32_t size);
            void RequestMACAddress(bljOS::common::uint32_t IP_BE);
            bljOS::common::uint64_t GetMACFromCache(bljOS::common::uint32_t IP_BE);
            bljOS::common::uint64_t Resolve(bljOS::common::uint32_t IP_BE);
            void BroadcastMACAddress(bljOS::common::uint32_t IP_BE);
        };
    }
}

#endif
