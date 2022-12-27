#ifndef _DELEGATE_ASYNC_H
#define _DELEGATE_ASYNC_H

// Delegate.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Oct 2022.

#include "Delegate.h"
#include "IDelegateThread.h"
#include "DelegateInvoker.h"
#include <memory>
#include <type_traits>
#if USE_XALLOCATOR
	#include <new>
#endif

namespace DelegateLib {

// std::shared_ptr reference arguments are not allowed with asynchronous delegates as the behavior is 
// undefined. In other words:
// void MyFunc(std::shared_ptr<T> data)		// Ok!
// void MyFunc(std::shared_ptr<T>& data)	// Error if DelegateAsync or DelegateSpAsync target!
template<class T>
struct is_shared_ptr : std::false_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>&> : std::true_type {};

template<class T>
struct is_shared_ptr<const std::shared_ptr<T>&> : std::true_type {};

template<class T>
struct is_shared_ptr<std::shared_ptr<T>*> : std::true_type {};

template<class T>
struct is_shared_ptr<const std::shared_ptr<T>*> : std::true_type {};

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
    using ClassType = DelegateMemberAsync<void(TClass(void))>;
    using BaseType = DelegateMember<void(TClass(void))>;

	// Contructors take a class instance, member function, and delegate thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync() = delete;

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
	virtual void operator()() override {
		if (m_thread == nullptr)
			BaseType::operator()();
		else
		{
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsgBase>(delegate);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Invoke the delegate function
		BaseType::operator()();
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread = nullptr;
};

template <class TClass, class Param1> 
class DelegateMemberAsync<void(TClass(Param1))> : public DelegateMember<void(TClass(Param1))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1);
	typedef void (TClass::*ConstMemberFunc)(Param1) const;
    using ClassType = DelegateMemberAsync<void(TClass(Param1))>;
    using BaseType = DelegateMember<void(TClass(Param1))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync() = delete;

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
	virtual void operator()(Param1 p1) override {
		if (m_thread == nullptr)
			BaseType::operator()(p1);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg1<Param1>>(delegate, heapParam1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>))),
			"std::shared_ptr reference argument not allowed");
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg1<Param1>* delegateMsg = static_cast<DelegateMsg1<Param1>*>(msg.get());

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();

		// Invoke the delegate function
		BaseType::operator()(param1);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread = nullptr;
};

template <class TClass, class Param1, class Param2> 
class DelegateMemberAsync<void(TClass(Param1, Param2))> : public DelegateMember<void(TClass(Param1, Param2))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2) const;
    using ClassType = DelegateMemberAsync<void(TClass(Param1, Param2))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync() = delete;

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
		if (m_thread == nullptr)
			BaseType::operator()(p1, p2);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg2<Param1, Param2>>(delegate, heapParam1, heapParam2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>))),
			"std::shared_ptr reference argument not allowed");
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg2<Param1, Param2>* delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(msg.get());

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();

		// Invoke the delegate function
		BaseType::operator()(param1, param2);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread = nullptr;
};

template <class TClass, class Param1, class Param2, class Param3> 
class DelegateMemberAsync<void(TClass(Param1, Param2, Param3))> : public DelegateMember<void(TClass(Param1, Param2, Param3))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const;
    using ClassType = DelegateMemberAsync<void(TClass(Param1, Param2, Param3))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2, Param3))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync() = delete;

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
		if (m_thread == nullptr)
			BaseType::operator()(p1, p2, p3);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg3<Param1, Param2, Param3>>(delegate, heapParam1, heapParam2, heapParam3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>)) ||
			(is_shared_ptr<Param3>::value && (std::is_lvalue_reference_v<Param3> || std::is_pointer_v<Param3>))),
			"std::shared_ptr reference argument not allowed");
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg3<Param1, Param2, Param3>* delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(msg.get());

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
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread = nullptr;
};

