#ifndef _DELEGATE_ASYNC_WAIT_H
#define _DELEGATE_ASYNC_WAIT_H

// DelegateAsyncWait.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Oct 2022.

#include "Delegate.h"
#include "IDelegateThread.h"
#include "DelegateInvoker.h"
#include "Semaphore.h"
#include <memory>
#ifdef USE_CXX17
#include <optional>
#endif

/// @brief Asynchronous member delegate that invokes the target function on the specified thread of control
/// and waits for the function to be executed or a timeout occurs. Use IsSuccess() to determine if asynchronous 
/// call succeeded.

namespace DelegateLib {

const int WAIT_INFINITE = -1;

template <typename Signature>
class DelegateMemberAsyncWaitInvoke;

// N=0
template <class TClass, class RetType>
class DelegateMemberAsyncWaitInvoke<RetType(TClass(void))> {
public:
	virtual void operator()(DelegateMember<RetType(TClass(void))>* instance) {
		m_retVal = (*instance)();
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=0 void return
template <class TClass>
class DelegateMemberAsyncWaitInvoke<void(TClass(void))> {
public:
	virtual void operator()(DelegateMember<void(TClass(void))>* instance) {
		(*instance)();
	}

	void GetRetVal() { }
};

// N=1
template <class TClass, class RetType, class Param1>
class DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1))> {
public:
	virtual void operator()(DelegateMember<RetType(TClass(Param1))>* instance,
		Param1 p1) {
		m_retVal = (*instance)(p1);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=1 void return
template <class TClass, class Param1>
class DelegateMemberAsyncWaitInvoke<void(TClass(Param1))> {
public:
	virtual void operator()(DelegateMember<void(TClass(Param1))>* instance,
		Param1 p1) {
		(*instance)(p1);
	}

	void GetRetVal() { }
};

// N=2
template <class TClass, class RetType, class Param1, class Param2>
class DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2))> {
public:
	virtual void operator()(DelegateMember<RetType(TClass(Param1, Param2))>* instance,
		Param1 p1, Param2 p2) {
		m_retVal = (*instance)(p1, p2);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=2 void return
template <class TClass, class Param1, class Param2>
class DelegateMemberAsyncWaitInvoke<void(TClass(Param1, Param2))> {
public:
	virtual void operator()(DelegateMember<void(TClass(Param1, Param2))>* instance,
		Param1 p1, Param2 p2) {
		(*instance)(p1, p2);
	}

	void GetRetVal() { }
};

// N=3
template <class TClass, class RetType, class Param1, class Param2, class Param3>
class DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2, Param3))> {
public:
	virtual void operator()(DelegateMember<RetType(TClass(Param1, Param2, Param3))>* instance,
		Param1 p1, Param2 p2, Param3 p3) {
		m_retVal = (*instance)(p1, p2, p3);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=3 void return
template <class TClass, class Param1, class Param2, class Param3>
class DelegateMemberAsyncWaitInvoke<void(TClass(Param1, Param2, Param3))> {
public:
	virtual void operator()(DelegateMember<void(TClass(Param1, Param2, Param3))>* instance,
		Param1 p1, Param2 p2, Param3 p3) {
		(*instance)(p1, p2, p3);
	}

	void GetRetVal() { }
};

// N=4
template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4>
class DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2, Param3, Param4))> {
public:
	virtual void operator()(DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		m_retVal = (*instance)(p1, p2, p3, p4);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=4 void return
template <class TClass, class Param1, class Param2, class Param3, class Param4>
class DelegateMemberAsyncWaitInvoke<void(TClass(Param1, Param2, Param3, Param4))> {
public:
	virtual void operator()(DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		(*instance)(p1, p2, p3, p4);
	}

	void GetRetVal() { }
};

// N=5
template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> {
public:
	virtual void operator()(DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		m_retVal = (*instance)(p1, p2, p3, p4, p5);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=5 void return
template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateMemberAsyncWaitInvoke<void(TClass(Param1, Param2, Param3, Param4, Param5))> {
public:
	virtual void operator()(DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		(*instance)(p1, p2, p3, p4, p5);
	}

	void GetRetVal() { }
};

template <typename Signature>
class DelegateFreeAsyncWaitInvoke;

// N=0
template <class RetType>
class DelegateFreeAsyncWaitInvoke<RetType(void)> {
public:
	virtual void operator()(DelegateFree<RetType(void)>* instance) {
		m_retVal = (*instance)();
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=0 void return
template <>
class DelegateFreeAsyncWaitInvoke<void(void)> {
public:
	virtual void operator()(DelegateFree<void(void)>* instance) {
		(*instance)();
	}

	void GetRetVal() { }
};

// N=1
template <class RetType, class Param1>
class DelegateFreeAsyncWaitInvoke<RetType(Param1)> {
public:
	virtual void operator()(DelegateFree<RetType(Param1)>* instance,
		Param1 p1) {
		m_retVal = (*instance)(p1);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=1 void return
template <class Param1>
class DelegateFreeAsyncWaitInvoke<void(Param1)> {
public:
	virtual void operator()(DelegateFree<void(Param1)>* instance,
		Param1 p1) {
		(*instance)(p1);
	}

	void GetRetVal() { }
};

// N=2
template <class RetType, class Param1, class Param2>
class DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2)> {
public:
	virtual void operator()(DelegateFree<RetType(Param1, Param2)>* instance,
		Param1 p1, Param2 p2) {
		m_retVal = (*instance)(p1, p2);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=2 void return
template <class Param1, class Param2>
class DelegateFreeAsyncWaitInvoke<void(Param1, Param2)> {
public:
	virtual void operator()(DelegateFree<void(Param1, Param2)>* instance,
		Param1 p1, Param2 p2) {
		(*instance)(p1, p2);
	}

	void GetRetVal() { }
};

// N=3
template <class RetType, class Param1, class Param2, class Param3>
class DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2, Param3)> {
public:
	virtual void operator()(DelegateFree<RetType(Param1, Param2, Param3)>* instance,
		Param1 p1, Param2 p2, Param3 p3) {
		m_retVal = (*instance)(p1, p2, p3);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=3 void return
template <class Param1, class Param2, class Param3>
class DelegateFreeAsyncWaitInvoke<void(Param1, Param2, Param3)> {
public:
	virtual void operator()(DelegateFree<void(Param1, Param2, Param3)>* instance,
		Param1 p1, Param2 p2, Param3 p3) {
		(*instance)(p1, p2, p3);
	}

	void GetRetVal() { }
};

// N=4
template <class RetType, class Param1, class Param2, class Param3, class Param4>
class DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2, Param3, Param4)> {
public:
	virtual void operator()(DelegateFree<RetType(Param1, Param2, Param3, Param4)>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		m_retVal = (*instance)(p1, p2, p3, p4);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=4 void return
template <class Param1, class Param2, class Param3, class Param4>
class DelegateFreeAsyncWaitInvoke<void(Param1, Param2, Param3, Param4)> {
public:
	virtual void operator()(DelegateFree<void(Param1, Param2, Param3, Param4)>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		(*instance)(p1, p2, p3, p4);
	}

	void GetRetVal() { }
};

// N=5
template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2, Param3, Param4, Param5)> {
public:
	virtual void operator()(DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		m_retVal = (*instance)(p1, p2, p3, p4, p5);
	}

	RetType GetRetVal() { return m_retVal; }
private:
	RetType m_retVal;
};

// N=5 void return
template <class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateFreeAsyncWaitInvoke<void(Param1, Param2, Param3, Param4, Param5)> {
public:
	virtual void operator()(DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>* instance,
		Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		(*instance)(p1, p2, p3, p4, p5);
	}

	void GetRetVal() { }
};

// Declare DelegateMember as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMember;

// Declare DelegateMemberAsyncWait as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMemberAsyncWait;

// N=0 
template <class TClass, class RetType>
class DelegateMemberAsyncWait<RetType(TClass(void))> : 
	public DelegateMember<RetType(TClass(void))>, 
	public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)();
	typedef RetType (TClass::*ConstMemberFunc)() const;
    using ClassType = DelegateMemberAsyncWait<RetType(TClass(void))>;
    using BaseType = DelegateMember<RetType(TClass(void))>;
	
	DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait<RetType(TClass(void))>(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateMemberAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()() override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()();
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsgBase>(delegate);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke()
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()();
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()();
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateMemberAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateMemberAsyncWaitInvoke<RetType(TClass(void))> m_invoke;
};

// N=1  
template <class TClass, class RetType, class Param1>
class DelegateMemberAsyncWait<RetType(TClass(Param1))> : 
	public DelegateMember<RetType(TClass(Param1))>, 
	public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1);
	typedef RetType (TClass::*ConstMemberFunc)(Param1) const;
    using ClassType = DelegateMemberAsyncWait<RetType(TClass(Param1))>;
    using BaseType = DelegateMember<RetType(TClass(Param1))>;
	
	DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait<RetType(TClass(Param1))>(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateMemberAsyncWait() = delete;

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg1<Param1>>(delegate, p1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg1<Param1>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateMemberAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1))> m_invoke;
};

// N=2
template <class TClass, class RetType, class Param1, class Param2>
class DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))> : 
	public DelegateMember<RetType(TClass(Param1, Param2))>,
	public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2);
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2) const;
    using ClassType = DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))>;
    using BaseType = DelegateMember<RetType(TClass(Param1, Param2))>;
	
	DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))>(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateMemberAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg2<Param1, Param2>>(delegate, p1, p2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateMemberAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2))> m_invoke;
};

// N=3 
template <class TClass, class RetType, class Param1, class Param2, class Param3>
class DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))> : 
	public DelegateMember<RetType(TClass(Param1, Param2, Param3))>,
	public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3);
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const;
    using ClassType = DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))>;
    using BaseType = DelegateMember<RetType(TClass(Param1, Param2, Param3))>;
	
	DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))>(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateMemberAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2, p3);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg3<Param1, Param2, Param3>>(delegate, p1, p2, p3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2, Param3 p3)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2, p3);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2, p3);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();
		auto param3 = delegateMsg->GetParam3();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2, param3);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateMemberAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2, Param3))> m_invoke;
};

// N=4
template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4>
class DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))> : 
	public DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))>,
	public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3, Param4);
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const;
    using ClassType = DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))>;
    using BaseType = DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))>;
	
	DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))>(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateMemberAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2, p3, p4);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg4<Param1, Param2, Param3, Param4>>(delegate, p1, p2, p3, p4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2, p3, p4);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2, p3, p4);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();
		auto param3 = delegateMsg->GetParam3();
		auto param4 = delegateMsg->GetParam4();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2, param3, param4);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateMemberAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2, Param3, Param4))> m_invoke;
};

