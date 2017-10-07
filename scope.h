// (c) 2006 Townsend Analytics, Ltd. All rights reserved.
//
// scope
//
// provides the class templates:
//   ScopedAcquire, ScopedReadAcquire, ScopedWriteAcquire: (RAII acquire/release)
//   ScopedRelease, ScopedReadRelease, ScopedWriteRelease: (RAII release/acquire)
// in the tal::threading namespace
//
#ifndef SCOPE_H_D6A81BC1_24A7_41D1_9F5C_4FF29CEC1FC8
#define SCOPE_H_D6A81BC1_24A7_41D1_9F5C_4FF29CEC1FC8

#include "no_copy.h"

namespace tal {
namespace threading {


	///////////////////////////////////
	// ScopedAcquire
	template< class Lockable >
	class ScopedAcquire : private NotCopyable
	{
	public:
		explicit ScopedAcquire( Lockable& l ) : l_(&l) { l_->lock(); }
		explicit ScopedAcquire( Lockable* l ) : l_(l) { l_->lock(); }
		~ScopedAcquire() { l_->unlock(); }
	private:
		Lockable* l_;
	};

	///////////////////////////////////
	// ScopedReadAcquire
	template< class Lockable >
	class ScopedReadAcquire : private NotCopyable
	{
	public:
		explicit ScopedReadAcquire( Lockable& l ) : l_(&l) { l_->lockread(); }
		explicit ScopedReadAcquire( Lockable* l ) : l_(l) { l_->lockread(); }
		~ScopedReadAcquire() { l_->unlockread(); }
	private:
		Lockable* l_;
	};

	///////////////////////////////////
	// ScopedWriteAcquire
	template< class Lockable >
	class ScopedWriteAcquire : private NotCopyable
	{
	public:
		explicit ScopedWriteAcquire( Lockable& l ) : l_(&l) { l_->lockwrite(); }
		explicit ScopedWriteAcquire( Lockable* l ) : l_(l) { l_->lockwrite(); }
		~ScopedWriteAcquire() { l_->unlockwrite(); }
	private:
		Lockable* l_;
	};



	///////////////////////////////////
	// ScopedRelease
	template< class Lockable >
	class ScopedRelease : private NotCopyable
	{
	public:
		explicit ScopedRelease( Lockable& l ) : l_(&l) { l_->unlock(); }
		explicit ScopedRelease( Lockable* l ) : l_(l) { l_->unlock(); }
		~ScopedRelease() { l_->lock(); }
	private:
		Lockable* l_;
	};

	///////////////////////////////////
	// ScopedReadRelease
	template< class Lockable >
	class ScopedReadRelease : private NotCopyable
	{
	public:
		explicit ScopedReadRelease( Lockable& l ) : l_(&l) { l_->unlockread(); }
		explicit ScopedReadRelease( Lockable* l ) : l_(l) { l_->unlockread(); }
		~ScopedReadRelease() { l_->lockread(); }
	private:
		Lockable* l_;
	};

	///////////////////////////////////
	// ScopedWriteRelease
	template< class Lockable >
	class ScopedWriteRelease : private NotCopyable
	{
	public:
		explicit ScopedWriteRelease( Lockable& l ) : l_(&l) { l_->unlockwrite(); }
		explicit ScopedWriteRelease( Lockable* l ) : l_(l) { l_->unlockwrite(); }
		~ScopedWriteRelease() { l_->lockwrite(); }
	private:
		Lockable* l_;
	};


} // namespace threading
} // namespace tal

#endif // #define SCOPE_H_D6A81BC1_24A7_41D1_9F5C_4FF29CEC1FC8
