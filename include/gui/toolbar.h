#ifndef __BLJOS__GUI__TOOLBAR_H
#define __BLJOS__GUI__TOOLBAR_H

#include <common/types.h>
#include <gui/widget.h>
#include <common/graphicscontext.h>

namespace bljOS{
    namespace gui{
        class Toolbar : public CompositeWidget{
        public:
            Toolbar(Widget* parent, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color);
            virtual void draw(bljOS::common::GraphicsContext* gc);
            virtual void onKeyDown(char key){}
            virtual void onKeyUp(char key){}
        };
    }
}

#endif
