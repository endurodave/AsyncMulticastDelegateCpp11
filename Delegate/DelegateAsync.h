#ifndef _DELEGATE_ASYNC_H
#define _DELEGATE_ASYNC_H

// Delegate.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Oct 2022.

#include "Delegate.h"
#include "DelegateThread.h"
#include "DelegateInvoker.h"
#if USE_XALLOCATOR
	#include <new>
#endif

namespace DelegateLib {

/// @brief Implements a new/delete for pass by value parameter values. Doesn't 
/// actually create memory as pass by value already has a full copy.
template <typename Param>
class DelegateParam
{
public:
	static Param New(Param param) {	return param; }
	static void Delete(Param param) { }
};

/// @brief Implement new/delete for pointer parameter values. If USE_ALLOCATOR is
/// defined, get memory from the fixed block allocator and not global heap.
template <typename Param>
class DelegateParam<Param *>
{
public:
	static Param* New(Param* param)	{
#if USE_XALLOCATOR
		void* mem = xmalloc(sizeof(*param));
		Param* newParam = new (mem) Param(*param);
#else
		Param* newParam = new Param(*param);
#endif
		return newParam;
	}

	static void Delete(Param* param) {
#if USE_XALLOCATOR
		param->~Param();
		xfree((void*)param);
#else
		delete param;
#endif
	}
};

/// @brief Implement new/delete for pointer to pointer parameter values. 
template <typename Param>
class DelegateParam<Param **>
{
public:
	static Param** New(Param** param) {
#if USE_XALLOCATOR
		void* mem = xmalloc(sizeof(*param));
		Param** newParam = new (mem) Param*();

		void* mem2 = xmalloc(sizeof(**param));
		*newParam = new (mem2) Param(**param);
#else
		Param** newParam = new Param*();
		*newParam = new Param(**param);
#endif
		return newParam;
	}

	static void Delete(Param** param) {
#if USE_XALLOCATOR
		(*param)->~Param();
		xfree((void*)(*param));

		xfree((void*)(param));
#else
		delete *param;
		delete param;
#endif
	}
};

/// @brief Implement new/delete for reference parameter values. 
template <typename Param>
class DelegateParam<Param &>
{
public:
	static Param& New(Param& param)	{
#if USE_XALLOCATOR
		void* mem = xmalloc(sizeof(param));
		Param* newParam = new (mem) Param(param);
#else
		Param* newParam = new Param(param);
#endif
		return *newParam;
	}

