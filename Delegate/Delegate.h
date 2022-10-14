#ifndef _DELEGATE_H
#define _DELEGATE_H

// Delegate.h
// @see https://www.codeproject.com/Articles/1160934/Asynchronous-Multicast-Delegates-in-Cplusplus
// David Lafreniere, Oct 2022.

#if USE_XALLOCATOR
	#include "xallocator.h"
#endif

namespace DelegateLib {

/// @brief Non-template common base class for all delegates.
class DelegateBase {
#if USE_XALLOCATOR
	XALLOCATOR
#endif
public:
	virtual ~DelegateBase() {}

	/// Derived class must implement operator== to compare objects.
	virtual bool operator==(const DelegateBase& rhs) const = 0;
	virtual bool operator!=(const DelegateBase& rhs) { return !(*this == rhs); }

	/// Use Clone to provide a deep copy using a base pointer. Covariant 
	/// overloading is used so that a Clone() method return type is a
	/// more specific type in the derived class implementations.
	/// @return A dynamic copy of this instance created with operator new. 
	/// @post The caller is responsible for deleting the clone instance. 
	virtual DelegateBase* Clone() const = 0;
};

// Declare Delegate as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class Delegate;

/// @brief Abstract delegate template base class.
template <class RetType>
class Delegate<RetType(void)> : public DelegateBase {
public:
	virtual RetType operator()() = 0;
	virtual Delegate* Clone() const = 0;
};

template <class RetType, class Param1>
class Delegate<RetType(Param1)> : public DelegateBase {
public:
	virtual RetType operator()(Param1 p1) = 0;
	virtual Delegate* Clone() const = 0;
};

template <class RetType, class Param1, class Param2>
class Delegate<RetType(Param1, Param2)> : public DelegateBase {
public:
	virtual RetType operator()(Param1 p1, Param2 p2) = 0;
	virtual Delegate* Clone() const = 0;
};

template <class RetType, class Param1, class Param2, class Param3>
class Delegate<RetType(Param1, Param2, Param3)> : public DelegateBase {
public:
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3) = 0;
	virtual Delegate* Clone() const = 0;
};

template <class RetType, class Param1, class Param2, class Param3, class Param4>
class Delegate<RetType(Param1, Param2, Param3, Param4)> : public DelegateBase {
public:
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) = 0;
	virtual Delegate* Clone() const = 0;
};

template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class Delegate<RetType(Param1, Param2, Param3, Param4, Param5)> : public DelegateBase {
public:
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) = 0;
	virtual Delegate* Clone() const = 0;
};

// Declare DelegateMember as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMember;

/// @brief DelegateMember is used to store and invoke an instance member function.
template <class TClass, class RetType> 
class DelegateMember<RetType(TClass(void))> : public Delegate<RetType(void)> {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(); 
	typedef RetType (TClass::*ConstMemberFunc)() const; 
    using ClassType = DelegateMember<RetType(TClass(void))>;

	DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }
	DelegateMember() :	m_object(0), m_func(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func) {
		m_object = object;
		m_func = func; }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func) {
		m_object = object;
		m_func = reinterpret_cast<MemberFunc>(func); }

	virtual DelegateMember* Clone() const override { return new ClassType(*this); }

	// Invoke the bound delegate function
	virtual RetType operator()() override {
		if (m_object)
			return (*m_object.*m_func)();
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func && 
			m_object == derivedRhs->m_object; }

	bool Empty() const { return !(m_object && m_func); }
	void Clear() { m_object = 0; m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	ObjectPtr m_object;		// Pointer to a class object
	MemberFunc m_func;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1> 
class DelegateMember<RetType(TClass(Param1))> : public Delegate<RetType(Param1)> {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1) const; 
    using ClassType = DelegateMember<RetType(TClass(Param1))>;

	DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMember() :	m_object(0), m_func(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func) {
		m_object = object;
		m_func = func; }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func) {
		m_object = object;
		m_func = reinterpret_cast<MemberFunc>(func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	// Invoke the bound delegate function
	virtual RetType operator()(Param1 p1) override {
		if (m_object)
			return (*m_object.*m_func)(p1);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func && 
			m_object == derivedRhs->m_object; }

	bool Empty() const { return !(m_object && m_func); }
	void Clear() { m_object = 0; m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	ObjectPtr m_object;		// Pointer to a class object
	MemberFunc m_func;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2> 
class DelegateMember<RetType(TClass(Param1, Param2))> : public Delegate<RetType(Param1, Param2)> {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2) const; 
    using ClassType = DelegateMember<RetType(TClass(Param1, Param2))>;

	DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMember() :	m_object(0), m_func(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func) {
		m_object = object;
		m_func = func; }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func) {
		m_object = object;
		m_func = reinterpret_cast<MemberFunc>(func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	// Invoke the bound delegate function
	virtual RetType operator()(Param1 p1, Param2 p2) override {
		if (m_object)
			return (*m_object.*m_func)(p1, p2);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func && 
			m_object == derivedRhs->m_object; }

	bool Empty() const { return !(m_object && m_func); }
	void Clear() { m_object = 0; m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	ObjectPtr m_object;		// Pointer to a class object
	MemberFunc m_func;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2, class Param3> 
class DelegateMember<RetType(TClass(Param1, Param2, Param3))> : public Delegate<RetType(Param1, Param2, Param3)> {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const; 
    using ClassType = DelegateMember<RetType(TClass(Param1, Param2, Param3))>;

	DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMember() :	m_object(0), m_func(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func) {
		m_object = object;
		m_func = func; }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func) {
		m_object = object;
		m_func = reinterpret_cast<MemberFunc>(func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	// Invoke the bound delegate function
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3) override {
		if (m_object)
			return (*m_object.*m_func)(p1, p2, p3);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func && 
			m_object == derivedRhs->m_object; }

	bool Empty() const { return !(m_object && m_func); }
	void Clear() { m_object = 0; m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	ObjectPtr m_object;		// Pointer to a class object
	MemberFunc m_func;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4> 
class DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))> : public Delegate<RetType(Param1, Param2, Param3, Param4)> {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3, Param4); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const; 
    using ClassType = DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))>;

	DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMember() :	m_object(0), m_func(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func) {
		m_object = object;
		m_func = func; }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func) {
		m_object = object;
		m_func = reinterpret_cast<MemberFunc>(func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	// Invoke the bound delegate function
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (m_object)
			return (*m_object.*m_func)(p1, p2, p3, p4);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func && 
			m_object == derivedRhs->m_object; }

	bool Empty() const { return !(m_object && m_func); }
	void Clear() { m_object = 0; m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	ObjectPtr m_object;		// Pointer to a class object
	MemberFunc m_func;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> : public Delegate<RetType(Param1, Param2, Param3, Param4, Param5)> {
public:
	typedef TClass* ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const; 
    using ClassType = DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>;

	DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMember() :	m_object(0), m_func(0) { }

	/// Bind a member function to a delegate. 
	void Bind(ObjectPtr object, MemberFunc func) {
		m_object = object;
		m_func = func; }

	/// Bind a const member function to a delegate. 
	void Bind(ObjectPtr object, ConstMemberFunc func) {
		m_object = object;
		m_func = reinterpret_cast<MemberFunc>(func); }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	// Invoke the bound delegate function
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (m_object)
			return (*m_object.*m_func)(p1, p2, p3, p4, p5);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func && 
			m_object == derivedRhs->m_object; }

	bool Empty() const { return !(m_object && m_func); }
	void Clear() { m_object = 0; m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	ObjectPtr m_object;		// Pointer to a class object
	MemberFunc m_func;   	// Pointer to an instance member function
};

// Declare DelegateFree as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateFree;

/// @brief DelegateFree is used to store and invoke any non-member function 
/// (i.e. a static member function or a global function). 
template <class RetType> 
class DelegateFree<RetType(void)> : public Delegate<RetType(void)> {
public:
	typedef RetType (*FreeFunc)(); 
    using ClassType = DelegateFree<RetType(void)>;

	DelegateFree(FreeFunc func) { Bind(func); }
	DelegateFree() : m_func(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func) { m_func = func; }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual RetType operator()() override {
		if (m_func)
			return (*m_func)();
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func; }

	bool Empty() const { return !m_func; }
	void Clear() { m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	FreeFunc m_func;		// Pointer to a free function
};

template <class RetType, class Param1> 
class DelegateFree<RetType(Param1)> : public Delegate<RetType(Param1)> {
public:
	typedef RetType (*FreeFunc)(Param1); 
    using ClassType = DelegateFree<RetType(Param1)>;

	DelegateFree(FreeFunc func) { Bind(func); }
	DelegateFree() : m_func(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func) { m_func = func; }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual RetType operator()(Param1 p1) override {
		if (m_func)
			return (*m_func)(p1);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func; }

	bool Empty() const { return !m_func; }
	void Clear() { m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	FreeFunc m_func;		// Pointer to a free function
};

template <class RetType, class Param1, class Param2> 
class DelegateFree<RetType(Param1, Param2)> : public Delegate<RetType(Param1, Param2)> {
public:
	typedef RetType (*FreeFunc)(Param1, Param2); 
    using ClassType = DelegateFree<RetType(Param1, Param2)>;

	DelegateFree(FreeFunc func) { Bind(func); }
	DelegateFree() : m_func(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func) {
		m_func = func; }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual RetType operator()(Param1 p1, Param2 p2) override {
		if (m_func)
			return (*m_func)(p1, p2);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func; }

	bool Empty() const { return !m_func; }
	void Clear() { m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	FreeFunc m_func;		// Pointer to a free function
};

template <class RetType, class Param1, class Param2, class Param3> 
class DelegateFree<RetType(Param1, Param2, Param3)> : public Delegate<RetType(Param1, Param2, Param3)> {
public:
	typedef RetType (*FreeFunc)(Param1, Param2, Param3); 
    using ClassType = DelegateFree<RetType(Param1, Param2, Param3)>;

	DelegateFree(FreeFunc func) { Bind(func); }
	DelegateFree() : m_func(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func) {
		m_func = func; }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3) override {
		if (m_func)
			return (*m_func)(p1, p2, p3);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func; }

	bool Empty() const { return !m_func; }
	void Clear() { m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	FreeFunc m_func;		// Pointer to a free function
};

template <class RetType, class Param1, class Param2, class Param3, class Param4> 
class DelegateFree<RetType(Param1, Param2, Param3, Param4)> : public Delegate<RetType(Param1, Param2, Param3, Param4)> {
public:
	typedef RetType (*FreeFunc)(Param1, Param2, Param3, Param4); 
    using ClassType = DelegateFree<RetType(Param1, Param2, Param3, Param4)>;

	DelegateFree(FreeFunc func) { Bind(func); }
	DelegateFree() : m_func(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func) {
		m_func = func; }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
		if (m_func)
			return (*m_func)(p1, p2, p3, p4);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func; }

	bool Empty() const { return !m_func; }
	void Clear() { m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	FreeFunc m_func;		// Pointer to a free function
};

template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)> : public Delegate<RetType(Param1, Param2, Param3, Param4, Param5)> {
public:
	typedef RetType (*FreeFunc)(Param1, Param2, Param3, Param4, Param5); 
    using ClassType = DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)>;

	DelegateFree(FreeFunc func) { Bind(func); }
	DelegateFree() : m_func(0) { }

	/// Bind a free function to the delegate.
	void Bind(FreeFunc func) {
		m_func = func; }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
		if (m_func)
			return (*m_func)(p1, p2, p3, p4, p5);
		else
			return RetType();
	}

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_func == derivedRhs->m_func; }

	bool Empty() const { return !m_func; }
	void Clear() { m_func = 0; }
	explicit operator bool() const { return !Empty(); }

private:
	FreeFunc m_func;		// Pointer to a free function
};

// MakeDelegate function creates a delegate object. C++ template argument deduction
// means you can call MakeDelegate without manually specifying the template parameters. 

//N=0
template <class TClass, class RetType>
DelegateMember<RetType(TClass(void))> MakeDelegate(TClass* object, RetType (TClass::*func)()) { 
	return DelegateMember<RetType(TClass(void))>(object, func);
}

template <class TClass, class RetType>
DelegateMember<RetType(TClass(void))> MakeDelegate(TClass* object, RetType (TClass::*func)() const) {
	return DelegateMember<RetType(TClass(void))>(object, func);
}

template <class RetType>
DelegateFree<RetType(void)> MakeDelegate(RetType (*func)()) { 
	return DelegateFree<RetType(void)>(func);
}

//N=1
template <class TClass, class RetType, class Param1>
DelegateMember<RetType(TClass(Param1))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1)) {
	return DelegateMember<RetType(TClass(Param1))>(object, func);
}

template <class TClass, class RetType, class Param1>
DelegateMember<RetType(TClass(Param1))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1) const) {
	return DelegateMember<RetType(TClass(Param1))>(object, func);
}

