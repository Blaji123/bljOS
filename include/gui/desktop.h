#ifndef __BLJOS__GUI__DESKTOP_H
#define __BLJOS__GUI__DESKTOP_H

#include <drivers/mouse.h>
#include <gui/widget.h>
#include <common/types.h>
#include <common/graphicscontext.h>

namespace bljOS{
    namespace gui{
        class Desktop : public CompositeWidget, public bljOS::drivers::MouseEventHandler{
        protected:
            bljOS::common::uint32_t mouseX, mouseY;
        public:
            Desktop(bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color);

            virtual void draw(bljOS::common::GraphicsContext* gc);
            virtual bool getRedraw();
            virtual void setRedraw(bool redraw);
            void drawToolbar(bljOS::common::GraphicsContext* gc, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color);
            virtual void onMouseDown(bljOS::common::uint8_t buttons);
            virtual void onMouseUp(bljOS::common::uint8_t buttons);
            virtual void onMouseMove(int x, int y);
        };
    }
}

#endif
