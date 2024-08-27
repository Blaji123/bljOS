#ifndef __BLJOS__GUI__WIDGET_H
#define __BLJOS__GUI__WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>

namespace bljOS{
    namespace gui{

        class Widget : public bljOS::drivers::KeyboardEventHandler{
        protected:
            Widget* parent;
            bljOS::common::int32_t x, y;
            bljOS::common::int32_t width, height;
            bljOS::common::uint32_t color;
            bool focussable;
            bool redraw;
        public:
            Widget(Widget* parent, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color);

            virtual void getFocus(Widget* widget);
            virtual void modelToScreen(bljOS::common::int32_t &x, bljOS::common::int32_t &y);
            virtual bool containsCoordinate(bljOS::common::int32_t x, bljOS::common::int32_t y);
            virtual void draw(bljOS::common::GraphicsContext* gc);
            virtual void onMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button);
            virtual void onMouseUp(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button){}
            virtual void onMouseMove(bljOS::common::int32_t oldX, bljOS::common::int32_t oldY, bljOS::common::int32_t newX, bljOS::common::int32_t newY){}
        };

        class CompositeWidget : public Widget{
        protected:
            Widget* children[100];
            int numChildren;
            Widget* focussedChild;
            bool redraw;
        public:
            CompositeWidget(Widget* parent, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color);

            virtual void getFocus(Widget* widget);
            virtual bool addChild(Widget* child);
            virtual void draw(bljOS::common::GraphicsContext* gc);
            virtual void onMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button);
            virtual void onMouseUp(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button);
            virtual void onMouseMove(bljOS::common::int32_t oldX, bljOS::common::int32_t oldY, bljOS::common::int32_t newX, bljOS::common::int32_t newY);
            virtual void onKeyDown(char key);
            virtual void onKeyUp(char key);
            virtual bool getRedraw();
            virtual void setRedraw(bool redraw);
        };
    }
}

#endif
