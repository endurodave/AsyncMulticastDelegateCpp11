#ifndef _MULTICAST_DELEGATE_H
#define _MULTICAST_DELEGATE_H

#include "MulticastDelegateBase.h"
#include "Delegate.h"

namespace DelegateLib {

/// @brief Multicast delegate container class. The class has a linked list of 
/// Delegate<> instances. When invoked, each Delegate instance within the invocation 
/// list is called. MulticastDelegate<> does support return values. A void return  
/// must always be used.
template <class Signature>
class MulticastDelegate : public MulticastDelegateBase
{
public:
	MulticastDelegate() = default;
	void operator()() {
		auto delegates = Delegates();
		for (DelegateBase* d : delegates) {
			auto delegate = static_cast<Delegate<void(void)>*>(d);
			(*delegate)();	// Invoke delegate callback
		}
	}
	void operator+=(const Delegate<void(void)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void(void)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
};

template<typename Param1>
class MulticastDelegate<void(Param1)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() = default;
	void operator()(Param1 p1) {
		auto delegates = Delegates();
		for (DelegateBase* d : delegates) {
			auto delegate = static_cast<Delegate<void(Param1)>*>(d);
			(*delegate)(p1);	// Invoke delegate callback
		}
	}
	void operator+=(const Delegate<void(Param1)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void(Param1)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
};

template<typename Param1, class Param2>
class MulticastDelegate<void(Param1, Param2)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() = default;
	void operator()(Param1 p1, Param2 p2) {
		auto delegates = Delegates();
		for (DelegateBase* d : delegates) {
			auto delegate = static_cast<Delegate<void(Param1, Param2)>*>(d);
			(*delegate)(p1, p2);	// Invoke delegate callback
		}
	}
	void operator+=(const Delegate<void (Param1, Param2)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
};

template<typename Param1, class Param2, class Param3>
class MulticastDelegate<void(Param1, Param2, Param3)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() = default;
	void operator()(Param1 p1, Param2 p2, Param3 p3) {
		auto delegates = Delegates();
		for (DelegateBase* d : delegates) {
			auto delegate = static_cast<Delegate<void(Param1, Param2, Param3)>*>(d);
			(*delegate)(p1, p2, p3);	// Invoke delegate callback
		}
	}
	void operator+=(const Delegate<void (Param1, Param2, Param3)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2, Param3)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
};

template<typename Param1, class Param2, class Param3, class Param4>
class MulticastDelegate<void(Param1, Param2, Param3, Param4)> : public MulticastDelegateBase
{
public:
    MulticastDelegate() = default;
	void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		auto delegates = Delegates();
		for (DelegateBase* d : delegates) {
			auto delegate = static_cast<Delegate<void(Param1, Param2, Param3, Param4)>*>(d);
			(*delegate)(p1, p2, p3, p4);	// Invoke delegate callback
		}
	}
	void operator+=(const Delegate<void (Param1, Param2, Param3, Param4)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2, Param3, Param4)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
};

template<typename Param1, class Param2, class Param3, class Param4, class Param5>
class MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)> : public MulticastDelegateBase
{
public:
    MulticastDelegate() = default;
	void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		auto delegates = Delegates();
		for (DelegateBase* d : delegates) {
			auto delegate = static_cast<Delegate<void(Param1, Param2, Param3, Param4, Param5)>*>(d);
			(*delegate)(p1, p2, p3, p4, p5);	// Invoke delegate callback
		}
	}
	void operator+=(const Delegate<void (Param1, Param2, Param3, Param4, Param5)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2, Param3, Param4, Param5)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate& operator=(const MulticastDelegate&) = delete;
};

}

#endif