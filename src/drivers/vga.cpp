#include <drivers/vga.h>

using namespace bljOS;
using namespace bljOS::drivers;
using namespace bljOS::common;
using namespace bljOS::hardwarecommunication;

VideoGraphicsArray::VideoGraphicsArray(MultibootInfo* multiboot){
    width = multiboot->framebuffer_width;
    height = multiboot->framebuffer_height;
    screen = (uint32_t*)multiboot->framebuffer_addr;
}

void VideoGraphicsArray::putPixel(int32_t x, int32_t y, uint32_t color){
    if(x < 0 || width <= x || y < 0 || height <= y)
        return;

    screen[width * y + x] = color;
}

void VideoGraphicsArray::putChar(uint8_t ch, int32_t x, int32_t y, uint32_t color){
    uint32_t px = 0;
    uint64_t bCh = FONT[ch];

    if(x + 8 < 0 || x > width || y + 8 < 0 || y > height)
        return;

    if(x >= 0 && x + 8 < width && y >= 0 && y + 8 < height){
        int i = width*(y - 1) + x + 8;
        int incAmount = width - 8;
        for(int y2=7;y2>=0;y2--){
            i += incAmount;
            for(int x2=7;x2>=0;x2--){
                if((bCh >> px++) & 1)
                    screen[i] = color;
                i++;
            }
        }
    }else{
        int xpos = 0;
        int i = width*(y - 1);
        for(int y2=0;y2<8;y2++){
            i += width;
            xpos = x;
            for(int x2=7;x2>=0;x2--){
                if ((bCh >> px++) & 1)
                    if(xpos > 0 && xpos < width && y2 + y > 0 && y2 + y < height)
                        screen[i + xpos] = color;
                xpos++;
            }
        }
    }
}

void VideoGraphicsArray::putStr(uint8_t* ch, int32_t x, int32_t y, uint32_t colorIndex){
    for(int32_t i = 0; ch[i] != 0; i++, x+=8)
        putChar(ch[i], x, y, colorIndex);
}

uint32_t abs(int32_t x){
    if(x < 0)
        return -x;
    return x;
}

int64_t round(float num)
{
    return num < 0 ? num - 0.5 : num + 0.5;
}

void VideoGraphicsArray::drawLineLow(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color){
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int32_t yi = 1;

    if(dy < 0){
        yi = -1;
        dy = -dy;
    }

    int32_t D = 2*dy - dx;
    int32_t y = y0;

    for(int32_t x=x0;x<x1;x++){
        putPixel(x,y,color);

        if(D>0){
            y += yi;
            D = D + (2*(dy - dx));
        }else
            D = D + 2*dy;
    }
}

void VideoGraphicsArray::drawLineHigh(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color){
    int32_t dx = x1 - x0;
    int32_t dy = y1 - y0;
    int32_t xi = 1;

    if(dx < 0){
        xi = -1;
        dx = -dx;
    }

    int32_t D = 2*dx - dy;
    int32_t x = x0;

    for(int32_t y=y0;y<y1;y++){
        putPixel(x,y,color);

        if(D>0){
            x += xi;
            D = D + (2*(dx - dy));
        }else
            D = D + 2*dx;
    }
}

void VideoGraphicsArray::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color){
    if(abs(y1 - y0) < abs(x1 - x0))
        if(x0 > x1)
            drawLineLow(x1, y1, x0, y0,color);
        else
            drawLineLow(x0,y0,x1,y1,color);
    else
        if(y0 > y1)
            drawLineHigh(x1,y1,x0,y0,color);
        else
            drawLineHigh(x0,y0,x1,y1,color);
}

void VideoGraphicsArray::drawRectangle(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t color){
    drawLine(x, y, x + w, y, color);
    drawLine(x, y + h, x + w, y + h, color);
    drawLine(x, y, x, y + h, color);
    drawLine(x + w, y, x + w, y + h, color);
}

void VideoGraphicsArray::drawCircle(int32_t centerX, int32_t centerY, int32_t radius, uint32_t color){
    int32_t x = radius;
    int32_t y = 0;
    int32_t decisionOver2 = 1 - x;

    while(x >= y){
        putPixel(centerX + x, centerY + y, color);
        putPixel(centerX + y, centerY + x, color);
        putPixel(centerX - y, centerY + x, color);
        putPixel(centerX - x, centerY + y, color);
        putPixel(centerX - x, centerY - y, color);
        putPixel(centerX - y, centerY - x, color);
        putPixel(centerX + y, centerY - x, color);
        putPixel(centerX + x, centerY - y, color);

        y++;
        if(decisionOver2 <= 0)
            decisionOver2 += 2*y+1;
        else{
            x--;
            decisionOver2 += 2*(y-x)+1;
        }
    }
}

