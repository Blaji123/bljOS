#include <gui/desktop.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::gui;

Desktop::Desktop(int32_t width, int32_t height, uint32_t color):CompositeWidget(0, 0, 0, width, height, color), MouseEventHandler(){
    mouseX = width/2;
    mouseY = height/2;
}

void Desktop::drawToolbar(GraphicsContext* gc, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color){
    gc->drawLine(x, y - 1, 1024, y -1, 0x1d2021);
    gc->fillRectangle(x, y, width, height, color);
}

void Desktop::draw(GraphicsContext* gc){
    drawToolbar(gc, 0, 735, 1024, 34, 0x3c3836);
    CompositeWidget::draw(gc);
    gc->putPixel(mouseX, mouseY, 0xFFFFFF); // Tip of the arrow (white pixel)
    gc->putPixel(mouseX + 1, mouseY + 1, 0xFFFFFF);
    gc->putPixel(mouseX + 2, mouseY + 2, 0xFFFFFF);
    gc->putPixel(mouseX + 3, mouseY + 3, 0xFFFFFF);
    gc->putPixel(mouseX + 4, mouseY + 4, 0xFFFFFF);
    gc->putPixel(mouseX + 5, mouseY + 5, 0xFFFFFF);
    gc->putPixel(mouseX + 6, mouseY + 6, 0xFFFFFF);

    gc->putPixel(mouseX + 1, mouseY, 0x000000); // Outline of the arrow (black pixel)
    gc->putPixel(mouseX + 2, mouseY + 1, 0x000000);
    gc->putPixel(mouseX + 3, mouseY + 2, 0x000000);
    gc->putPixel(mouseX + 4, mouseY + 3, 0x000000);
    gc->putPixel(mouseX + 5, mouseY + 4, 0x000000);
    gc->putPixel(mouseX + 6, mouseY + 5, 0x000000);
    gc->putPixel(mouseX + 7, mouseY + 6, 0x000000);
}

bool Desktop::getRedraw(){
    return CompositeWidget::getRedraw();
}

void Desktop::setRedraw(bool redraw){
    CompositeWidget::setRedraw(redraw);
}


void Desktop::onMouseDown(uint8_t buttons){
    CompositeWidget::onMouseDown(mouseX, mouseY, buttons);
}

void Desktop::onMouseUp(uint8_t buttons){
    CompositeWidget::onMouseUp(mouseX, mouseY, buttons);
}

void Desktop::onMouseMove(int x, int y){
    x /= 4;
    y /= 4;
    int32_t newMouseX = mouseX + x;
    if(newMouseX < 0) newMouseX = 0;
    if(newMouseX >= width) newMouseX = width - 1;

    int32_t newMouseY = mouseY + y;
    if(newMouseY < 0) newMouseY = 0;
    if(newMouseY >= height) newMouseY = height - 1;

    setRedraw(true);
    CompositeWidget::onMouseMove(mouseX, mouseY, newMouseX, newMouseY);

    mouseX = newMouseX;
    mouseY = newMouseY;
}
