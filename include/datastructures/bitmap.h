#ifndef __BLJOS__DATASTRUCTURES__BITMAP_H
#define __BLJOS__DATASTRUCTURES__BITMAP_H

#include <common/types.h>
#include <common/graphicscontext.h>

namespace bljOS{
    namespace datastructures{

        struct BitmapFileHeader{
            bljOS::common::uint16_t file_type;
            bljOS::common::uint32_t file_size;
            bljOS::common::uint16_t reserved1;
            bljOS::common::uint16_t reserved2;
            bljOS::common::uint32_t pixel_data_offset;
        }__attribute__((packed));

        struct BitmapInfoHeader{
            bljOS::common::uint32_t header_size;
            bljOS::common::int32_t width;
            bljOS::common::int32_t height;
            bljOS::common::uint16_t planes;
            bljOS::common::uint16_t bits_per_pixel;
            bljOS::common::uint32_t compression;
            bljOS::common::uint32_t image_size;
            bljOS::common::int32_t x_pixels_per_meter;
            bljOS::common::int32_t y_pixels_per_meter;
            bljOS::common::uint32_t colors_used;
            bljOS::common::uint32_t important_colors;
        }__attribute__((packed));

        class Bitmap{
        public:
            Bitmap(){}

            void drawBMP(bljOS::common::uint8_t* bmp_data, bljOS::common::GraphicsContext* gc, bljOS::common::uint32_t xpos, bljOS::common::uint32_t ypos);
        };
    }
}

#endif