void VideoGraphicsArray::drawEllipse(int32_t x0, int32_t y0, int32_t rx, int32_t ry, uint32_t color){
    int32_t x,y;
    int64_t rx2 = (int64_t)rx * rx;
    int64_t ry2 = (int64_t)ry * ry;
    int64_t tworx2 = 2 * rx2;
    int64_t twory2 = 2 * ry2;
    int64_t p;
    int64_t px = 0;
    int64_t py = tworx2 * ry;

    x = 0;
    y = ry;
    putPixel(x0 + x, y0 + y, color);
    putPixel(x0 - x, y0 + y, color);
    putPixel(x0 + x, y0 - y, color);
    putPixel(x0 - x, y0 - y, color);
    p=round(ry2 - (rx2*ry) + (0.25*rx2));

    while(px < py){
        x++;
        px += twory2;
        if(p < 0){
            p += ry2 + px;
        }else{
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
        putPixel(x0 + x, y0 + y, color);
        putPixel(x0 - x, y0 + y, color);
        putPixel(x0 + x, y0 - y, color);
        putPixel(x0 - x, y0 - y, color);
    }

    p=round(ry2*(x + 0.5)*(x + 0.5) + rx2*(y - 1)*(y - 1) - rx2*ry2);
    while(y > 0){
        y--;
        py -= tworx2;
        if(p > 0){
            p += rx2 - py;
        }else{
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
        putPixel(x0 + x, y0 + y, color);
        putPixel(x0 - x, y0 + y, color);
        putPixel(x0 + x, y0 - y, color);
        putPixel(x0 - x, y0 - y, color);
    }
}

void VideoGraphicsArray::drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color){
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void VideoGraphicsArray::fillCircle(int32_t centerX, int32_t centerY, int32_t radius, uint32_t color){
    int32_t x = radius;
    int32_t y = 0;
    int32_t decisionOver2 = 1 - x;

    while(x >= y){
        drawLine(centerX - x, centerY + y, centerX + x, centerY + y, color);
        drawLine(centerX - y, centerY + x, centerX + y, centerY + x, color);
        drawLine(centerX - x, centerY - y, centerX + x, centerY - y, color);
        drawLine(centerX - y, centerY - x, centerX + y, centerY - x, color);

        y++;
        if(decisionOver2 <= 0)
            decisionOver2 += 2*y+1;
        else{
            x--;
            decisionOver2 += 2*(y-x)+1;
        }
    }
}

void VideoGraphicsArray::fillRectangle(int32_t x, int32_t y, uint32_t w, uint32_t h, uint32_t color){
    uint32_t i = width * (y-1);

    if(x>=0 && x+w < width && y>=0 && y+h < height){
        i += x+w;
        for(int32_t y2=h;y2>0;y2--){
            i += width - w;
            for(int32_t x2=w;x2>0;x2--)
                screen[i++] = color;
        }
    }else{
        for(int32_t y2=y;y2<y+h;y2++){
            i += width;
            for(int32_t x2=x;x2<x+w;x2++)
                if(x2 >= 0 && x2 < width && y2 >= 0 && y2 < height)
                    screen[i + x2] = color;
        }
    }
}

void VideoGraphicsArray::fillEllipse(int32_t x0, int32_t y0, int32_t rx, int32_t ry, uint32_t color){
    int32_t x,y;
    int64_t rx2 = (int64_t)rx * rx;
    int64_t ry2 = (int64_t)ry * ry;
    int64_t tworx2 = 2 * rx2;
    int64_t twory2 = 2 * ry2;
    int64_t p;
    int64_t px = 0;
    int64_t py = tworx2 * ry;

    x = 0;
    y = ry;
    drawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
    drawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
    p=round(ry2 - (rx2*ry) + (0.25*rx2));

    while(px < py){
        x++;
        px += twory2;
        if(p < 0){
            p += ry2 + px;
        }else{
            y--;
            py -= tworx2;
            p += ry2 + px - py;
        }
        drawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        drawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
    }

    p=round(ry2*(x + 0.5)*(x + 0.5) + rx2*(y - 1)*(y - 1) - rx2*ry2);
    while(y > 0){
        y--;
        py -= tworx2;
        if(p > 0){
            p += rx2 - py;
        }else{
            x++;
            px += twory2;
            p += rx2 - py + px;
        }
        drawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        drawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
    }
}

void VideoGraphicsArray::fillTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color){
    auto swap = [](int32_t &a, int32_t &b) { int32_t t = a; a = b; b = t; };

    if (y0 > y1) { swap(y0, y1); swap(x0, x1); }
    if (y1 > y2) { swap(y1, y2); swap(x1, x2); }
    if (y0 > y1) { swap(y0, y1); swap(x0, x1); }

    int32_t total_height = y2 - y0;
    for (int32_t i = 0; i < total_height; i++) {
        bool second_half = i > y1 - y0 || y1 == y0;
        int32_t segment_height = second_half ? y2 - y1 : y1 - y0;
        float alpha = (float)i / total_height;
        float beta = (float)(i - (second_half ? y1 - y0 : 0)) / segment_height;

        int32_t A = x0 + (x2 - x0) * alpha;
        int32_t B = second_half ? x1 + (x2 - x1) * beta : x0 + (x1 - x0) * beta;

        if (A > B) swap(A, B);
        drawLine(A, y0 + i, B, y0 + i, color);
    }
}

