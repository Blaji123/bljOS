#include <datastructures/bitmap.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::datastructures;

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);

void Bitmap::drawBMP(uint8_t* bmp_data, GraphicsContext* gc, uint32_t xpos, uint32_t ypos){
    BitmapFileHeader* file_header = (BitmapFileHeader*)bmp_data;
    BitmapInfoHeader* info_header = (BitmapInfoHeader*)(bmp_data + sizeof(BitmapFileHeader));

    if(file_header->file_type != 0x4D42){
        printf((uint8_t*)"error", 20, 20, 0xebdbb2);
        return;
    }

    uint8_t* pixel_data = bmp_data + file_header->pixel_data_offset;

    int width = info_header->width;
    int height = info_header->height;
    int bytes_per_pixel = info_header->bits_per_pixel / 8;

    for(int y=0;y<height;y++)
        for(int x=0;x<width;x++){
            int bmp_index = (x + (height - y - 1) * width) * bytes_per_pixel;

            uint8_t blue = pixel_data[bmp_index];
            uint8_t green = pixel_data[bmp_index + 1];
            uint8_t red = pixel_data[bmp_index + 2];

            uint8_t alpha = (bytes_per_pixel == 4) ? pixel_data[bmp_index + 3] : 0xFF;

            uint32_t color = (alpha << 24) | (red << 16) | (green << 8) | blue;

            gc->putPixel(x + xpos, y + ypos, color);
        }
}
