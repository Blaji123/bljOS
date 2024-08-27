#include <gui/window.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::gui;

Window::Window(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color):CompositeWidget(parent, x, y, width, height, color){
    dragging = false;
}

void Window::onMouseDown(int32_t x, int32_t y, uint8_t button){
    dragging = button == 1;
    CompositeWidget::onMouseDown(x, y, button);
}

void Window::onMouseUp(int32_t x, int32_t y, uint8_t button){
    dragging = false;
    CompositeWidget::onMouseUp(x, y, button);
}

void Window::onMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY){
    if(dragging){
        this->x += newX - oldX;
        this->y += newY - oldY;
    }

    CompositeWidget::onMouseMove(oldX, oldY, newX, newY);
}