	static void Delete(Param& param) {
#if USE_XALLOCATOR
		(&param)->~Param();
		xfree((void*)(&param));
#else
		delete &param;
#endif
	}
};

// Declare DelegateMemberAsync as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMemberAsync;

/// @brief Asynchronous member delegate that invokes the target function on the specified thread of control.
template <class TClass> 
class DelegateMemberAsync<void(TClass(void))> : public DelegateMember<void(TClass(void))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)();
	typedef void (TClass::*ConstMemberFunc)() const;

	// Contructors take a class instance, member function, and delegate thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateMember<void(TClass(void))>::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread)	{
		m_thread = thread;
		DelegateMember<void(TClass(void))>::Bind(object, func); }

	virtual DelegateMemberAsync<void(TClass(void))>* Clone() const {
		return new DelegateMemberAsync<void(TClass(void))>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const 	{
		const DelegateMemberAsync<void(TClass(void))>* derivedRhs = dynamic_cast<const DelegateMemberAsync<void(TClass(void))>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			DelegateMember<void(TClass(void))>::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()() {
		if (m_thread == 0)
			DelegateMember<void(TClass(void))>::operator()();
		else
		{
			// Create a clone instance of this delegate 
			auto delegate = Clone();

			// Create a new message instance 
			DelegateMsgBase* msg = new DelegateMsgBase(delegate);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Invoke the delegate function
		DelegateMember<void(TClass(void))>::operator()();

		// Delete heap data created inside operator()
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread;
};

template <class TClass, class Param1> 
class DelegateMemberAsync<void(TClass(Param1))> : public DelegateMember<void(TClass(Param1))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1);
	typedef void (TClass::*ConstMemberFunc)(Param1) const;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateMember<void(TClass(Param1))>::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread)	{
		m_thread = thread;
		DelegateMember<void(TClass(Param1))>::Bind(object, func); }

	virtual DelegateMemberAsync<void(TClass(Param1))>* Clone() const {
		return new DelegateMemberAsync<void(TClass(Param1))>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const 	{
		const DelegateMemberAsync<void(TClass(Param1))>* derivedRhs = dynamic_cast<const DelegateMemberAsync<void(TClass(Param1))>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			DelegateMember<void(TClass(Param1))>::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1) {
		if (m_thread == 0)
			DelegateMember<void(TClass(Param1))>::operator()(p1);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);

			// Create a clone instance of this delegate 
			DelegateMemberAsync<void(TClass(Param1))>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg1<Param1>* msg = new DelegateMsg1<Param1>(delegate, heapParam1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg1<Param1>* delegateMsg = static_cast<DelegateMsg1<Param1>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();

		// Invoke the delegate function
		DelegateMember<void(TClass(Param1))>::operator()(param1);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread;
};

template <class TClass, class Param1, class Param2> 
class DelegateMemberAsync<void(TClass(Param1, Param2))> : public DelegateMember<void(TClass(Param1, Param2))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2) const;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateMember<void(TClass(Param1, Param2))>::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread)	{
		m_thread = thread;
		DelegateMember<void(TClass(Param1, Param2))>::Bind(object, func); }

	virtual DelegateMemberAsync<void(TClass(Param1, Param2))>* Clone() const {
		return new DelegateMemberAsync<void(TClass(Param1, Param2))>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const 	{
		const DelegateMemberAsync<void(TClass(Param1, Param2))>* derivedRhs = dynamic_cast<const DelegateMemberAsync<void(TClass(Param1, Param2))>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			DelegateMember<void(TClass(Param1, Param2))>::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2) {
		if (m_thread == 0)
			DelegateMember<void(TClass(Param1, Param2))>::operator()(p1, p2);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);

			// Create a clone instance of this delegate 
			DelegateMemberAsync<void(TClass(Param1, Param2))>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg2<Param1, Param2>* msg = new DelegateMsg2<Param1, Param2>(delegate, heapParam1, heapParam2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg2<Param1, Param2>* delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();

		// Invoke the delegate function
		DelegateMember<void(TClass(Param1, Param2))>::operator()(param1, param2);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread;
};

template <class TClass, class Param1, class Param2, class Param3> 
class DelegateMemberAsync<void(TClass(Param1, Param2, Param3))> : public DelegateMember<void(TClass(Param1, Param2, Param3))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateMember<void(TClass(Param1, Param2, Param3))>::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread)	{
		m_thread = thread;
		DelegateMember<void(TClass(Param1, Param2, Param3))>::Bind(object, func); }

	virtual DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>* Clone() const {
		return new DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const 	{
		const DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>* derivedRhs = dynamic_cast<const DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			DelegateMember<void(TClass(Param1, Param2, Param3))>::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3) {
		if (m_thread == 0)
			DelegateMember<void(TClass(Param1, Param2, Param3))>::operator()(p1, p2, p3);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);

			// Create a clone instance of this delegate 
			DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg3<Param1, Param2, Param3>* msg = new DelegateMsg3<Param1, Param2, Param3>(delegate, heapParam1, heapParam2, heapParam3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg3<Param1, Param2, Param3>* delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();

		// Invoke the delegate function
		DelegateMember<void(TClass(Param1, Param2, Param3))>::operator()(param1, param2, param3);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		DelegateParam<Param3>::Delete(param3);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread;
};

template <class TClass, class Param1, class Param2, class Param3, class Param4> 
class DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))> : public DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread)	{
		m_thread = thread;
		DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>::Bind(object, func); }

	virtual DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>* Clone() const {
		return new DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const 	{
		const DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>* derivedRhs = dynamic_cast<const DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		if (m_thread == 0)
			DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>::operator()(p1, p2, p3, p4);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);

			// Create a clone instance of this delegate 
			DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg4<Param1, Param2, Param3, Param4>* msg = new DelegateMsg4<Param1, Param2, Param3, Param4>(delegate, heapParam1, heapParam2, heapParam3, heapParam4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg4<Param1, Param2, Param3, Param4>* delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();
		Param4 param4 = delegateMsg->GetParam4();

		// Invoke the delegate function
		DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>::operator()(param1, param2, param3, param4);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		DelegateParam<Param3>::Delete(param3);
		DelegateParam<Param4>::Delete(param4);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread;
};

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> : public DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread)	{
		m_thread = thread;
		DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>::Bind(object, func); }

	virtual DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>* Clone() const {
		return new DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const 	{
		const DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>* derivedRhs = dynamic_cast<const DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		if (m_thread == 0)
			DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>::operator()(p1, p2, p3, p4, p5);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);
			Param5 heapParam5 = DelegateParam<Param5>::New(p5);

			// Create a clone instance of this delegate 
			DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* msg = new DelegateMsg5<Param1, Param2, Param3, Param4, Param5>(delegate, heapParam1, heapParam2, heapParam3, heapParam4, heapParam5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();
		Param4 param4 = delegateMsg->GetParam4();
		Param5 param5 = delegateMsg->GetParam5();

		// Invoke the delegate function
		DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>::operator()(param1, param2, param3, param4, param5);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		DelegateParam<Param3>::Delete(param3);
		DelegateParam<Param4>::Delete(param4);
		DelegateParam<Param5>::Delete(param5);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread;
};

/// @brief Asynchronous free delegate that invokes the target function on the specified thread of control.
template <class Signature>
class DelegateFreeAsync : public DelegateFree<void(void)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)();

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) { Bind(func, thread); }
	DelegateFreeAsync() : m_thread(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateFree<void(void)>::Bind(func);	}

	virtual DelegateFreeAsync* Clone() const {
		return new DelegateFreeAsync(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		const DelegateFreeAsync* derivedRhs = dynamic_cast<const DelegateFreeAsync*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			DelegateFree<void(void)>::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()() {
		if (m_thread == 0)
			DelegateFree<void(void)>::operator()();
		else
		{
			// Create a clone instance of this delegate 
			DelegateFreeAsync* delegate = Clone();

			// Create a new message instance 
			DelegateMsgBase* msg = new DelegateMsgBase(delegate);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Invoke the delegate function
		DelegateFree<void(void)>::operator()();

		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	DelegateThread* m_thread;
};

template <class Param1> 
class DelegateFreeAsync<void(Param1)> : public DelegateFree<void(Param1)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1);

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) { Bind(func, thread); }
	DelegateFreeAsync() : m_thread(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateFree<void(Param1)>::Bind(func);	}

	virtual DelegateFreeAsync<void(Param1)>* Clone() const {
		return new DelegateFreeAsync<void(Param1)>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		const DelegateFreeAsync<void(Param1)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<void(Param1)>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			DelegateFree<void(Param1)>::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1) {
		if (m_thread == 0)
			DelegateFree<void(Param1)>::operator()(p1);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);

			// Create a clone instance of this delegate 
			DelegateFreeAsync<void(Param1)>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg1<Param1>* msg = new DelegateMsg1<Param1>(delegate, heapParam1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg1<Param1>* delegateMsg = static_cast<DelegateMsg1<Param1>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();

		// Invoke the delegate function
		DelegateFree<void(Param1)>::operator()(param1);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	DelegateThread* m_thread;
};

template <class Param1, class Param2> 
class DelegateFreeAsync<void(Param1, Param2)> : public DelegateFree<void(Param1, Param2)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2);

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) { Bind(func, thread); }
	DelegateFreeAsync() : m_thread(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateFree<void(Param1, Param2)>::Bind(func);	}

	virtual DelegateFreeAsync<void(Param1, Param2)>* Clone() const {
		return new DelegateFreeAsync<void(Param1, Param2)>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		const DelegateFreeAsync<void(Param1, Param2)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<void(Param1, Param2)>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			DelegateFree<void(Param1, Param2)>::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2) {
		if (m_thread == 0)
			DelegateFree<void(Param1, Param2)>::operator()(p1, p2);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);

			// Create a clone instance of this delegate 
			DelegateFreeAsync<void(Param1, Param2)>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg2<Param1, Param2>* msg = new DelegateMsg2<Param1, Param2>(delegate, heapParam1, heapParam2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg2<Param1, Param2>* delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();

		// Invoke the delegate function
		DelegateFree<void(Param1, Param2)>::operator()(param1, param2);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	DelegateThread* m_thread;
};

