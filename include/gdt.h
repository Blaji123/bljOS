#ifndef __BLJOS__GDT_H
#define __BLJOS__GDT_H

#include <common/types.h>

namespace bljOS{

    class GlobalDescriptorTable{
    public:
        class SegmentDescriptor{
        private:
            bljOS::common::uint16_t limit_lo;
            bljOS::common::uint16_t base_lo;
            bljOS::common::uint8_t base_hi;
            bljOS::common::uint8_t type;
            bljOS::common::uint8_t flags_limit_hi;
            bljOS::common::uint8_t base_vhi;
        public:
            SegmentDescriptor(bljOS::common::uint32_t base, bljOS::common::uint32_t limit, bljOS::common::uint8_t type);

            bljOS::common::uint32_t getBase();
            bljOS::common::uint32_t getLimit();
        } __attribute__((packed));

    private:
        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

    public:
        GlobalDescriptorTable();

        bljOS::common::uint16_t getCodeSegmentSelector();
        bljOS::common::uint16_t getDataSegmentSelector();
    };
}
#endif