// N=5 
template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> : 
	public DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>, 
	public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5);
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const;
    using ClassType = DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>;
    using BaseType = DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>;
	
	DelegateMemberAsyncWait(ObjectPtr object, MemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread, int timeout) :
		BaseType(object, func), m_timeout(timeout) {
		Bind(object, func, thread);
	}
	DelegateMemberAsyncWait(const DelegateMemberAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateMemberAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(object, func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateMemberAsyncWait& operator=(const DelegateMemberAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2, p3, p4, p5);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>>(delegate, p1, p2, p3, p4, p5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2, p3, p4, p5);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2, p3, p4, p5);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();
		auto param3 = delegateMsg->GetParam3();
		auto param4 = delegateMsg->GetParam4();
		auto param5 = delegateMsg->GetParam5();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2, param3, param4, param5);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateMemberAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateMemberAsyncWaitInvoke<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> m_invoke;
};

// *** Free Classes Start ***

// Declare DelegateFree as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateFree;

// Declare DelegateFreeAsyncWait as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateFreeAsyncWait;

// N=0 
template <class RetType>
class DelegateFreeAsyncWait<RetType(void)> : 
	public DelegateFree<RetType(void)>,
	public IDelegateInvoker {
public:
	typedef RetType (*FreeFunc)();
    using ClassType = DelegateFreeAsyncWait<RetType(void)>;
    using BaseType = DelegateFree<RetType(void)>;

	DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) :
		BaseType(func), m_timeout(timeout) {
		Bind(func, thread);
	}
	DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateFreeAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()() override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()();
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsgBase>(delegate);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke()
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()();
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()();
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateFreeAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateFreeAsyncWaitInvoke<RetType(void)> m_invoke;
};

// N=1
template <class RetType, class Param1>
class DelegateFreeAsyncWait<RetType(Param1)> : 
	public DelegateFree<RetType(Param1)>,
	public IDelegateInvoker {
public:
	typedef RetType (*FreeFunc)(Param1);
    using ClassType = DelegateFreeAsyncWait<RetType(Param1)>;
    using BaseType = DelegateFree<RetType(Param1)>;

	DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) :
		BaseType(func), m_timeout(timeout) {
		Bind(func, thread);
	}
	DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateFreeAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg1<Param1>>(delegate, p1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg1<Param1>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateFreeAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateFreeAsyncWaitInvoke<RetType(Param1)> m_invoke;

};