template <class Param1, class Param2, class Param3> 
class DelegateFreeAsync<void(Param1, Param2, Param3)> : public DelegateFree<void(Param1, Param2, Param3)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2, Param3);

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) { Bind(func, thread); }
	DelegateFreeAsync() : m_thread(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateFree<void(Param1, Param2, Param3)>::Bind(func);	}

	virtual DelegateFreeAsync<void(Param1, Param2, Param3)>* Clone() const {
		return new DelegateFreeAsync<void(Param1, Param2, Param3)>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		const DelegateFreeAsync<void(Param1, Param2, Param3)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<void(Param1, Param2, Param3)>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			DelegateFree<void(Param1, Param2, Param3)>::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3) {
		if (m_thread == 0)
			DelegateFree<void(Param1, Param2, Param3)>::operator()(p1, p2, p3);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);

			// Create a clone instance of this delegate 
			DelegateFreeAsync<void(Param1, Param2, Param3)>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg3<Param1, Param2, Param3>* msg = new DelegateMsg3<Param1, Param2, Param3>(delegate, heapParam1, heapParam2, heapParam3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg3<Param1, Param2, Param3>* delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();

		// Invoke the delegate function
		DelegateFree<void(Param1, Param2, Param3)>::operator()(param1, param2, param3);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		DelegateParam<Param3>::Delete(param3);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	DelegateThread* m_thread;
};

