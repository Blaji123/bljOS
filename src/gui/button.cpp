#include <gui/button.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::gui;
using namespace bljOS::drivers;

void printfDec(uint8_t value, int32_t x, int32_t y, uint32_t color);
void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);

Button::Button(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, char* label, uint32_t labelColor):CompositeWidget(parent, x, y, width, height, color){
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

TimeButton::TimeButton(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color, uint32_t labelColor):Button(parent, x, y, width, height, color, (char*)"", labelColor){
    timeString[0] = '\0';
    label = timeString;
}

bool TimeButton::getRedraw(){
    return CompositeWidget::getRedraw();
}

void TimeButton::setRedraw(bool redraw){
    CompositeWidget::setRedraw(redraw);
}

void TimeButton::onTimeChange(const DateTime& time){
    dateTime = time;

    timeString[0] = '0' + (dateTime.hour / 10);
    timeString[1] = '0' + (dateTime.hour % 10);
    timeString[2] = ':';
    timeString[3] = '0' + (dateTime.minute / 10);
    timeString[4] = '0' + (dateTime.minute % 10);
    timeString[5] = '\0';

    label = timeString;

    setRedraw(true);
}

void TimeButton::onClick(){

}

void TimeButton::draw(GraphicsContext* gc){
    Button::draw(gc);

    gc->putStr((uint8_t*)label, x + 2, y + (height/2 - 2), labelColor);
}
