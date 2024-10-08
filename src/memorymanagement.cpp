#include <memorymanagement.h>

using namespace bljOS;
using namespace bljOS::common;

MemoryManager* MemoryManager::activeMemoryManager = 0;

MemoryManager::MemoryManager(size_t start, size_t size){
    activeMemoryManager = this;
    if(size < sizeof(MemoryChunk)){
        first = 0;
    }else{
        first = (MemoryChunk*)start;

        first->allocated = false;
        first->prev = 0;
        first->next = 0;
        first->size = size - sizeof(MemoryChunk);
    }
}

MemoryManager::~MemoryManager(){
    if(activeMemoryManager == this)
        activeMemoryManager = 0;
}

void* MemoryManager::malloc(size_t size){
    MemoryChunk* result = 0;
    for(MemoryChunk* chunk = first; chunk!=0 && result==0; chunk = chunk->next){
        if(chunk->size > size && !chunk->allocated)
            result = chunk;
    }
    if(result == 0)
        return 0;
    if(result->size >= size + sizeof(MemoryChunk) + 1){
        MemoryChunk* temp = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
        temp->allocated = false;
        temp->size = result->size - size - sizeof(MemoryChunk);
        temp->prev = result;
        temp->next = result->next;
        if(temp->next!=0)
            temp->next->prev = temp;

        result->size = size;
        result->next = temp;
    }
    result->allocated = true;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}

void MemoryManager::free(void* ptr){
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
    chunk->allocated = false;

    if(chunk->prev != 0 && !chunk->prev->allocated){
        chunk->prev->next = chunk->next;
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        if(chunk->next!=0)
            chunk->next->prev = chunk->prev;
        chunk = chunk->prev;
    }
    if(chunk->next!=0 && !chunk->next->allocated){
        chunk->size += chunk->next->size + sizeof(MemoryChunk);
        chunk->next = chunk->next->next;
        if(chunk->next != 0)
            chunk->next->prev = chunk;
    }

}

int memcmp(const void* ptr1, const void* ptr2, size_t num) {
    const unsigned char* p1 = (const unsigned char*) ptr1;
    const unsigned char* p2 = (const unsigned char*) ptr2;

    for (size_t i = 0; i < num; i++) {
        if (p1[i] != p2[i]) {
            return (p1[i] - p2[i]);
        }
    }

    return 0;  // They are equal
}

void *memcpy(void *dest, void *src, size_t n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}

void *memset(void *ptr, int value, size_t num) {
    unsigned char *p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

void* operator new(unsigned size){
    if(bljOS::MemoryManager::activeMemoryManager == 0)
        return 0;
    return bljOS::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new[](unsigned size){
    if(bljOS::MemoryManager::activeMemoryManager == 0)
        return 0;
    return bljOS::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new(unsigned size, void* ptr){
    return ptr;
}

void* operator new[](unsigned size, void* ptr){
    return ptr;
}

void operator delete(void* ptr){
    if(bljOS::MemoryManager::activeMemoryManager != 0)
        bljOS::MemoryManager::activeMemoryManager->free(ptr);
}

void operator delete[](void* ptr){
    if(bljOS::MemoryManager::activeMemoryManager != 0)
        bljOS::MemoryManager::activeMemoryManager->free(ptr);
}


