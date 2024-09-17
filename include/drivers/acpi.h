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

        struct GenericAddressStructure{
            bljOS::common::uint8_t address_space;
            bljOS::common::uint8_t bit_width;
            bljOS::common::uint8_t bit_offset;
            bljOS::common::uint8_t access_size;
            bljOS::common::uint64_t address;
        };

        struct FADT{
            struct ACPISDTHeader h;
            bljOS::common::uint32_t firmware_ctrl;
            bljOS::common::uint32_t dsdt;

            bljOS::common::uint8_t reserved;

            bljOS::common::uint8_t preferred_power_management_profile;
            bljOS::common::uint16_t sci_interrupt;
            bljOS::common::uint32_t smi_commandport;
            bljOS::common::uint8_t acpi_enable;
            bljOS::common::uint8_t acpi_disable;
            bljOS::common::uint8_t s4bios_req;
            bljOS::common::uint8_t pstate_control;
            bljOS::common::uint32_t pm1a_event_block;
            bljOS::common::uint32_t pm1b_event_block;
            bljOS::common::uint32_t pm1a_control_block;
            bljOS::common::uint32_t pm1b_control_block;
            bljOS::common::uint32_t pm2_control_block;
            bljOS::common::uint32_t pm_timer_block;
            bljOS::common::uint32_t gpe0_block;
            bljOS::common::uint32_t gpe1_block;
            bljOS::common::uint8_t pm1_event_length;
            bljOS::common::uint8_t pm1_control_length;
            bljOS::common::uint8_t pm2_control_length;
            bljOS::common::uint8_t pm_timer_length;
            bljOS::common::uint8_t gpe0_length;
            bljOS::common::uint8_t gpe1_length;
            bljOS::common::uint8_t gpe1_base;
            bljOS::common::uint8_t cstate_control;
            bljOS::common::uint16_t worstc2_latency;
            bljOS::common::uint16_t worstc3_latency;
            bljOS::common::uint16_t flush_size;
            bljOS::common::uint16_t flush_stride;
            bljOS::common::uint8_t duty_offset;
            bljOS::common::uint8_t duty_width;
            bljOS::common::uint8_t day_alarm;
            bljOS::common::uint8_t month_alarm;
            bljOS::common::uint8_t century;

            bljOS::common::uint16_t boot_architecture_flags;

            bljOS::common::uint8_t reserved2;
            bljOS::common::uint32_t flags;

            GenericAddressStructure reset_reg;

            bljOS::common::uint8_t reset_value;
            bljOS::common::uint8_t reserved3[3];

            bljOS::common::uint64_t x_firmware_control;
            bljOS::common::uint64_t x_dsdt;

            GenericAddressStructure x_pm1a_event_block;
            GenericAddressStructure x_pm1b_event_block;
            GenericAddressStructure x_pm1a_control_block;
            GenericAddressStructure x_pm1b_control_block;
            GenericAddressStructure x_pm2_control_block;
            GenericAddressStructure x_pm_timer_block;
            GenericAddressStructure x_gpe0_block;
            GenericAddressStructure x_gpe1_block;
        };

        class AdvancedConfigurationPowerInterface : public Driver{
            RSDP_t* rsdp;
            RSDT* rsdt;
        public:
            AdvancedConfigurationPowerInterface();

            void* find_rsdp();
            struct RSDT* get_rsdt(void* rsdp);
            void parse_rsdt(struct RSDT* table);
            void parse_fadt(struct FADT* fadt);
        };

    }
}

#endif
