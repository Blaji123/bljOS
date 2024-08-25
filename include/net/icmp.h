#ifndef __BLJOS__NET__ICMP_H
#define __BLJOS__NET__ICMP_H

#include <common/types.h>
#include <net/ipv4.h>

namespace bljOS{
    namespace net{
        struct InternetControlMessageProtocolMessage{
            bljOS::common::uint8_t type;
            bljOS::common::uint8_t code;
            bljOS::common::uint16_t checksum;
            bljOS::common::uint32_t data;
        }__attribute__((packed));

        class InternetControlMessageProtocol : public bljOS::net::InternetProtocolHandler{
        public:
            InternetControlMessageProtocol(bljOS::net::InternetProtocolProvider* backend);
            ~InternetControlMessageProtocol();

            bool OnInternetProtocolReceived(bljOS::common::uint32_t srcIP_BE, bljOS::common::uint32_t dstIP_BE, bljOS::common::uint8_t* internetProtocolPayload, bljOS::common::uint32_t size);
            void RequestEchoReply(bljOS::common::uint32_t ip_be);
        };
    }
}

#endif
