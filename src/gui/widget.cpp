#include <gui/widget.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::gui;

Widget::Widget(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color){
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->focussable = true;
    this->redraw = false;
}

void Widget::getFocus(Widget* widget){
    if(parent!=0)
        parent->getFocus(widget);
}

void Widget::modelToScreen(int32_t &x, int32_t &y){
    if(parent!=0)
        parent->modelToScreen(x, y);
    x += this->x;
    y += this->y;
}

bool Widget::containsCoordinate(int32_t x, int32_t y){
    return this->x <= x && x < this->x + this->width && this->y <= y && y<this->y + this->height;
}

void Widget::draw(GraphicsContext* gc){
    int X = 0;
    int Y = 0;
    modelToScreen(X,Y);
    gc->fillRectangle(X, Y, width, height, color);
}

void Widget::onMouseDown(int32_t x, int32_t y, uint8_t buttton){
    if(focussable)
        getFocus(this);
}

CompositeWidget::CompositeWidget(Widget* parent, int32_t x, int32_t y, int32_t width, int32_t height, uint32_t color):Widget(parent, x, y, width, height, color){
    focussedChild = 0;
    numChildren = 0;
}

void CompositeWidget::getFocus(Widget* widget){
    this->focussedChild = widget;
    if(parent!=0)
        parent->getFocus(this);
}

bool CompositeWidget::addChild(Widget* child){
    if(numChildren >= 100)
        return false;
    children[numChildren++] = child;
    return true;
}

void CompositeWidget::draw(GraphicsContext* gc){
    Widget::draw(gc);
    for(int i=numChildren-1;i>=0;i--)
        children[i]->draw(gc);
}

void CompositeWidget::onMouseDown(int32_t x, int32_t y, uint8_t button){
    for(int i=0;i<numChildren;i++)
        if(children[i]->containsCoordinate(x - this->x, y - this->y)){
            children[i]->onMouseDown(x - this->x, y - this->y, button);
            break;
        }
}

void CompositeWidget::onMouseUp(int32_t x, int32_t y, uint8_t button){
    for(int i=0;i<numChildren;i++)
        if(children[i]->containsCoordinate(x - this->x, y - this->y)){
            children[i]->onMouseUp(x - this->x, y - this->y, button);
            break;
        }
}

void CompositeWidget::onMouseMove(int32_t oldX, int32_t oldY, int32_t newX, int32_t newY){
    int firstChild = -1;
    for(int i=0;i<numChildren;i++)
        if(children[i]->containsCoordinate(oldX - this->x, oldY - this->y)){
            children[i]->onMouseMove(oldX - this->x, oldY - this->y, newX - this->x, newY - this->y);
            firstChild = i;
            break;
        }
    for(int i=0;i<numChildren;i++)
        if(children[i]->containsCoordinate(newX - this->x, newY - this->y)){
            if(firstChild!=i)
                children[i]->onMouseMove(oldX - this->x, oldY - this->y,newX - this->x, newY - this->y);
            break;
        }
}

void CompositeWidget::onKeyDown(char key){
    if(focussedChild!=0)
        focussedChild->onKeyDown(key);
}

void CompositeWidget::onKeyUp(char key){
    if(focussedChild!=0)
        focussedChild->onKeyUp(key);
}


bool CompositeWidget::getRedraw(){
    return this->redraw;
}

void CompositeWidget::setRedraw(bool redraw){
    this->redraw = redraw;
}


