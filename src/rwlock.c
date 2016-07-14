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

#define RWLOCK_VENDOR_PTHREAD 0
#define RWLOCK_VENDOR_WINDOWS_SRW 1
#define RWLOCK_VENDOR_WINDOWS_SIM 2

#if defined(HAVE_PTHREAD_H) && defined(HAVE_LIBPTHREAD)
#   include <pthread.h>
#   define RWLOCK_VENDOR RWLOCK_VENDOR_PTHREAD
#elif defined(_MSC_VER)
#   include <windows.h>
#   if  (defined(WINVER) && WINVER >= 0x601) || (defined(_WIN32_WINNT) && _WIN32_WINNT >= 0x601)
#       define RWLOCK_VENDOR RWLOCK_VENDOR_WINDOWS_SRW
#       if (defined(WINVER) && WINVER > 0x601) || (defined(_WIN32_WINNT) && _WIN32_WINNT > 0x601)
#           include <synchapi.h>
#       endif
#   else
#       define RWLOCK_VENDOR RWLOCK_VENDOR_WINDOWS_SIM
#   endif
#elif defined(__CYGWIN__) || defined(__MINGW32__) || defined(__MINGW64__)
#   define RWLOCK_VENDOR RWLOCK_VENDOR_WINDOWS_SIM
#   include <windows.h>
#else
#   error Can not determine which platform this is.
#endif

struct tagRWLock {
#if RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SRW
	SRWLOCK _M_locker;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_PTHREAD
	pthread_rwlock_t _M_locker;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SIM
#endif
};

RWLock* RWLockCreate() {
	RWLock *lock = NULL;
	lock = (RWLock*) malloc(sizeof(RWLock));
	if (!lock)
		return lock;
#if RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SRW
	InitializeSRWLock(&lock->_M_locker);
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_PTHREAD
	if (pthread_rwlock_init(&lock->_M_locker, NULL) == 0)
		return lock;
	free(lock);
	lock = NULL;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SIM
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
#if RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SRW
	AcquireSRWLockShared(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_PTHREAD
	if (pthread_rwlock_rdlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SIM
	// TODO
	return 0;
#endif
}

int RWLockReadUnlock(RWLock *lock) {
#if RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SRW
	ReleaseSRWLockShared(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_PTHREAD
	if (pthread_rwlock_unlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SIM
	// TODO
	return 0;
#endif
}

int RWLockWriteLock(RWLock *lock) {
#if RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SRW
	AcquireSRWLockExclusive(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_PTHREAD
	if (pthread_rwlock_wrlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SIM
	// TODO
	return 0;
#endif
}

int RWLockWriteUnlock(RWLock *lock) {
#if RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SRW
	ReleaseSRWLockExclusive(&lock->_M_locker);
	return 1;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_PTHREAD
	if (pthread_rwlock_unlock(&lock->_M_locker) == 0)
		return 1;
	return 0;
#elif RWLOCK_VENDOR == RWLOCK_VENDOR_WINDOWS_SIM
	// TODO
	return 0;
#endif
}

char const* RWLockVendor() {
    return RWLOCK_VENDORS[RWLOCK_VENDOR];
}