template <class TClass, class Param1, class Param2, class Param3, class Param4> 
class DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))> : public DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const;
    using ClassType = DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync() = delete;

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
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (m_thread == nullptr)
			BaseType::operator()(p1, p2, p3, p4);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg4<Param1, Param2, Param3, Param4>>(delegate, heapParam1, heapParam2, heapParam3, heapParam4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>)) ||
			(is_shared_ptr<Param3>::value && (std::is_lvalue_reference_v<Param3> || std::is_pointer_v<Param3>)) ||
			(is_shared_ptr<Param4>::value && (std::is_lvalue_reference_v<Param4> || std::is_pointer_v<Param4>))),
			"std::shared_ptr reference argument not allowed");
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg4<Param1, Param2, Param3, Param4>* delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(msg.get());

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
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread = nullptr;
};

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))> : public DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>, public IDelegateInvoker {
public:
	typedef TClass* ObjectPtr;
	typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5);
	typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const;
    using ClassType = DelegateMemberAsync<void(TClass(Param1, Param2, Param3, Param4, Param5))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>;

	// Contructors take a class instance, member function, and callback thread
	DelegateMemberAsync(ObjectPtr object, MemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync(ObjectPtr object, ConstMemberFunc func, DelegateThread* thread) : BaseType(object, func) { Bind(object, func, thread); }
	DelegateMemberAsync() = delete;

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
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (m_thread == nullptr)
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
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>>(delegate, heapParam1, heapParam2, heapParam3, heapParam4, heapParam5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>)) ||
			(is_shared_ptr<Param3>::value && (std::is_lvalue_reference_v<Param3> || std::is_pointer_v<Param3>)) ||
			(is_shared_ptr<Param4>::value && (std::is_lvalue_reference_v<Param4> || std::is_pointer_v<Param4>)) ||
			(is_shared_ptr<Param5>::value && (std::is_lvalue_reference_v<Param5> || std::is_pointer_v<Param5>))),
			"std::shared_ptr reference argument not allowed");
	}

	/// Called by the target thread to invoke the delegate function 
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(msg.get());

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
	}

private:
	/// Target thread to invoke the delegate function
	DelegateThread* m_thread = nullptr;
};

/// @brief Asynchronous free delegate that invokes the target function on the specified thread of control.
template <class Signature>
class DelegateFreeAsync : public DelegateFree<void(void)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)();
    using ClassType = DelegateFreeAsync<void(void)>;
    using BaseType = DelegateFree<void(void)>;

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : BaseType(func) { Bind(func, thread); }
	DelegateFreeAsync() = delete;

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(func);	}

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()() override {
		if (m_thread == nullptr)
			BaseType::operator()();
		else
		{
			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsgBase>(delegate);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Invoke the delegate function
		BaseType::operator()();
	}

private:
	DelegateThread* m_thread = nullptr;
};

template <class Param1> 
class DelegateFreeAsync<void(Param1)> : public DelegateFree<void(Param1)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1);
    using ClassType = DelegateFreeAsync<void(Param1)>;
    using BaseType = DelegateFree<void(Param1)>;

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : BaseType(func) { Bind(func, thread); }
	DelegateFreeAsync() = delete;

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(func);	}

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1) override {
		if (m_thread == nullptr)
			BaseType::operator()(p1);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg1<Param1>>(delegate, heapParam1);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>))),
			"std::shared_ptr reference argument not allowed");
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg1<Param1>* delegateMsg = static_cast<DelegateMsg1<Param1>*>(msg.get());

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();

		// Invoke the delegate function
		BaseType::operator()(param1);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
	}

private:
	DelegateThread* m_thread = nullptr;
};

template <class Param1, class Param2> 
class DelegateFreeAsync<void(Param1, Param2)> : public DelegateFree<void(Param1, Param2)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2);
    using ClassType = DelegateFreeAsync<void(Param1, Param2)>;
    using BaseType = DelegateFree<void(Param1, Param2)>;

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : BaseType(func) { Bind(func, thread); }
	DelegateFreeAsync() = delete;

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(func);	}

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2) override {
		if (m_thread == nullptr)
			BaseType::operator()(p1, p2);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg2<Param1, Param2>>(delegate, heapParam1, heapParam2);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>))),
			"std::shared_ptr reference argument not allowed");
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg2<Param1, Param2>* delegateMsg = static_cast<DelegateMsg2<Param1, Param2>*>(msg.get());

		// Get the function parameter data
		Param1 param1 = delegateMsg->GetParam1();
		Param2 param2 = delegateMsg->GetParam2();

		// Invoke the delegate function
		BaseType::operator()(param1, param2);

		// Delete heap data created inside operator()
		DelegateParam<Param1>::Delete(param1);
		DelegateParam<Param2>::Delete(param2);
	}

private:
	DelegateThread* m_thread = nullptr;
};

template <class Param1, class Param2, class Param3> 
class DelegateFreeAsync<void(Param1, Param2, Param3)> : public DelegateFree<void(Param1, Param2, Param3)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2, Param3);
    using ClassType = DelegateFreeAsync<void(Param1, Param2, Param3)>;
    using BaseType = DelegateFree<void(Param1, Param2, Param3)>;

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : BaseType(func) { Bind(func, thread); }
	DelegateFreeAsync() = delete;

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(func);	}

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3) override {
		if (m_thread == nullptr)
			BaseType::operator()(p1, p2, p3);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg3<Param1, Param2, Param3>>(delegate, heapParam1, heapParam2, heapParam3);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>)) ||
			(is_shared_ptr<Param3>::value && (std::is_lvalue_reference_v<Param3> || std::is_pointer_v<Param3>))),
			"std::shared_ptr reference argument not allowed");
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg3<Param1, Param2, Param3>* delegateMsg = static_cast<DelegateMsg3<Param1, Param2, Param3>*>(msg.get());

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
	}

