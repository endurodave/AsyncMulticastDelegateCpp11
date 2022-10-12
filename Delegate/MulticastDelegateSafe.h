#ifndef _MULTICAST_DELEGATE_SAFE_H
#define _MULTICAST_DELEGATE_SAFE_H

#include "MulticastDelegate.h"
#include "LockGuard.h"

namespace DelegateLib {

// Declare DelegateMemberSp as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class MulticastDelegateSafe;
 
/// @brief Thread-safe multicast delegate container class. May contain any delegate,
/// but typically used to hold DelegateMemberAsync<> or DelegateFreeAsync<> instances.
template<class RetType>
class MulticastDelegateSafe<RetType(void)> : public MulticastDelegate<void(void)>
{
public:
	MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
	~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

	void operator+=(const Delegate<void(void)>& delegate) { 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate::operator +=(delegate); 
	}
	void operator-=(const Delegate<void(void)>& delegate)	{ 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate::operator -=(delegate); 
	}
	void operator()() {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate::operator ()(); 
	}
	bool Empty() { 
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate::Empty();
	}
	void Clear() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate::Clear();
	}

	explicit operator bool() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegateBase::operator bool();
	}

private:
	// Prevent copying objects
	MulticastDelegateSafe(const MulticastDelegateSafe&);
	MulticastDelegateSafe& operator=(const MulticastDelegateSafe&);

	/// Lock to make the class thread-safe
	LOCK m_lock;
};

template<typename Param1>
class MulticastDelegateSafe<void(Param1)> : public MulticastDelegate<void(Param1)>
{
public:
	MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
	~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

	void operator+=(const Delegate<void(Param1)>& delegate) { 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1)>::operator +=(delegate); 
	}
	void operator-=(const Delegate<void(Param1)>& delegate)	{ 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1)>::operator -=(delegate); 
	}
	void operator()(Param1 p1) {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1)>::operator ()(p1); 
	}
	bool Empty() { 
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate<void(Param1)>::Empty();
	}
	void Clear() {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1)>::Clear();
	}

	explicit operator bool() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegateBase::operator bool();
	}

private:
	// Prevent copying objects
	MulticastDelegateSafe(const MulticastDelegateSafe&);
	MulticastDelegateSafe& operator=(const MulticastDelegateSafe&);

	/// Lock to make the class thread-safe
	LOCK m_lock;
};

template<typename Param1, typename Param2>
class MulticastDelegateSafe<void(Param1, Param2)> : public MulticastDelegate<void(Param1, Param2)>
{
public:
	MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
	~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

	void operator+=(const Delegate<void(Param1, Param2)>& delegate) { 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2)>::operator +=(delegate); 
	}
	void operator-=(const Delegate<void(Param1, Param2)>& delegate)	{ 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2)>::operator -=(delegate); 
	}
	void operator()(Param1 p1, Param2 p2) {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2)>::operator ()(p1, p2); 
	}
	bool Empty() { 
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate<void(Param1, Param2)>::Empty();
	}
	void Clear() {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2)>::Clear();
	}

	explicit operator bool() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegateBase::operator bool();
	}

private:
	// Prevent copying objects
	MulticastDelegateSafe(const MulticastDelegateSafe&);
	MulticastDelegateSafe& operator=(const MulticastDelegateSafe&);

	/// Lock to make the class thread-safe
	LOCK m_lock;
};

template<typename Param1, typename Param2, typename Param3>
class MulticastDelegateSafe<void(Param1, Param2, Param3)> : public MulticastDelegate<void(Param1, Param2, Param3)>
{
public:
	MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
	~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

	void operator+=(const Delegate<void(Param1, Param2, Param3)>& delegate) { 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3)>::operator +=(delegate); 
	}
	void operator-=(const Delegate<void(Param1, Param2, Param3)>& delegate)	{ 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3)>::operator -=(delegate); 
	}
	void operator()(Param1 p1, Param2 p2, Param3 p3) {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3)>::operator ()(p1, p2, p3); 
	}
	bool Empty() { 
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate<void(Param1, Param2, Param3)>::Empty();
	}
	void Clear() {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3)>::Clear();
	}

	explicit operator bool() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegateBase::operator bool();
	}

private:
	// Prevent copying objects
	MulticastDelegateSafe(const MulticastDelegateSafe&);
	MulticastDelegateSafe& operator=(const MulticastDelegateSafe&);

	/// Lock to make the class thread-safe
	LOCK m_lock;
};

template<typename Param1, typename Param2, typename Param3, typename Param4>
class MulticastDelegateSafe<void(Param1, Param2, Param3, Param4)> : public MulticastDelegate<void(Param1, Param2, Param3, Param4)>
{
public:
	MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
	~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

	void operator+=(const Delegate<void(Param1, Param2, Param3, Param4)>& delegate) { 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4)>::operator +=(delegate); 
	}
	void operator-=(const Delegate<void(Param1, Param2, Param3, Param4)>& delegate)	{ 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4)>::operator -=(delegate); 
	}
	void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4)>::operator ()(p1, p2, p3, p4); 
	}
	bool Empty() { 
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate<void(Param1, Param2, Param3, Param4)>::Empty();
	}
	void Clear() {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4)>::Clear();
	}

	explicit operator bool() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegateBase::operator bool();
	}

private:
	// Prevent copying objects
	MulticastDelegateSafe(const MulticastDelegateSafe&);
	MulticastDelegateSafe& operator=(const MulticastDelegateSafe&);

	/// Lock to make the class thread-safe
	LOCK m_lock;
};

template<typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
class MulticastDelegateSafe<void(Param1, Param2, Param3, Param4, Param5)> : public MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)>
{
public:
	MulticastDelegateSafe() { LockGuard::Create(&m_lock); }
	~MulticastDelegateSafe() { LockGuard::Destroy(&m_lock); }

	void operator+=(const Delegate<void(Param1, Param2, Param3, Param4, Param5)>& delegate) { 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)>::operator +=(delegate); 
	}
	void operator-=(const Delegate<void(Param1, Param2, Param3, Param4, Param5)>& delegate)	{ 
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)>::operator -=(delegate); 
	}
	void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)>::operator ()(p1, p2, p3, p4, p5); 
	}
	bool Empty() { 
		LockGuard lockGuard(&m_lock);
		return MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)>::Empty();
	}
	void Clear() {
		LockGuard lockGuard(&m_lock);
		MulticastDelegate<void(Param1, Param2, Param3, Param4, Param5)>::Clear();
	}

	explicit operator bool() {
		LockGuard lockGuard(&m_lock);
		return MulticastDelegateBase::operator bool();
	}

private:
	// Prevent copying objects
	MulticastDelegateSafe(const MulticastDelegateSafe&);
	MulticastDelegateSafe& operator=(const MulticastDelegateSafe&);

	/// Lock to make the class thread-safe
	LOCK m_lock;
};

}

#endif
