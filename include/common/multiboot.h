#ifndef __BLJOS__MULTIBOOT_H
#define __BLJOS__MULTIBOOT_H

#include <common/types.h>

namespace bljOS{

    struct MultibootInfo{
        bljOS::common::uint32_t flags;
        bljOS::common::uint32_t mem_lower;
        bljOS::common::uint32_t mem_upper;
        bljOS::common::uint32_t boot_device;
        bljOS::common::uint32_t cmdline;
        bljOS::common::uint32_t mods_count;
        bljOS::common::uint32_t mods_addr;
        bljOS::common::uint32_t syms[4];
        bljOS::common::uint32_t mmap_length;
        bljOS::common::uint32_t mmap_addr;
        bljOS::common::uint32_t drives_length;
        bljOS::common::uint32_t drives_addr;
        bljOS::common::uint32_t config_table;
        bljOS::common::uint32_t boot_loader_name;
        bljOS::common::uint32_t apm_table;

        bljOS::common::uint32_t vbe_control_info;
        bljOS::common::uint32_t vbe_mode_info;
        bljOS::common::uint16_t vbe_mode;
        bljOS::common::uint16_t vbe_interface_seg;
        bljOS::common::uint16_t vbe_interface_off;
        bljOS::common::uint16_t vbe_interface_len;

        bljOS::common::uint64_t framebuffer_addr;
        bljOS::common::uint32_t framebuffer_pitch;
        bljOS::common::uint32_t framebuffer_width;
        bljOS::common::uint32_t framebuffer_height;
        bljOS::common::uint8_t framebuffer_bpp;
        bljOS::common::uint8_t framebuffer_type;
        bljOS::common::uint8_t reserved;
    }__attribute__((packed));
}

#endif