private:
	DelegateThread* m_thread = nullptr;
};

template <class Param1, class Param2, class Param3, class Param4> 
class DelegateFreeAsync<void(Param1, Param2, Param3, Param4)> : public DelegateFree<void(Param1, Param2, Param3, Param4)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2, Param3, Param4);
    using ClassType = DelegateFreeAsync<void(Param1, Param2, Param3, Param4)>;
    using BaseType = DelegateFree<void(Param1, Param2, Param3, Param4)>;

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : BaseType(func) { Bind(func, thread); }
	DelegateFreeAsync() = delete;

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(func);	}

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (m_thread == nullptr)
			BaseType::operator()(p1, p2, p3, p4);
		else
		{
			// Create a new instance of the function argument data and copy
			Param1 heapParam1 = DelegateParam<Param1>::New(p1);
			Param2 heapParam2 = DelegateParam<Param2>::New(p2);
			Param3 heapParam3 = DelegateParam<Param3>::New(p3);
			Param4 heapParam4 = DelegateParam<Param4>::New(p4);

			// Create a clone instance of this delegate 
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg4<Param1, Param2, Param3, Param4>>(delegate, heapParam1, heapParam2, heapParam3, heapParam4);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>)) ||
			(is_shared_ptr<Param3>::value && (std::is_lvalue_reference_v<Param3> || std::is_pointer_v<Param3>)) ||
			(is_shared_ptr<Param4>::value && (std::is_lvalue_reference_v<Param4> || std::is_pointer_v<Param4>))),
			"std::shared_ptr reference argument not allowed");
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg4<Param1, Param2, Param3, Param4>* delegateMsg = static_cast<DelegateMsg4<Param1, Param2, Param3, Param4>*>(msg.get());

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
	}

private:
	DelegateThread* m_thread = nullptr;
};

template <class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)> : public DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>, public IDelegateInvoker {
public:
	typedef void (*FreeFunc)(Param1, Param2, Param3, Param4, Param5);
    using ClassType = DelegateFreeAsync<void(Param1, Param2, Param3, Param4, Param5)>;
    using BaseType = DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>;

	DelegateFreeAsync(FreeFunc func, DelegateThread* thread) : BaseType(func) { Bind(func, thread); }
	DelegateFreeAsync() = delete;

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func, DelegateThread* thread) {
		m_thread = thread; 
		BaseType::Bind(func);	}

	virtual ClassType* Clone() const override {	return new ClassType(*this); }

	virtual bool operator==(const DelegateBase& rhs) const {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_thread == derivedRhs->m_thread &&
			BaseType::operator == (rhs); }

	// Invoke delegate function asynchronously
	virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (m_thread == nullptr)
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
			auto delegate = std::shared_ptr<ClassType>(Clone());

			// Create a new message instance 
			auto msg = std::make_shared<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>>(delegate, heapParam1, heapParam2, heapParam3, heapParam4, heapParam5);

			// Dispatch message onto the callback destination thread. DelegateInvoke()
			// will be called by the target thread. 
			m_thread->DispatchDelegate(msg);
		}

		static_assert(!(
			(is_shared_ptr<Param1>::value && (std::is_lvalue_reference_v<Param1> || std::is_pointer_v<Param1>)) ||
			(is_shared_ptr<Param2>::value && (std::is_lvalue_reference_v<Param2> || std::is_pointer_v<Param2>)) ||
			(is_shared_ptr<Param3>::value && (std::is_lvalue_reference_v<Param3> || std::is_pointer_v<Param3>)) ||
			(is_shared_ptr<Param4>::value && (std::is_lvalue_reference_v<Param4> || std::is_pointer_v<Param4>)) ||
			(is_shared_ptr<Param5>::value && (std::is_lvalue_reference_v<Param5> || std::is_pointer_v<Param5>))),
			"std::shared_ptr reference argument not allowed");
	}

	// Called to invoke the delegate function on the target thread of control
	virtual void DelegateInvoke(std::shared_ptr<DelegateMsgBase> msg) override {
		// Typecast the base pointer to back to the templatized instance
		DelegateMsg5<Param1, Param2, Param3, Param4, Param5>* delegateMsg = static_cast<DelegateMsg5<Param1, Param2, Param3, Param4, Param5>*>(msg.get());

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
	}

private:
	DelegateThread* m_thread = nullptr;
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