template <class Param1, class Param2, class Param3, class Param4> 
class DelegateFreeAsync<void(Param1, Param2, Param3, Param4)> : public DelegateFree<void(Param1, Param2, Param3, Param4)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2, Param3, Param4);

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) { Bind(func, thread); }
	DelegateFreeAsync() : m_thread(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateFree<void(Param1, Param2, Param3, Param4)>::Bind(func);	}

	virtual DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>* Clone() const {
		return new DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		const DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			DelegateFree<void(Param1, Param2, Param3, Param4)>::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		if (m_thread == 0)
			DelegateFree<void(Param1, Param2, Param3, Param4)>::operator()(p1, p2, p3, p4);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);

			// Create a clone instance of this delegate 
			DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg4<Param1, Param2, Param3, Param4>* msg = new DelegateMsg4<Param1, Param2, Param3, Param4>(delegate, heapParam1, heapParam2, heapParam3, heapParam4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg4<Param1, Param2, Param3, Param4>* delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();
		Param4 param4 = delegateMsg->GetParam4();

		// Invoke the delegate function
		DelegateFree<void(Param1, Param2, Param3, Param4)>::operator()(param1, param2, param3, param4);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		DelegateParam<Param3>::Delete(param3);
		DelegateParam<Param4>::Delete(param4);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	DelegateThread* m_thread;
};

