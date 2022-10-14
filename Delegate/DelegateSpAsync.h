#ifndef _DELEGATE_SP_ASYNC_H
#define _DELEGATE_SP_ASYNC_H

// Delegate.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Oct 2022.
//
// The DelegateMemberSpX delegate implemenations asynchronously bind and invoke class instance member functions. 
// The std::shared_ptr<TClass> is used in lieu of a raw TClass* pointer. 

#include "DelegateSp.h"
#include "IDelegateThread.h"
#include "DelegateInvoker.h"

namespace DelegateLib {

// Declare DelegateMemberSpAsync as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMemberSpAsync;

/// @brief Asynchronous member delegate that invokes the target function on the specified thread of control.
template <class TClass> 
class DelegateMemberSpAsync<void(TClass(void))> : public DelegateMemberSp<void(TClass(void))>, public IDelegateInvoker {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef void (TClass::*MemberFunc)();
	typedef void (TClass::*ConstMemberFunc)() const;
    using ClassType = DelegateMemberSpAsync<void(TClass(void))>;
    using BaseType = DelegateMemberSp<void(TClass(void))>;

	// Contructors take a class instance, member function, and delegate thread
	DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			BaseType::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()() override {
		if (m_thread == 0)
			BaseType::operator()();
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
	virtual void DelegateInvoke(DelegateMsgBase** msg) override {
		// Invoke the delegate function
		BaseType::operator()();

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
class DelegateMemberSpAsync<void(TClass(Param1))> : public DelegateMemberSp<void(TClass(Param1))>, public IDelegateInvoker {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1);
	typedef void (TClass::*ConstMemberFunc)(Param1) const;
    using ClassType = DelegateMemberSpAsync<void(TClass(Param1))>;
    using BaseType = DelegateMemberSp<void(TClass(Param1))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			BaseType::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1) override {
		if (m_thread == 0)
			BaseType::operator()(p1);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);

			// Create a clone instance of this delegate 
			auto delegate = Clone();

			// Create a new message instance 
			DelegateMsg1<Param1>* msg = new DelegateMsg1<Param1>(delegate, heapParam1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg1<Param1>* delegateMsg = static_cast<DelegateMsg1<Param1>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();

		// Invoke the delegate function
		BaseType::operator()(param1);

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
class DelegateMemberSpAsync<void(TClass(Param1, Param2))> : public DelegateMemberSp<void(TClass(Param1, Param2))>, public IDelegateInvoker {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2) const;
    using ClassType = DelegateMemberSpAsync<void(TClass(Param1, Param2))>;
    using BaseType = DelegateMemberSp<void(TClass(Param1, Param2))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func); }

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			BaseType::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2) override {
		if (m_thread == 0)
			BaseType::operator()(p1, p2);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);

			// Create a clone instance of this delegate 
			auto delegate = Clone();

			// Create a new message instance 
			DelegateMsg2<Param1, Param2>* msg = new DelegateMsg2<Param1, Param2>(delegate, heapParam1, heapParam2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg2<Param1, Param2>* delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();

		// Invoke the delegate function
		BaseType::operator()(param1, param2);

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
class DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3))> : public DelegateMemberSp<void(TClass(Param1, Param2, Param3))>, public IDelegateInvoker {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const;
    using ClassType = DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3))>;
    using BaseType = DelegateMemberSp<void(TClass(Param1, Param2, Param3))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func); }

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			BaseType::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3) override {
		if (m_thread == 0)
			BaseType::operator()(p1, p2, p3);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);

			// Create a clone instance of this delegate 
			auto delegate = Clone();

			// Create a new message instance 
			DelegateMsg3<Param1, Param2, Param3>* msg = new DelegateMsg3<Param1, Param2, Param3>(delegate, heapParam1, heapParam2, heapParam3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg3<Param1, Param2, Param3>* delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();

		// Invoke the delegate function
		BaseType::operator()(param1, param2, param3);

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
class DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4))> : public DelegateMemberSp<void (TClass(Param1, Param2, Param3, Param4))>, public IDelegateInvoker {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const;
    using ClassType = DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4))>;
    using BaseType = DelegateMemberSp<void(TClass(Param1, Param2, Param3, Param4))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			BaseType::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (m_thread == 0)
			BaseType::operator()(p1, p2, p3, p4);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);

			// Create a clone instance of this delegate 
			auto delegate = Clone();

			// Create a new message instance 
			DelegateMsg4<Param1, Param2, Param3, Param4>* msg = new DelegateMsg4<Param1, Param2, Param3, Param4>(delegate, heapParam1, heapParam2, heapParam3, heapParam4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg4<Param1, Param2, Param3, Param4>* delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();
		Param4 param4 = delegateMsg->GetParam4();

		// Invoke the delegate function
		BaseType::operator()(param1, param2, param3, param4);

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
class DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> : public DelegateMemberSp<void(TClass(Param1, Param2, Param3, Param4, Param5))>, public IDelegateInvoker {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const;
    using ClassType = DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>;
    using BaseType = DelegateMemberSp<void(TClass(Param1, Param2, Param3, Param4, Param5))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberSpAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) { Bind(object, func, thread); }
	DelegateMemberSpAsync() : m_thread(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(object, func); }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread && 
			BaseType::operator == (rhs); }

	/// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (m_thread == 0)
			BaseType::operator()(p1, p2, p3, p4, p5);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);
			Param5 heapParam5 = DelegateParam<Param5>::New(p5);

			// Create a clone instance of this delegate 
			auto delegate = Clone();

			// Create a new message instance 
			DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* msg = new DelegateMsg5<Param1, Param2, Param3, Param4, Param5>(delegate, heapParam1, heapParam2, heapParam3, heapParam4, heapParam5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(DelegateMsgBase** msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(*msg);

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();
		Param3 param3 = delegateMsg->GetParam3();
		Param4 param4 = delegateMsg->GetParam4();
		Param5 param5 = delegateMsg->GetParam5();

		// Invoke the delegate function
		BaseType::operator()(param1, param2, param3, param4, param5);

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

//N=0
template <class TClass>
DelegateMemberSpAsync<void(TClass(void))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(), DelegateThread* thread) { 
	return DelegateMemberSpAsync<void(TClass(void))>(object, func, thread);
}

template <class TClass>
DelegateMemberSpAsync<void(TClass(void))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)() const, DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(void))>(object, func, thread);
}

//N=1
template <class TClass, class Param1>
DelegateMemberSpAsync<void(TClass(Param1))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1), DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1))>(object, func, thread);
}

template <class TClass, class Param1>
DelegateMemberSpAsync<void(TClass(Param1))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1) const, DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1))>(object, func, thread);
}

//N=2
template <class TClass, class Param1, class Param2>
DelegateMemberSpAsync<void(TClass(Param1, Param2))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2), DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2))>(object, func, thread);
}

template <class TClass, class Param1, class Param2>
DelegateMemberSpAsync<void(TClass(Param1, Param2))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2) const, DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2))>(object, func, thread);
}

//N=3
template <class TClass, class Param1, class Param2, class Param3>
DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3), DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3))>(object, func, thread);
}

template <class TClass, class Param1, class Param2, class Param3>
DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3) const, DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3))>(object, func, thread);
}

//N=4
template <class TClass, class Param1, class Param2, class Param3, class Param4>
DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4))>(object, func, thread);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4>
DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const, DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4))>(object, func, thread);
}

//N=5
template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, thread);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(std::shared_ptr<TClass> object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const, DelegateThread* thread) {
	return DelegateMemberSpAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, thread);
}

}

#endif
