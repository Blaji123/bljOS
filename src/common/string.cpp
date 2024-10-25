#include <common/string.h>

using namespace bljOS;
using namespace bljOS::common;

uint32_t String::strlength(const char* ch){
    uint32_t i = 0;
    while(ch[i++])
        ;
    return i-1;
}
