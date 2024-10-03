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

void AdvancedConfigurationPowerInterface::parse_rsdt(struct RSDT* table){
    int entries = (table->h.length - sizeof(struct ACPISDTHeader)) / sizeof(uint32_t);
    for(int i=0;i<entries;i++){
        struct ACPISDTHeader* hdr = (struct ACPISDTHeader*)table->entry[i];
        if(memcmp(hdr->signature, "FACP", 4)==0)
            parse_fadt((struct FADT*)hdr);
    }
}

void AdvancedConfigurationPowerInterface::parse_fadt(struct FADT* fadt){
    if(fadt->dsdt != 0){
        struct ACPISDTHeader* dsdt = (struct ACPISDTHeader*)fadt->dsdt;
//         parse_dsdt(dsdt);
    }

    if(fadt->pm1a_event_block){
        uint32_t pm1a_event_value = *(volatile uint32_t*)fadt->pm1a_event_block;
    }

}
