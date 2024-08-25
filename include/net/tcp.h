#ifndef __BLJOS__NET__TCP_H
#define __BLJOS__NET__TCP_H

#include <common/types.h>
#include <net/ipv4.h>
#include <memorymanagement.h>

namespace bljOS{
    namespace net{
       enum TransmissionControlProtocolSocketState{
            CLOSED,
            LISTENING,
            SYN_SENT,
            SYN_RECEIVED,
            ESTABLISHED,
            FIN_WAIT1,
            FIN_WAIT2,
            CLOSING,
            TIME_WAIT,
            CLOSE_WAIT,
            //LAST_ACK,
       };
       enum TransmissionControlProtocolFlag{
            FIN = 1,
            SYN = 2,
            RST = 4,
            PSH = 8,
            ACK = 16,
            URG = 32,
            ECE = 64,
            CWR = 128,
            NS = 256,
       };

       struct TransmissionControlProtocolHeader{
            bljOS::common::uint16_t srcPort;
            bljOS::common::uint16_t dstPort;
            bljOS::common::uint32_t sequenceNumber;
            bljOS::common::uint32_t acknowledgementNumber;
            bljOS::common::uint8_t reserved : 4;
            bljOS::common::uint8_t headerSize32 : 4;
            bljOS::common::uint8_t flags;
            bljOS::common::uint16_t windowSize;
            bljOS::common::uint16_t checksum;
            bljOS::common::uint16_t urgentPtr;
            bljOS::common::uint32_t options;
        }__attribute__((packed));

        struct TransmissionControlProtocolPseudoHeader{
            bljOS::common::uint32_t srcIP;
            bljOS::common::uint32_t dstIP;
            bljOS::common::uint16_t protocol;
            bljOS::common::uint16_t totalLength;
        }__attribute__((packed));

        class TransmissionControlProtocolSocket;
        class TransmissionControlProtocolProvider;

        class TransmissionControlProtocolHandler{
        public:
            TransmissionControlProtocolHandler();
            ~TransmissionControlProtocolHandler();
            virtual bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket* socket, bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
        };

        class TransmissionControlProtocolSocket{
            friend class TransmissionControlProtocolProvider;
        protected:
            bljOS::common::uint16_t remotePort;
            bljOS::common::uint32_t remoteIP;
            bljOS::common::uint16_t localPort;
            bljOS::common::uint32_t localIP;
            bljOS::common::uint32_t sequenceNumber;
            bljOS::common::uint32_t acknowledgementNumber;
            TransmissionControlProtocolProvider* backend;
            TransmissionControlProtocolHandler* handler;
            TransmissionControlProtocolSocketState state;
        public:
            TransmissionControlProtocolSocket(TransmissionControlProtocolProvider* backend);
            ~TransmissionControlProtocolSocket();
            virtual bool HandleTransmissionControlProtocolMessage(bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
            virtual void Send(bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
            virtual void Disconnect();
        };

        class TransmissionControlProtocolProvider : public bljOS::net::InternetProtocolHandler{
            TransmissionControlProtocolSocket* sockets[65535];
            bljOS::common::uint16_t numSockets;
            bljOS::common::uint16_t freePort;
        public:
            TransmissionControlProtocolProvider(InternetProtocolProvider* backend);
            ~TransmissionControlProtocolProvider();
            virtual bool OnInternetProtocolReceived(bljOS::common::uint32_t srcIP_BE, bljOS::common::uint32_t dstIP_BE, bljOS::common::uint8_t* internetProtocolPayload, bljOS::common::uint32_t size);
            virtual void Disconnect(TransmissionControlProtocolSocket* socket);
            virtual TransmissionControlProtocolSocket* Connect(bljOS::common::uint32_t ip, bljOS::common::uint16_t port);
            virtual TransmissionControlProtocolSocket* Listen(bljOS::common::uint16_t port);
            virtual void Send(TransmissionControlProtocolSocket* socket, bljOS::common::uint8_t* data, bljOS::common::uint16_t size, bljOS::common::uint16_t flags = 0);
            virtual void Bind(TransmissionControlProtocolSocket* socket, TransmissionControlProtocolHandler* handler);
        };
    }
}

#endif
