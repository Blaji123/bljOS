#include <gui/toolbar.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::gui;

Toolbar::Toolbar(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color):CompositeWidget(parent, x, y, width, height, color){}

void Toolbar::draw(GraphicsContext* gc){
    gc->drawLine(x, y - 1, 1024, y -1, 0x1d2021);
    gc->fillRectangle(x, y, width, height, color);

    CompositeWidget::draw(gc);
}
