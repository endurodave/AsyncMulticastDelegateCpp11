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
	MulticastDelegate() { }
	void operator()() {
		InvocationNode* node = GetInvocationHead();
		while (node != 0) {
			auto delegate = static_cast<Delegate<void(void)>*>(node->Delegate);
			(*delegate)();	// Invoke delegate callback
			node = node->Next;
		}
	}
	void operator+=(const Delegate<void(void)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void(void)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&);
	MulticastDelegate& operator=(const MulticastDelegate&);
};

template<typename Param1>
class MulticastDelegate<void(Param1)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() { }
	void operator()(Param1 p1) {
		InvocationNode* node = GetInvocationHead();
		while (node != 0) {
			auto delegate = static_cast<Delegate<void(Param1)>*>(node->Delegate);
			(*delegate)(p1);	// Invoke delegate callback
			node = node->Next;
		}
	}
	void operator+=(const Delegate<void(Param1)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void(Param1)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&);
	MulticastDelegate& operator=(const MulticastDelegate&);
};

template<typename Param1, class Param2>
class MulticastDelegate<void(Param1, Param2)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() { }
	void operator()(Param1 p1, Param2 p2) {
		InvocationNode* node = GetInvocationHead();
		while (node != 0) {
			auto delegate = static_cast<Delegate<void(Param1, Param2)>*>(node->Delegate);
			(*delegate)(p1, p2);	// Invoke delegate callback
			node = node->Next;
		}
	}
	void operator+=(const Delegate<void (Param1, Param2)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&);
	MulticastDelegate& operator=(const MulticastDelegate&);
};

template<typename Param1, class Param2, class Param3>
class MulticastDelegate<void(Param1, Param2, Param3)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() { }
	void operator()(Param1 p1, Param2 p2, Param3 p3) {
		InvocationNode* node = GetInvocationHead();
		while (node != 0) {
			auto delegate = static_cast<Delegate<void(Param1, Param2, Param3)>*>(node->Delegate);
			(*delegate)(p1, p2, p3);	// Invoke delegate callback
			node = node->Next;
		}
	}
	void operator+=(const Delegate<void (Param1, Param2, Param3)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2, Param3)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&);
	MulticastDelegate& operator=(const MulticastDelegate&);
};

template<typename Param1, class Param2, class Param3, class Param4>
class MulticastDelegate<void(Param1, Param2, Param3, Param4)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() { }
	void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		InvocationNode* node = GetInvocationHead();
		while (node != 0) {
			auto delegate = static_cast<Delegate<void(Param1, Param2, Param3, Param4)>*>(node->Delegate);
			(*delegate)(p1, p2, p3, p4);	// Invoke delegate callback
			node = node->Next;
		}
	}
	void operator+=(const Delegate<void (Param1, Param2, Param3, Param4)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2, Param3, Param4)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&);
	MulticastDelegate& operator=(const MulticastDelegate&);
};

template<typename Param1, class Param2, class Param3, class Param4, class Param5>
class MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)> : public MulticastDelegateBase
{
public:
	MulticastDelegate() { }
	void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		InvocationNode* node = GetInvocationHead();
		while (node != 0) {
			auto delegate = static_cast<Delegate<void(Param1, Param2, Param3, Param4, Param5)>*>(node->Delegate);
			(*delegate)(p1, p2, p3, p4, p5);	// Invoke delegate callback
			node = node->Next;
		}
	}
	void operator+=(const Delegate<void (Param1, Param2, Param3, Param4, Param5)>& delegate) { MulticastDelegateBase::operator+=(delegate); }
	void operator-=(const Delegate<void (Param1, Param2, Param3, Param4, Param5)>& delegate) { MulticastDelegateBase::operator-=(delegate); }

private:
	// Prevent copying objects
	MulticastDelegate(const MulticastDelegate&);
	MulticastDelegate& operator=(const MulticastDelegate&);
};

}

#endif