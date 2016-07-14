#ifndef RWLOCK_RWLOCK_H_INCLUDED
#define RWLOCK_RWLOCK_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

    struct tagRWLock;
    typedef struct tagRWLock RWLock;

    extern RWLock* RWLockCreate();
    extern void RWLockDestroy(RWLock*);

    extern int RWLockReadLock(RWLock*);
    extern int RWLockReadUnlock(RWLock*);

    extern int RWLockWriteLock(RWLock*);
    extern int RWLockWriteUnlock(RWLock*);

    extern char const* RWLockVendor();

#ifdef __cplusplus
}
#endif

#endif
