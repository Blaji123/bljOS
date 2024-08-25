#ifndef __BLJOS__HARDWARECOMMUNICATON_PORT_H
#define __BLJOS__HARDWARECOMMUNICATON_PORT_H

#include <common/types.h>

namespace bljOS{
    namespace hardwarecommunication{

        class Port{
        protected:
            bljOS::common::uint16_t portNumber;

            Port(bljOS::common::uint16_t portNumber);
        };

        class Port8Bit : public Port{
        public:
            Port8Bit(bljOS::common::uint16_t portNumber);

            virtual void write(bljOS::common::uint8_t data);
            virtual bljOS::common::uint8_t read();
        };

        class Port8BitSlow : public Port8Bit{
        public:
            Port8BitSlow(bljOS::common::uint16_t portNumber);

            virtual void write(bljOS::common::uint8_t data);
        };

        class Port16Bit : public Port{
        public:
            Port16Bit(bljOS::common::uint16_t portNumber);

            virtual void write(bljOS::common::uint16_t data);
            virtual bljOS::common::uint16_t read();
        };

        class Port32Bit : public Port{
        public:
            Port32Bit(bljOS::common::uint16_t portNumber);

            virtual void write(bljOS::common::uint32_t data);
            virtual bljOS::common::uint32_t read();
        };
    }
}
#endif
