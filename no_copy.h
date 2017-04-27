// (c) 2006 Townsend Analytics, Ltd. All rights reserved.
//
// no_copy
//
//  Provides the mix-in class NotCopyable in the tal namespace.
//  Inherit from this class to prevent your class from being copyable or assignable.
//
//  class NiftyObject : private NotCopyable {
//    // your class goes here, no need to define a copy ctor or assign oper
//  };
//
#ifndef NOT_COPYABLE_A89B94B2_8C8B_4174_B5D3_22D0774173D1
#define NOT_COPYABLE_A89B94B2_8C8B_4174_B5D3_22D0774173D1

namespace tal {

	class NotCopyable
	{
	protected:
		NotCopyable() {}
		virtual ~NotCopyable() {}
	private:
		NotCopyable( const NotCopyable& rhs );
		const NotCopyable& operator=( const NotCopyable& rhs );
	};

} // namespace tal


#endif // #define NOT_COPYABLE_A89B94B2_8C8B_4174_B5D3_22D0774173D1
