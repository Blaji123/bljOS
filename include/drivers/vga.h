#ifndef __BLJOS__DRIVERS__VGA_H
#define __BLJOS__DRIVERS__VGA_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace bljOS{
    namespace drivers{
        class VideoGraphicsArray{
            bljOS::hardwarecommunication::Port8Bit miscPort;
            bljOS::hardwarecommunication::Port8Bit crtcIndexPort;
            bljOS::hardwarecommunication::Port8Bit crtcDataPort;
            bljOS::hardwarecommunication::Port8Bit sequencerIndexPort;
            bljOS::hardwarecommunication::Port8Bit sequencerDataPort;
            bljOS::hardwarecommunication::Port8Bit graphicsControllerIndexPort;
            bljOS::hardwarecommunication::Port8Bit graphicsControllerDataPort;
            bljOS::hardwarecommunication::Port8Bit attributeControllerIndexPort;
            bljOS::hardwarecommunication::Port8Bit attributeControllerReadPort;
            bljOS::hardwarecommunication::Port8Bit attributeControllerWritePort;
            bljOS::hardwarecommunication::Port8Bit attributeControllerResetPort;

            void WriteRegisters(bljOS::common::uint8_t* registers);
            bljOS::common::uint8_t* GetFrameBufferSegment();

            virtual bljOS::common::uint8_t GetColorIndex(bljOS::common::uint8_t r, bljOS::common::uint8_t g, bljOS::common::uint8_t b);

        public:
            VideoGraphicsArray();
            ~VideoGraphicsArray();

            virtual bool SetMode(bljOS::common::uint32_t width, bljOS::common::uint32_t height, bljOS::common::uint32_t colordepth);
            virtual bool SupportsMode(bljOS::common::uint32_t width, bljOS::common::uint32_t height, bljOS::common::uint32_t colordepth);
            virtual void PutPixel(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t r, bljOS::common::uint8_t g, bljOS::common::uint8_t b);
            virtual void PutPixel(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t colorIndex);

            virtual void FillRectangle(bljOS::common::uint32_t x, bljOS::common::uint32_t y, bljOS::common::uint32_t w, bljOS::common::uint32_t h, bljOS::common::uint8_t r, bljOS::common::uint8_t g, bljOS::common::uint8_t b);

        };

    }
}

#endif
