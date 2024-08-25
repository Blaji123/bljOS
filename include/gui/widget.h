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
            bljOS::common::int32_t x;
            bljOS::common::int32_t y;
            bljOS::common::int32_t w;
            bljOS::common::int32_t h;

            bljOS::common::uint8_t r;
            bljOS::common::uint8_t g;
            bljOS::common::uint8_t b;
            bool Focussable;
        public:
            Widget(Widget* parent,bljOS::common::int32_t x,bljOS::common::int32_t y,bljOS::common::int32_t w,bljOS::common::int32_t h,bljOS::common::uint8_t r,bljOS::common::uint8_t g,bljOS::common::uint8_t b);
            ~Widget();
            virtual void GetFocus(Widget* widget);
            virtual void ModelToScreen(bljOS::common::int32_t &x, bljOS::common::int32_t &y);
            virtual bool ContainsCoordinate(bljOS::common::int32_t x, bljOS::common::int32_t y);

            virtual void Draw(bljOS::common::GraphicsContext* gc);
            virtual void OnMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t buttton);
            virtual void OnMouseUp(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t buttton);
            virtual void OnMouseMove(bljOS::common::int32_t oldx, bljOS::common::int32_t oldy, bljOS::common::int32_t newx, bljOS::common::int32_t newy);
        };

        class CompositeWidget : public Widget{
        private:
            Widget* children[100];
            int numChildren;
            Widget* focussedChild;

        public:
            CompositeWidget(Widget* parent,bljOS::common::int32_t x,bljOS::common::int32_t y,bljOS::common::int32_t w,bljOS::common::int32_t h,bljOS::common::uint8_t r,bljOS::common::uint8_t g,bljOS::common::uint8_t b);
            ~CompositeWidget();
            virtual void GetFocus(Widget* widget);
            virtual bool AddChild(Widget* child);

            virtual void Draw(bljOS::common::GraphicsContext* gc);
            virtual void OnMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t buttton);
            virtual void OnMouseUp(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t buttton);
            virtual void OnMouseMove(bljOS::common::int32_t oldx, bljOS::common::int32_t oldy, bljOS::common::int32_t newx, bljOS::common::int32_t newy);

            virtual void OnKeyDown(char str);
            virtual void OnKeyUp(char str);
        };
    }
}

#endif
