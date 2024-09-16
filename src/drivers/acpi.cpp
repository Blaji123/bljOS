#include <drivers/acpi.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;

void* AdvancedConfigurationPowerInterface::find_rsdp(){
    for(uint8_t* ptr=(uint8_t*)0xE0000;ptr<(uint8_t*)0xFFFFF;ptr+=16)
        if(memcmp(ptr, ACPI_RSDP_SIGNATURE, 8) == 0)
            return (void*)ptr;

    return 0;
}

struct RSDT* AdvancedConfigurationPowerInterface::get_rsdt(void* rsdp){
    struct RSDP_t* desc = (struct RSDP_t*)rsdp;
    return (struct RSDT*)desc->rsdt_address;
}
