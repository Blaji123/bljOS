#ifndef __BLJOS__SPINLOCK_H
#define __BLJOS__SPINLOCK_H

#include <common/types.h>

namespace bljOS{

    typedef enum {
        memory_order_relaxed,   // No synchronization or ordering constraints
        memory_order_consume,   // Consume ordering (data dependency ordering)
        memory_order_acquire,   // Acquire ordering (ensures that subsequent reads/writes are not reordered before this operation)
        memory_order_release,   // Release ordering (ensures that previous reads/writes are not reordered after this operation)
        memory_order_acq_rel,   // Acquire-Release ordering (both acquire and release constraints)
        memory_order_seq_cst    // Sequential consistency (strongest ordering guarantees)
    } memory_order;

    typedef struct{
        volatile int flag;
    }atomic_flag;

    inline int atomic_flag_test_and_set_explicit(atomic_flag* lock, memory_order memory_order1){
        int old_value = 1;

        __asm__ __volatile__(
            "lock xchg %0, %1"
            : "=r" (old_value), "=m" (lock->flag)
            : "0" (old_value), "m" (lock->flag)
            : "memory"
        );

        return old_value;
    }

    inline void atomic_flag_clear_explicit(atomic_flag* lock, memory_order memory_order1){
        __asm__ __volatile__(
            "movl $0, %0"
            : "=m" (lock->flag)
            :
            : "memory"
        );
    }

    typedef atomic_flag Spinlock;

    void spinlockAcquire(Spinlock* lock);
    void spinlockRelease(Spinlock* lock);
    void spinlockWait(Spinlock* lock);

    typedef struct{
        bljOS::common::int64_t cnt;
    }SpinlockCnt;

    void spinlockCntReadAcquire(SpinlockCnt* lock);
    void spinlockCntReadRelease(SpinlockCnt* lock);
    void spinlockCntWriteAcquire(SpinlockCnt* lock);
    void spinlockCntWriteRelease(SpinlockCnt* lock);
}

#endif
