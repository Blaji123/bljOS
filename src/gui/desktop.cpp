#include <gui/desktop.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::gui;
using namespace bljOS::datastructures;

Desktop::Desktop(int32_t width, int32_t height, uint32_t color, Bitmap* bitmap):CompositeWidget(0, 0, 0, width, height, color), MouseEventHandler(){
    this->bitmap = bitmap;
    mouseX = width/2;
    mouseY = height/2;
}

void Desktop::draw(GraphicsContext* gc){
    CompositeWidget::draw(gc);

    bitmap->drawBMP(mouse_icon, gc, mouseX, mouseY);
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
