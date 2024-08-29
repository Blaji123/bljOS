#include <gui/button.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::gui;
using namespace bljOS::drivers;

Button::Button(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint8_t* label, uint32_t labelColor):CompositeWidget(parent, x, y, width, height, color){
    this->label = label;
    this->enabled = true;
    this->labelColor = labelColor;
}

void Button::draw(GraphicsContext* gc){
    gc->fillRectangle(x, y, width, height, color);
}

void Button::onMouseDown(int32_t x, int32_t y, uint8_t button){
    CompositeWidget::onMouseDown(x, y, button);
}

void Button::setEnabled(bool enabled){
    this->enabled = enabled;
}

bool Button::isEnabled() const{
    return this->enabled;
}

TimeButton::TimeButton(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint32_t labelColor):Button(parent, x, y, width, height, color, (uint8_t*)"da", labelColor){

}

void TimeButton::onTimeChange(const DateTime& time){
    dateTime = time;

    label = (uint8_t*)"nu";

    setRedraw(true);
}

void TimeButton::onClick(){

}

void TimeButton::draw(GraphicsContext* gc){
    Button::draw(gc);

    if(label){
        gc->putStr(label, x + 5, y + 5, labelColor);
    }
}
