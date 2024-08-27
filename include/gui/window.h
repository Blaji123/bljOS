#ifndef __BLJOS__GUI__WINDOW_H
#define __BLJOS__GUI__WINDOW_H

#include <drivers/mouse.h>
#include <gui/widget.h>
#include <common/types.h>

namespace bljOS{
    namespace gui{
        class Window : public CompositeWidget{
        protected:
            bool dragging;
        public:
            Window(Widget* parent, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color);

            virtual void onMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button);
            virtual void onMouseUp(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button);
            virtual void onMouseMove(bljOS::common::int32_t oldX, bljOS::common::int32_t oldY, bljOS::common::int32_t newX, bljOS::common::int32_t newY);
        };
    }
}

#endif
