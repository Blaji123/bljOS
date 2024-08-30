#ifndef __BLJOS__GUI__BUTTON_H
#define __BLJOS__GUI__BUTTON_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <gui/widget.h>
#include <drivers/rtc.h>

namespace bljOS{
    namespace gui{
        class Button : public CompositeWidget{
        protected:
            bool enabled;
            char* label;
            bljOS::common::uint32_t labelColor;
        public:
            Button(Widget* parent, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height, bljOS::common::uint32_t color, char* label, bljOS::common::uint32_t labelColor);

            virtual void draw(bljOS::common::GraphicsContext* gc);
            virtual void onMouseDown(bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::uint8_t button);
            virtual void onClick() = 0;

            void setEnabled(bool enabled);
            bool isEnabled() const;
        };

        class TimeButton : public Button, public bljOS::drivers::RTCEventHandler{
        private:
            bljOS::drivers::DateTime dateTime;
            char* timeString;
        public:
            TimeButton(Widget* parent, bljOS::common::int32_t x, bljOS::common::int32_t y, bljOS::common::int32_t width, bljOS::common::int32_t height,  bljOS::common::uint32_t color, bljOS::common::uint32_t labelColor);

            virtual bool getRedraw();
            virtual void setRedraw(bool redraw);
            void onTimeChange(const bljOS::drivers::DateTime& time);
            void onClick();
            void draw(bljOS::common::GraphicsContext* gc);
        };
    }
}

#endif
