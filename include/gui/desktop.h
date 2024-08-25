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
            bljOS::common::uint32_t MouseX;
            bljOS::common::uint32_t MouseY;

        public:
            Desktop(bljOS::common::int32_t w, bljOS::common::int32_t h, bljOS::common::uint8_t r, bljOS::common::uint8_t g, bljOS::common::uint8_t b);
            ~Desktop();

            void Draw(bljOS::common::GraphicsContext* gc);
            void OnMouseDown(bljOS::common::uint8_t buttons);
            void OnMouseUp(bljOS::common::uint8_t buttons);
            void OnMouseMove(int x, int y);
        };
    }
}

#endif
