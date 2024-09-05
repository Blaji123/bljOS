#include <drivers/disk.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::hardwarecommunication;

MBR_PARTITION_TABLE::MBR_PARTITION_TABLE(AdvancedHostControllerInterface* ahci):ahci{ahci}{}

bljOS::common::uint16_t mbr_partition_indexes[] = {MBR_PARTITION_1, MBR_PARTITION_2, MBR_PARTITION_3, MBR_PARTITION_4};


bool MBR_PARTITION_TABLE::openDisk(uint32_t disk, uint8_t partition, MBR_PARTITION* out){
    uint8_t* rawArr = (uint8_t*)MemoryManager::activeMemoryManager->malloc(SECTOR_SIZE);
    bool ret = validateMbr(rawArr);

    if(!ret)
        return false;

    memcpy((void*)out, (void*)((size_t)rawArr + mbr_partition_indexes[partition]), sizeof(MBR_PARTITION));
    MemoryManager::activeMemoryManager->free(rawArr);

    return true;
}

bool MBR_PARTITION_TABLE::validateMbr(uint8_t* mbrSector){
    return mbrSector[510] == 0x55 && mbrSector[511] == 0xAA;
}

void MBR_PARTITION_TABLE::diskBytes(uint8_t *target_address, uint32_t LBA, uint32_t sector_count, bool write){
    int pos = 0;

    while(!(ahci->sata & (1<<pos)))
        pos++;

    if(write)
        ahci->write(&ahci->abar->ports[pos], LBA, 0, sector_count, target_address);
    else
        ahci->read(&ahci->abar->ports[pos], LBA, 0, sector_count, target_address);
}

void MBR_PARTITION_TABLE::getDiskBytes(uint8_t* target_address, uint32_t LBA, size_t sector_count){
    int max = 65536;

    size_t chunks = sector_count / max;
    size_t remainder = sector_count % max;
    if(chunks)
        for(size_t i=0;i<chunks;i++)
            diskBytes(target_address + i * max * SECTOR_SIZE, LBA + i * max, max, false);

    if(remainder)
            diskBytes(target_address + chunks * max * SECTOR_SIZE, LBA + chunks * max, remainder, false);
}

void MBR_PARTITION_TABLE::setDiskBytes(const uint8_t* target_address, uint32_t LBA, uint8_t sector_count){
    uint8_t* rw_target_address = (uint8_t*)((size_t)target_address);
    diskBytes(rw_target_address, LBA, sector_count, true);
}
