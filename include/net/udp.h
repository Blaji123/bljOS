#ifndef __BLJOS__NET__UDP_H
#define __BLJOS__NET__UDP_H

#include <common/types.h>
#include <net/ipv4.h>
#include <memorymanagement.h>

namespace bljOS{
    namespace net{
        struct UserDatagramProtocolHeader{
            bljOS::common::uint16_t srcPort;
            bljOS::common::uint16_t dstPort;
            bljOS::common::uint16_t length;
            bljOS::common::uint16_t checksum;
        }__attribute__((packed));

        class UserDatagramProtocolSocket;
        class UserDatagramProtocolProvider;

        class UserDatagramProtocolHandler{
        public:
            UserDatagramProtocolHandler();
            ~UserDatagramProtocolHandler();
            virtual void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket* socket, bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
        };

        class UserDatagramProtocolSocket{
            friend class UserDatagramProtocolProvider;
        protected:
            bljOS::common::uint16_t remotePort;
            bljOS::common::uint32_t remoteIP;
            bljOS::common::uint16_t localPort;
            bljOS::common::uint32_t localIP;
            bool listening;
            UserDatagramProtocolProvider* backend;
            UserDatagramProtocolHandler* handler;
        public:
            UserDatagramProtocolSocket(UserDatagramProtocolProvider* backend);
            ~UserDatagramProtocolSocket();
            virtual void HandleUserDatagramProtocolMessage(bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
            virtual void Send(bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
            virtual void Disconnect();
        };

        class UserDatagramProtocolProvider : public bljOS::net::InternetProtocolHandler{
            UserDatagramProtocolSocket* sockets[65535];
            bljOS::common::uint16_t numSockets;
            bljOS::common::uint16_t freePort;
        public:
            UserDatagramProtocolProvider(InternetProtocolProvider* backend);
            ~UserDatagramProtocolProvider();
            virtual bool OnInternetProtocolReceived(bljOS::common::uint32_t srcIP_BE, bljOS::common::uint32_t dstIP_BE, bljOS::common::uint8_t* internetProtocolPayload, bljOS::common::uint32_t size);
            virtual void Disconnect(UserDatagramProtocolSocket* socket);
            virtual UserDatagramProtocolSocket* Connect(bljOS::common::uint32_t ip, bljOS::common::uint16_t port);
            virtual UserDatagramProtocolSocket* Listen(bljOS::common::uint16_t port);
            virtual void Send(UserDatagramProtocolSocket* socket, bljOS::common::uint8_t* data, bljOS::common::uint16_t size);
            virtual void Bind(UserDatagramProtocolSocket* socket, UserDatagramProtocolHandler* handler);
        };
    }
}

#endif