template <class Param1, class RetType>
DelegateFree<RetType(Param1)> MakeDelegate(RetType (*func)(Param1 p1)) {
	return DelegateFree<RetType(Param1)>(func);
}

//N=2
template <class TClass, class RetType, class Param1, class Param2>
DelegateMember<RetType(TClass(Param1, Param2))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2)) {
	return DelegateMember<RetType(TClass(Param1, Param2))>(object, func);
}

template <class TClass, class RetType, class Param1, class Param2>
DelegateMember<RetType(TClass(Param1, Param2))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2) const) {
	return DelegateMember<RetType(TClass(Param1, Param2))>(object, func);
}

template <class RetType, class Param1, class Param2>
DelegateFree<RetType(Param1, Param2)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2)) {
	return DelegateFree<RetType(Param1, Param2)>(func);
}

//N=3
template <class TClass, class RetType, class Param1, class Param2, class Param3>
DelegateMember<RetType(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3)) {
	return DelegateMember<RetType(TClass(Param1, Param2, Param3))>(object, func);
}

template <class TClass, class RetType, class Param1, class Param2, class Param3>
DelegateMember<RetType(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3) const) {
	return DelegateMember<RetType(TClass(Param1, Param2, Param3))>(object, func);
}

template <class RetType, class Param1, class Param2, class Param3>
DelegateFree<RetType(Param1, Param2, Param3)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2, Param3 p3)) {
	return DelegateFree<RetType(Param1, Param2, Param3)>(func);
}

//N=4
template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4>
DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) {
	return DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))>(object, func);
}

template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4>
DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) { 
	return DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4))>(object, func);
}

template <class RetType, class Param1, class Param2, class Param3, class Param4>
DelegateFree<RetType(Param1, Param2, Param3, Param4)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) {
	return DelegateFree<RetType(Param1, Param2, Param3, Param4)>(func);
}

//N=5
template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) {
	return DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func);
}

template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) {
	return DelegateMember<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func);
}

template <class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)> MakeDelegate(RetType (*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) {
	return DelegateFree<RetType(Param1, Param2, Param3, Param4, Param5)>(func);
}

}

#endif