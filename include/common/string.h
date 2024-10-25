#ifndef __BLJOS__COMMON__STRING_H
#define __BLJOS__COMMON__STRING_H

#include <common/types.h>

namespace bljOS{
    namespace common{

    class String{
    public:
        static uint32_t strlength(const char* ch);
    };

    }
}

#endif
