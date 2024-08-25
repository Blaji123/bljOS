#ifndef __BLJOS__NET__IPV4_H
#define __BLJOS__NET__IPV4_H

#include <common/types.h>
#include <net/etherframe.h>
#include <net/arp.h>

namespace bljOS{
    namespace net{
        struct InternetProtocolV4Message{
            bljOS::common::uint8_t headerLength : 4;
            bljOS::common::uint8_t version : 4;
            bljOS::common::uint8_t tos;
            bljOS::common::uint16_t totalLength;
            bljOS::common::uint16_t ident;
            bljOS::common::uint16_t flagsAndOffset;
            bljOS::common::uint8_t timeToLive;
            bljOS::common::uint8_t protocol;
            bljOS::common::uint16_t checksum;
            bljOS::common::uint32_t srcIP;
            bljOS::common::uint32_t dstIP;
        }__attribute__((packed));

        class InternetProtocolProvider;

        class InternetProtocolHandler{
        protected:
            InternetProtocolProvider* backend;
            bljOS::common::uint8_t ip_protocol;
        public:
            InternetProtocolHandler(InternetProtocolProvider* backend,bljOS::common::uint8_t protocol);
            ~InternetProtocolHandler();

            virtual bool OnInternetProtocolReceived(bljOS::common::uint32_t srcIP_BE, bljOS::common::uint32_t dstIP_BE, bljOS::common::uint8_t* internetProtocolPayload, bljOS::common::uint32_t size);
            void Send(bljOS::common::uint32_t dstIP_BE, bljOS::common::uint8_t* internetProtocolPayload, bljOS::common::uint32_t size);
        };

        class InternetProtocolProvider : public bljOS::net::EtherFrameHandler{
            friend class InternetProtocolHandler;
        protected:
            InternetProtocolHandler* handlers[255];
            bljOS::net::AddressResolutionProtocol* arp;
            bljOS::common::uint32_t gatewayIP;
            bljOS::common::uint32_t subnetMask;
        public:
            InternetProtocolProvider(bljOS::net::EtherFrameProvider* backend, bljOS::net::AddressResolutionProtocol* arp, bljOS::common::uint32_t gatewayIP, bljOS::common::uint32_t subnetMask);
            ~InternetProtocolProvider();

            bool OnEtherFrameReceived(bljOS::common::uint8_t* etherFramePayload, bljOS::common::uint32_t size);
            void Send(bljOS::common::uint32_t dstIP_BE, bljOS::common::uint8_t protocol, bljOS::common::uint8_t* data, bljOS::common::uint32_t size);
            static bljOS::common::uint16_t Checksum(bljOS::common::uint16_t* data, bljOS::common::uint32_t lengthInBytes);
        };
    }
}

#endif
