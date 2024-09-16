#ifndef __BLJOS__MEMORYMANAGEMENT_H
#define __BLJOS__MEMORYMANAGEMENT_H

#include <common/types.h>

namespace bljOS{
    struct MemoryChunk{
        MemoryChunk* next;
        MemoryChunk* prev;
        bool allocated;
        bljOS::common::size_t size;
    };

    class MemoryManager{
    protected:
        MemoryChunk* first;
    public:
        static MemoryManager* activeMemoryManager;
        MemoryManager(bljOS::common::size_t start, bljOS::common::size_t size);
        ~MemoryManager();

        void* malloc(bljOS::common::size_t size);
        void free(void* ptr);
    };
}

void *memset(void *ptr, int value, bljOS::common::size_t num);
void *memcpy(void *dest, void *src, bljOS::common::size_t n);
int memcmp(const void* ptr1, const void* ptr2, bljOS::common::size_t num);

void* operator new(unsigned size);
void* operator new[](unsigned size);

void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif
