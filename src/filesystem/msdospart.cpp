#include <filesystem/msdospart.h>
#include <filesystem/fat32.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::drivers;
using namespace bljOS::filesystem;

void printf(char*);
void printfHex(uint8_t);

void MSDOSPartitionTable::ReadPartitions(AdvancedTechnologyAttachment* hd){
    MasterBootRecord mbr;
    hd->Read28(0, (uint8_t*)&mbr, sizeof(MasterBootRecord));

//     printf("MBR: ");
//     for(int i=446; i<446 + 4*16;i++){
//         printfHex(((uint8_t*)&mbr)[i]);
//         printf(" ");
//     }
//     printf("\n");

    if(mbr.magicNumber != 0xAA55){
        printf("illegal MBR");
        return;
    }

    for(int i=0;i<4;i++){
        if(mbr.primaryPartition[i].partition_id == 0x00)
            continue;
        printf(" Parition ");
        printfHex(i & 0xFF);
        if(mbr.primaryPartition[i].bootable == 0x80)
            printf(" bootable. Type");
        else
            printf(" not bootable. Type");
        printfHex(mbr.primaryPartition[i].partition_id);

        ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    }
}