template <class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)> : public DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2, Param3, Param4, Param5);

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) { Bind(func, thread); }
	DelegateFreeAsync() : m_thread(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>::Bind(func);	}

	virtual DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>* Clone() const {
		return new DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		const DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>* derivedRhs = dynamic_cast<const DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		if (m_thread == 0)
			DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>::operator()(p1, p2, p3, p4, p5);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);
			Param5 heapParam5 = DelegateParam<Param5>::New(p5);

			// Create a clone instance of this delegate 
			DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>* delegate = Clone();

			// Create a new message instance 
			DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* msg = new DelegateMsg5<Param1, Param2, Param3, Param4, Param5>(delegate, heapParam1, heapParam2, heapParam3, heapParam4, heapParam5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(DelegateMsgBase** msg) {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();
		Param4 param4 = delegateMsg->GetParam4();
		Param5 param5 = delegateMsg->GetParam5();

		// Invoke the delegate function
		DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>::operator()(param1, param2, param3, param4, param5);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
		DelegateParam<Param3>::Delete(param3);
		DelegateParam<Param4>::Delete(param4);
		DelegateParam<Param5>::Delete(param5);
		delete *msg;
		*msg = 0;

		// Do this last before returning!
		delete this;
	}

private:
	DelegateThread* m_thread;
};

//N=0
template <class TClass>
DelegateMemberAsync<void(TClass(void))> MakeDelegate(TClass* object, void (TClass::*func)(), DelegateThread* thread) { 
	return DelegateMemberAsync<void(TClass(void))>(object, func, thread);
}

template <class TClass>
DelegateMemberAsync<void(TClass(void))> MakeDelegate(TClass* object, void (TClass::*func)() const, DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(void))>(object, func, thread);
}

DelegateFreeAsync<void(void)> MakeDelegate(void(*func)(void), DelegateThread* thread);

//N=1
template <class TClass, class Param1>
DelegateMemberAsync<void(TClass(Param1))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1), DelegateThread* thread) { 
	return DelegateMemberAsync<void(TClass(Param1))>(object, func, thread);
}

template <class TClass, class Param1>
DelegateMemberAsync<void(TClass(Param1))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1) const, DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1))>(object, func, thread);
}

template <class Param1>
DelegateFreeAsync<void(Param1)> MakeDelegate(void (*func)(Param1 p1), DelegateThread* thread) {
	return DelegateFreeAsync<void(Param1)>(func, thread);
}

//N=2
template <class TClass, class Param1, class Param2>
DelegateMemberAsync<void(TClass(Param1, Param2))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2), DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2))>(object, func, thread);
}

template <class TClass, class Param1, class Param2>
DelegateMemberAsync<void(TClass(Param1, Param2))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2) const, DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2))>(object, func, thread);
}

template <class Param1, class Param2>
DelegateFreeAsync<void(Param1, Param2)> MakeDelegate(void (*func)(Param1 p1, Param2 p2), DelegateThread* thread) {
	return DelegateFreeAsync<void(Param1, Param2)>(func, thread);
}

//N=3
template <class TClass, class Param1, class Param2, class Param3>
DelegateMemberAsync<void(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3), DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>(object, func, thread);
}

template <class TClass, class Param1, class Param2, class Param3>
DelegateMemberAsync<void(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3) const, DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>(object, func, thread);
}

template <class Param1, class Param2, class Param3>
DelegateFreeAsync<void(Param1, Param2, Param3)> MakeDelegate(void (*func)(Param1 p1, Param2 p2, Param3 p3), DelegateThread* thread) {
	return DelegateFreeAsync<void(Param1, Param2, Param3)>(func, thread);
}

//N=4
template <class TClass, class Param1, class Param2, class Param3, class Param4>
DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>(object, func, thread);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4>
DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const, DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>(object, func, thread);
}

template <class Param1, class Param2, class Param3, class Param4>
DelegateFreeAsync<void(Param1, Param2, Param3, Param4)> MakeDelegate(void (*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateThread* thread) {
	return DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>(func, thread);
}

//N=5
template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, thread);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const, DelegateThread* thread) {
	return DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, thread);
}

template <class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)> MakeDelegate(void (*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateThread* thread) {
	return DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>(func, thread);
}

}

#endif
