#ifndef __BLJOS__GUI__WINDOW_H
#define __BLJOS__GUI__WINDOW_H

#include <drivers/mouse.h>
#include <gui/widget.h>
#include <common/types.h>

namespace bljOS{
    namespace gui{
        class Window : public CompositeWidget{
        protected:
            bool Dragging;
        public:
            Window(Widget* parent,bljOS::common::int32_t x,bljOS::common::int32_t y,bljOS::common::int32_t w,bljOS::common::int32_t h,bljOS::common::uint8_t r,bljOS::common::uint8_t g,bljOS::common::uint8_t b);
            ~Window();
            void OnMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t buttton);
            void OnMouseUp(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t buttton);
            void OnMouseMove(bljOS::common::int32_t oldx, bljOS::common::int32_t oldy, bljOS::common::int32_t newx, bljOS::common::int32_t newy);
        };
    }
}

#endif
