#ifndef __BLJOS__DRIVERS__ACPI_H
#define __BLJOS__DRIVERS__ACPI_H

#include <common/types.h>
#include <drivers/driver.h>
#include <memorymanagement.h>

#define ACPI_RSDP_SIGNATURE "RSD PTR "

namespace bljOS{
    namespace drivers{

        struct RSDP_t{
            char signature[8];
            bljOS::common::uint8_t checksum;
            char oem_id[6];
            bljOS::common::uint8_t revision;
            bljOS::common::uint32_t rsdt_address;
        }__attribute__((packed));

        struct XSDP_t{
            char signature[8];
            bljOS::common::uint8_t checksum;
            char oem_id[6];
            bljOS::common::uint8_t revision;
            bljOS::common::uint32_t rsdt_address;

            bljOS::common::uint32_t length;
            bljOS::common::uint64_t xsdt_address;
            bljOS::common::uint8_t extended_checksum;
            bljOS::common::uint8_t reserved[3];
        }__attribute__((packed));

        struct ACPISDTHeader{
            char signature[4];
            bljOS::common::uint32_t length;
            bljOS::common::uint8_t revision;
            bljOS::common::uint8_t checksum;
            char oem_id[6];
            char oem_table_id[8];
            bljOS::common::uint32_t oem_revision;
            bljOS::common::uint32_t creator_id;
            bljOS::common::uint32_t creator_revision;
        }__attribute__((packed));

        struct RSDT{
            struct ACPISDTHeader h;
            bljOS::common::uint32_t entry[];
        };

        class AdvancedConfigurationPowerInterface : public Driver{
            RSDP_t* rsdp;
            RSDT* rsdt;
        public:
            AdvancedConfigurationPowerInterface();

            void* find_rsdp();
            struct RSDT* get_rsdt(void* rsdp);
            void parse_rsdt(struct rsdt* table);
        };

    }
}

#endif
