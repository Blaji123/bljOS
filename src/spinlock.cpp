#include <spinlock.h>

using namespace bljOS;
using namespace bljOS::common;

void printf(uint8_t* str, int32_t x, int32_t y, uint32_t color);

void spinlockAcquire(Spinlock* lock){
    while(atomic_flag_test_and_set_explicit(lock, memory_order_acquire))
        asm volatile("pause");
}

void spinlockRelease(Spinlock* lock){
    atomic_flag_clear_explicit(lock, memory_order_release);
}

void spinlockWait(Spinlock* lock){
    while(atomic_flag_test_and_set_explicit(lock, memory_order_relaxed)){
        atomic_flag_clear_explicit(lock, memory_order_relaxed);
    }
}

void spinlockCntReadAcquire(SpinlockCnt* lock){
    while(lock->cnt < 0)
        asm volatile("pause");
    lock->cnt++;
}

void spinlockCntReadRelease(SpinlockCnt* lock){
    if(lock->cnt < 0){
        printf((uint8_t*)"error", 20, 20, 0xebdbb2);
    }
    lock->cnt--;
}

void spinlockCntWriteAcquire(SpinlockCnt* lock){
    while(lock->cnt != 0)
        asm volatile("pause");
    lock->cnt = -1;
}

void spinlockCntWriteRelease(SpinlockCnt* lock){
    if(lock->cnt != -1){
        printf((uint8_t*)"error", 20, 20, 0xebdbb2);
    }
    lock->cnt = 0;
}
