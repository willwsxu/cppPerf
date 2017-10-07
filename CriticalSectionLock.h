// (c) 2006 Townsend Analytics, Ltd. All rights reserved.
//////////////////////////////////////////////////////////////////////////
// tal :: threading :: CriticalSectionLock
//
// CriticalSectionLock is a lock object that uses a CRITICAL_SECTION
//
// usage example:
//
//  class MyClass {
//    long m_val;
//    CriticalSectionLock m_csl;
//  public:
//    void add( val ) {
//      CriticalSectionLock::scope l( m_csl );
//      m_val += val;
//    }
//  };
//////////////////////////////////////////////////////////////////////////
#ifndef _CRITICALSECTIONLOCK_H_12CF1D68_806E_4350_A28D_5B02039306AC
#define _CRITICALSECTIONLOCK_H_12CF1D68_806E_4350_A28D_5B02039306AC


#ifndef LINUX
#include <WTypes.h>
#else
#include <rtix_common/win_base.h>
#include <pthread.h>
#endif
#include <exception>
#include "no_copy.h"
#include "scope.h"


namespace tal {
namespace threading {

/////////////////////////////////
// CriticalSectionLock
//  Reentrant - may lock recursively
//

#ifndef LINUX

class CriticalSectionLock : private NotCopyable
{
public:
	typedef ScopedAcquire<CriticalSectionLock> scope;
	typedef ScopedRelease<CriticalSectionLock> descope;

	//lint -e{715} - suppress formal parameter not referenced warning for dwSpinCount
	explicit CriticalSectionLock( DWORD dwSpinCount=0x1000 )
	{
#if _WIN32_WINNT >= 0x0403
		if ( !::InitializeCriticalSectionAndSpinCount( &m_cs, dwSpinCount ) ) {
			throw std::exception( "Error initializing critical section" );
		}
#else
		::InitializeCriticalSection( &m_cs );
#endif
	}

	~CriticalSectionLock()
	{
		::DeleteCriticalSection( &m_cs );
	}

	void lock()
	{
		::EnterCriticalSection( &m_cs );
	}
	void unlock()
	{
		::LeaveCriticalSection( &m_cs );
	}
	bool try_lock()
	{
		return (TRUE == ::TryEnterCriticalSection( &m_cs ));
	}

private:
	CRITICAL_SECTION m_cs;
};

#else

class CriticalSectionLock : private NotCopyable
{
public:
	typedef ScopedAcquire<CriticalSectionLock> scope;
	typedef ScopedRelease<CriticalSectionLock> descope;

	//lint -e{715} - suppress formal parameter not referenced warning for dwSpinCount
	explicit CriticalSectionLock( DWORD dwSpinCount=0x1000 )
	{
		pthread_mutexattr_t attr;
		if( 0 != pthread_mutexattr_init( &attr ) ) {
			throw CriticalSectionException();
		}

		// Set the mutex as a recursive mutex
		pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );

		if( 0 != pthread_mutex_init( &mutex_, &attr ) ) {
			throw CriticalSectionException();
		}

		pthread_mutexattr_destroy( &attr );
	}

	~CriticalSectionLock()
	{
		pthread_mutex_destroy( &mutex_ );
	}

	void lock()
	{
		pthread_mutex_lock( &mutex_ );
	}
	void unlock()
	{
		pthread_mutex_unlock( &mutex_ );
	}
	bool try_lock()
	{
		return 0 == pthread_mutex_trylock( &mutex_ );
	}

private:
	pthread_mutex_t mutex_;

private:
	class CriticalSectionException : public std::exception
	{
	public:
		virtual const char* what() const throw()
		{
			return "Error initializing critical section";
		}

	};
};
#endif

} // namespace threading
} // namespace tal

#endif // #define _CRITICALSECTIONLOCK_H_12CF1D68_806E_4350_A28D_5B02039306AC

