#include <datastructures/linkedlist.h>

using namespace bljOS;
using namespace bljOS::common;
using namespace bljOS::datastructures;

void* LinkedList::LinkedListAllocate(void** LLFirstPtr, uint32_t structSize){
    LLheader* target = (LLheader*)MemoryManager::activeMemoryManager->malloc(structSize);
    memset(target, 0, structSize);

    LLheader* curr = (LLheader*)(*LLFirstPtr);
    while(1){

        if(curr == 0){
            *LLFirstPtr = target;
            break;
        }

        if(curr->next == 0){
            curr->next = target;
            break;
        }

        curr = curr->next;
    }

    target->next = 0;
    return target;
}

bool LinkedList::LinkedListUnregister(void** LLFirstPtr, const void* LLTarget){
    LLheader* LLFirstCopy = (LLheader*)LLFirstPtr;

    LLheader* curr = (LLheader*)(*LLFirstPtr);
    while(curr){
        if(curr->next && curr->next == LLTarget)
            break;
        curr = curr->next;
    }

    if(LLFirstCopy == LLTarget){
        *LLFirstPtr = LLFirstCopy->next;
        return true;
    }else if(!curr)
        return false;

    LLheader* target = curr->next;
    curr->next = target->next;

    return true;
}

bool LinkedList::LinkedListRemove(void** LLFirstPtr, void** LLtarget){
    bool res = LinkedListUnregister(LLFirstPtr, LLtarget);
    MemoryManager::activeMemoryManager->free(LLtarget);
    return res;
}

bool LinkedList::LinkedListDuplicate(void** LLFirstPtrSource, void** LLFirstPtrTarget, uint32_t structSize){
    LLheader* browse = (LLheader*)(LLFirstPtrSource);
    while(browse){
        LLheader* LLNew = (LLheader*)LinkedListAllocate(LLFirstPtrTarget, structSize);
        memcpy((void*)((size_t)LLNew + sizeof(LLNew->next)), (void*)((size_t)browse + sizeof(browse->next)), structSize - sizeof(browse->next));
        browse = browse->next;
    }
    return true;
}

void LinkedList::LinkedListPushFrontUnsafe(void** LLFirstPtr, void* LLTarget){
    if(*LLFirstPtr == 0){
        *LLFirstPtr = LLTarget;
        return;
    }

    void* next = *LLFirstPtr;
    *LLFirstPtr = LLTarget;
    LLheader* target = (LLheader*)(LLTarget);
    target->next = (LLheader*)next;
}