// N=2 
template <class RetType, class Param1, class Param2>
class DelegateFreeAsyncWait<RetType(Param1, Param2)> : 
	public DelegateFree<RetType(Param1, Param2)>,
	public IDelegateInvoker {
public:
	typedef RetType (*FreeFunc)(Param1, Param2);
    using ClassType = DelegateFreeAsyncWait<RetType(Param1, Param2)>;
    using BaseType = DelegateFree<RetType(Param1, Param2)>;

	DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) :
		BaseType(func), m_timeout(timeout) {
		Bind(func, thread);
	}
	DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateFreeAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg2<Param1, Param2>>(delegate, p1, p2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateFreeAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2)> m_invoke;
};

// N=3
template <class RetType, class Param1, class Param2, class Param3>
class DelegateFreeAsyncWait<RetType(Param1, Param2, Param3)> : 
	public DelegateFree<RetType(Param1, Param2, Param3)>,
	public IDelegateInvoker {
public:
	typedef RetType (*FreeFunc)(Param1, Param2, Param3);
    using ClassType = DelegateFreeAsyncWait<RetType(Param1, Param2, Param3)>;
    using BaseType = DelegateFree<RetType(Param1, Param2, Param3)>;

	DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) :
		BaseType(func), m_timeout(timeout) {
		Bind(func, thread);
	}
	DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateFreeAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2, p3);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg3<Param1, Param2, Param3>>(delegate, p1, p2, p3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2, Param3 p3)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2, p3);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2, p3);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();
		auto param3 = delegateMsg->GetParam3();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2, param3);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateFreeAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2, Param3)> m_invoke;
};

// N=4
template <class RetType, class Param1, class Param2, class Param3, class Param4>
class DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4)> : 
	public DelegateFree<RetType(Param1, Param2, Param3, Param4)>,
	public IDelegateInvoker {
public:
	typedef RetType (*FreeFunc)(Param1, Param2, Param3, Param4);
    using ClassType = DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4)>;
    using BaseType = DelegateFree<RetType(Param1, Param2, Param3, Param4)>;

	DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) :
		BaseType(func), m_timeout(timeout) {
		Bind(func, thread);
	}
	DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateFreeAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2, p3, p4);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg4<Param1, Param2, Param3, Param4>>(delegate, p1, p2, p3, p4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2, p3, p4);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2, p3, p4);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();
		auto param3 = delegateMsg->GetParam3();
		auto param4 = delegateMsg->GetParam4();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2, param3, param4);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateFreeAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2, Param3, Param4)> m_invoke;
};

// N=5 
template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4, Param5)> : 
	public DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)>,
	public IDelegateInvoker {
public:
	typedef RetType (*FreeFunc)(Param1, Param2, Param3, Param4, Param5);
    using ClassType = DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4, Param5)>;
    using BaseType = DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)>;

	DelegateFreeAsyncWait(FreeFunc func, DelegateThread* thread, int timeout) :
		BaseType(func), m_timeout(timeout) {
		Bind(func, thread);
	}
	DelegateFreeAsyncWait(const DelegateFreeAsyncWait& rhs) : BaseType(rhs), m_sync(false) {
		Swap(rhs);
	}
	DelegateFreeAsyncWait() = delete;

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Bind a member function to a delegate. 
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread;
		BaseType::Bind(func);
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator==(rhs);
	}

	DelegateFreeAsyncWait& operator=(const DelegateFreeAsyncWait& rhs) {
		if (&rhs != this) {
			BaseType::operator=(rhs);
			Swap(rhs);
		}
		return *this;
	}

	/// Invoke delegate function asynchronously
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (this->m_thread == nullptr || m_sync)
			return BaseType::operator()(p1, p2, p3, p4, p5);
		else {
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());
			delegate->m_sema.Create();
			delegate->m_sema.Reset();

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>>(delegate, p1, p2, p3, p4, p5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			this->m_thread->DispatchDelegate(msg);

			// Wait for target thread to execute the delegate function
			if ((this->m_success = delegate->m_sema.Wait(this->m_timeout)))
				m_invoke = delegate->m_invoke;

			return m_invoke.GetRetVal();
		}
	}

#ifdef USE_CXX17
	/// Invoke delegate function asynchronously
	auto AsyncInvoke(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)
	{
		if constexpr (std::is_void<RetType>::value == true)
		{
			operator()(p1, p2, p3, p4, p5);
			return IsSuccess() ? std::optional<bool>(true) : std::optional<bool>();
		}
		else
		{
			auto retVal = operator()(p1, p2, p3, p4, p5);
			return IsSuccess() ? std::optional<RetType>(retVal) : std::optional<RetType>();
		}
	}
