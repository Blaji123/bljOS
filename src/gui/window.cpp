#include <gui/window.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::gui;

Window::Window(Widget* parent,int32_t x,int32_t y,int32_t w,int32_t h,uint8_t r,uint8_t g,uint8_t b):CompositeWidget(parent, x, y, w ,h, r, g, b){
    Dragging = false;
}

Window::~Window(){

}

void Window::OnMouseDown(int32_t x, int32_t y, uint8_t buttton){
    Dragging = buttton == 1;
    CompositeWidget::OnMouseDown(x,y,buttton);
}

void Window::OnMouseUp(int32_t x, int32_t y, uint8_t buttton){
    Dragging = false;
    CompositeWidget::OnMouseUp(x,y,buttton);
}

void Window::OnMouseMove(int32_t oldx, int32_t oldy, int32_t newx, int32_t newy){
    if(Dragging){
        this->x += newx-oldx;
        this->y = newy-oldy;
    }
    CompositeWidget::OnMouseMove(oldx,oldy,newx, newy);
}
