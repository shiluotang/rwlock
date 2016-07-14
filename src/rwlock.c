#include "rwlock.h"

#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#   include <config.h>
#endif

static char const* const RWLOCK_VENDORS[] = {
    "PThread Reader/Writer Lock",
    "Windows Slim Reader/Writer Lock",
    "Windows Software Simulation"
};

#define PTHREAD_RWLOCK_VENDOR 0
#define WINDOWS_SRW_RWLOCK_VENDOR 1
#define WINDOWS_SIM_RWLOCK_VENDOR 2

#ifdef HAVE_PTHREAD_H
#   include <pthread.h>
#   define RWLOCK_VENDOR PTHREAD_RWLOCK_VENDOR
#elif defined(_MSC_VER)
#   include <windows.h>
#   if  (defined(WINVER) && WINVER >= 0x601) || (defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x601)
#       define RWLOCK_VENDOR WINDOWS_SRW_RWLOCK_VENDOR
#       if (defined(WINVER) && WINVER > 0x601) || (defined(_WIN32_WINNT) && _WIN32_WINNT > 0x601)
#           include <synchapi.h>
#       endif
#   else
#       define RWLOCK_VENDOR WINDOWS_SIM_RWLOCK_VENDOR
#   endif
#elif defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
#   define RWLOCK_VENDOR WINDOWS_SIM_RWLOCK_VENDOR
#   include <windows.h>
#else
#   error Can not determine which platform this is.
#endif

struct tagRWLock {
#if RWLOCK_VENDOR == WINDOWS_SRW_RWLOCK_VENDOR
	SRWLOCK _M_locker;
#elif RWLOCK_VENDOR == PTHREAD_RWLOCK_VENDOR
	pthread_rwlock_t _M_locker;
#elif RWLOCK_VENDOR == WINDOWS_SIM_RWLOCK_VENDOR
#endif
};

RWLock* RWLockCreate() {
	RWLock *lock = NULL;
	lock = (RWLock*) malloc(sizeof(RWLock));
	if (!lock)
		return lock;
#if RWLOCK_VENDOR == WINDOWS_SRW_RWLOCK_VENDOR
	InitializeSRWLock(&lock->_M_locker);
#elif RWLOCK_VENDOR == PTHREAD_RWLOCK_VENDOR
	if (pthread_rwlock_init(&lock->_M_locker, NULL) == 0)
		return lock;
	free(lock);
	lock = NULL;
#elif RWLOCK_VENDOR == WINDOWS_SIM_RWLOCK_VENDOR
	// TODO
#endif
    return lock;
}

void RWLockDestroy(RWLock *lock) {
	if (lock) {
		free(lock);
		// FIXME Useless statement!
		lock = NULL;
	}
}

int RWLockReadLock(RWLock *lock) {
#if RWLOCK_VENDOR == WINDOWS_SRW_RWLOCK_VENDOR
	AcquireSRWLockShared(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == PTHREAD_RWLOCK_VENDOR
	if (pthread_rwlock_rdlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == WINDOWS_SIM_RWLOCK_VENDOR
	// TODO
	return 0;
#endif
}

int RWLockReadUnlock(RWLock *lock) {
#if RWLOCK_VENDOR == WINDOWS_SRW_RWLOCK_VENDOR
	ReleaseSRWLockShared(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == PTHREAD_RWLOCK_VENDOR
	if (pthread_rwlock_unlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == WINDOWS_SIM_RWLOCK_VENDOR
	// TODO
	return 0;
#endif
}

int RWLockWriteLock(RWLock *lock) {
#if RWLOCK_VENDOR == WINDOWS_SRW_RWLOCK_VENDOR
	AcquireSRWLockExclusive(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == PTHREAD_RWLOCK_VENDOR
	if (pthread_rwlock_wrlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == WINDOWS_SIM_RWLOCK_VENDOR
	// TODO
	return 0;
#endif
}

int RWLockWriteUnlock(RWLock *lock) {
#if RWLOCK_VENDOR == WINDOWS_SRW_RWLOCK_VENDOR
	ReleaseSRWLockExclusive(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == PTHREAD_RWLOCK_VENDOR
	if (pthread_rwlock_unlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == WINDOWS_SIM_RWLOCK_VENDOR
	// TODO
	return 0;
#endif
}

char const* RWLockVendor() {
    return RWLOCK_VENDORS[RWLOCK_VENDOR];
}
