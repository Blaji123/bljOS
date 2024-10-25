#ifndef __BLJOS__DATASTRUCTURES__LINKEDLIST_H
#define __BLJOS__DATASTRUCTURES__LINKEDLIST_H

#include <memorymanagement.h>
#include <common/types.h>

namespace bljOS{
    namespace datastructures{

        struct LLheader{
            LLheader* next;
        };

        class LinkedList{
        public:
            LinkedList(){}

            static void* LinkedListAllocate(void** LLFirstPtr, bljOS::common::uint32_t structSize);
            static bool LinkedListUnregister(void** LLFirstPtr, const void* LLTarget);
            static bool LinkedListRemove(void** LLFirstPtr, void** LLtarget);
            static bool LinkedListDuplicate(void** LLFirstPtrSource, void** LLFirstPtrTarget, bljOS::common::uint32_t structSize);
            static void LinkedListPushFrontUnsafe(void** LLFirstPtr, void* LLTarget);
        };

    }
}

#endif