#endif

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		auto delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(msg.get());

		// Get the function parameter data
		auto param1 = delegateMsg->GetParam1();
		auto param2 = delegateMsg->GetParam2();
		auto param3 = delegateMsg->GetParam3();
		auto param4 = delegateMsg->GetParam4();
		auto param5 = delegateMsg->GetParam5();

		// Invoke the delegate function then signal the waiting thread
		m_sync = true;
		m_invoke(this, param1, param2, param3, param4, param5);
		this->m_sema.Signal();
	}

	/// Returns true if asynchronous function successfully invoked on target thread
	bool IsSuccess() { return m_success; }

	RetType GetRetVal() { return m_invoke.GetRetVal(); }

private:
	void Swap(const DelegateFreeAsyncWait& s) {
		m_thread = s.m_thread;
		m_timeout = s.m_timeout;
		m_success = s.m_success;
	}

	DelegateThread* m_thread = nullptr;		// Target thread to invoke the delegate function
	bool m_success = false;					// Set to true if async function succeeds
	int m_timeout = 0;						// Time in mS to wait for async function to invoke
	Semaphore m_sema;						// Semaphore to signal waiting thread
	bool m_sync = false;                    // Set true when synchronous invocation is required
	DelegateFreeAsyncWaitInvoke<RetType(Param1, Param2, Param3, Param4, Param5)> m_invoke;
};

//N=0
template <class TClass, class RetType>
DelegateMemberAsyncWait<RetType(TClass(void))> MakeDelegate(TClass* object, RetType (TClass::*func)(), DelegateThread* thread, int timeout) { 
	return DelegateMemberAsyncWait<RetType(TClass(void))>(object, func, thread, timeout);
}

template <class TClass, class RetType>
DelegateMemberAsyncWait<RetType(TClass(void))> MakeDelegate(TClass* object, RetType (TClass::*func)() const, DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(void))>(object, func, thread, timeout);
}

template <class RetType>
DelegateFreeAsyncWait<RetType(void)> MakeDelegate(RetType (*func)(), DelegateThread* thread, int timeout) { 
	return DelegateFreeAsyncWait<RetType(void)>(func, thread, timeout);
}

//N=1
template <class TClass, class Param1, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1), DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1))>(object, func, thread, timeout);
}

template <class TClass, class Param1, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1) const, DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1))>(object, func, thread, timeout);
}

template <class Param1, class RetType>
DelegateFreeAsyncWait<RetType(Param1)> MakeDelegate(RetType (*func)(Param1 p1), DelegateThread* thread, int timeout) {
	return DelegateFreeAsyncWait<RetType(Param1)>(func, thread, timeout);
}

//N=2
template <class TClass, class Param1, class Param2, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2), DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))>(object, func, thread, timeout);
}

template <class TClass, class Param1, class Param2, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2) const, DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2))>(object, func, thread, timeout);
}

template <class Param1, class Param2, class RetType>
DelegateFreeAsyncWait<RetType(Param1, Param2)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2), DelegateThread* thread, int timeout) {
	return DelegateFreeAsyncWait<RetType(Param1, Param2)>(func, thread, timeout);
}

//N=3
template <class TClass, class Param1, class Param2, class Param3, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3), DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))>(object, func, thread, timeout);
}

template <class TClass, class Param1, class Param2, class Param3, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3) const, DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3))>(object, func, thread, timeout);
}

template <class Param1, class Param2, class Param3, class RetType>
DelegateFreeAsyncWait<RetType(Param1, Param2, Param3)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2, Param3 p3), DelegateThread* thread, int timeout) {
	return DelegateFreeAsyncWait<RetType(Param1, Param2, Param3)>(func, thread, timeout);
}

//N=4
template <class TClass, class Param1, class Param2, class Param3, class Param4, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))>(object, func, thread, timeout);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const, DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4))>(object, func, thread, timeout);
}

template <class Param1, class Param2, class Param3, class Param4, class RetType>
DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateThread* thread, int timeout) {
	return DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4)>(func, thread, timeout);
}

//N=5
template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, thread, timeout);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const, DelegateThread* thread, int timeout) {
	return DelegateMemberAsyncWait<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, thread, timeout);
}

template <class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4, Param5)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateThread* thread, int timeout) {
	return DelegateFreeAsyncWait<RetType(Param1, Param2, Param3, Param4, Param5)>(func, thread, timeout);
}

} 

#endif
