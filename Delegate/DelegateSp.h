#ifndef _DELEGATE_SP_H
#define _DELEGATE_SP_H

// Delegate.h
// @see https://github.com/endurodave/AsyncMulticastDelegateCpp11
// David Lafreniere, Oct 2022.
//
// The DelegateMemberSpX delegate implemenations synchronously bind and invoke class instance member functions. 
// The std::shared_ptr<TClass> is used in lieu of a raw TClass* pointer. 

#include "Delegate.h"
#include <memory>

namespace DelegateLib {

// Declare DelegateMemberSp as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMemberSp;

/// @brief DelegateMemberSp is used to store and invoke an instance member function.
template <class TClass, class RetType> 
class DelegateMemberSp<RetType(TClass(void))> : public Delegate<RetType(void)> {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(); 
	typedef RetType (TClass::*ConstMemberFunc)() const; 
    using ClassType = DelegateMemberSp<RetType(TClass(void))>;
    using BaseType = Delegate<RetType(void)>;

	DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }
	DelegateMemberSp() = delete;

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
    virtual RetType operator()() override
    {
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
	explicit operator bool() const { return !Empty();  }

private:
	ObjectPtr m_object = nullptr;		// Pointer to a class object
	MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1> 
class DelegateMemberSp<RetType(TClass(Param1))> : public Delegate<RetType(Param1)> {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1) const; 
    using ClassType = DelegateMemberSp<RetType(TClass(Param1))>;
    using BaseType = Delegate<RetType(Param1)>;

	DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMemberSp() = delete;

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
    virtual RetType operator()(Param1 p1) override
    {
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
	explicit operator bool() const { return !Empty();  }

private:
	ObjectPtr m_object = nullptr;		// Pointer to a class object
	MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2> 
class DelegateMemberSp<RetType(TClass(Param1, Param2))> : public Delegate<RetType(Param1, Param2)> {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2) const; 
    using ClassType = DelegateMemberSp<RetType(TClass(Param1, Param2))>;
    using BaseType = Delegate<RetType(Param1, Param2)>;

	DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMemberSp() = delete;

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
    virtual RetType operator()(Param1 p1, Param2 p2) override
    {
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
	explicit operator bool() const { return !Empty();  }

private:
	ObjectPtr m_object = nullptr;		// Pointer to a class object
	MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2, class Param3> 
class DelegateMemberSp<RetType(TClass(Param1, Param2, Param3))> : public Delegate<RetType(Param1, Param2, Param3)> {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const; 
    using ClassType = DelegateMemberSp<RetType(TClass(Param1, Param2, Param3))>;
    using BaseType = Delegate<RetType(Param1, Param2, Param3)>;

	DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMemberSp() = delete;

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
    virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3) override
    {
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
	explicit operator bool() const { return !Empty();  }

private:
	ObjectPtr m_object = nullptr;		// Pointer to a class object
	MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4> 
class DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4))> : public Delegate<RetType(Param1, Param2, Param3, Param4)> {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3, Param4); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const; 
    using ClassType = DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4))>;
    using BaseType = Delegate<RetType(Param1, Param2, Param3, Param4)>;

	DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMemberSp() = delete;

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
    virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override
    {
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
	explicit operator bool() const { return !Empty();  }

private:
	ObjectPtr m_object = nullptr;		// Pointer to a class object
	MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class Param1, class Param2, class Param3, class Param4, class Param5> 
class DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> : public Delegate<RetType(Param1, Param2, Param3, Param4, Param5)> {
public:
	typedef std::shared_ptr<TClass> ObjectPtr;
	typedef RetType (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5); 
	typedef RetType (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const; 
    using ClassType = DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>;
    using BaseType = Delegate<RetType(Param1, Param2, Param3, Param4, Param5)>;

	DelegateMemberSp(ObjectPtr object, MemberFunc func) { Bind(object, func); }
	DelegateMemberSp(ObjectPtr object, ConstMemberFunc func) { Bind(object, func);	}
	DelegateMemberSp() = delete;

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
	virtual RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override
    {
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
	explicit operator bool() const { return !Empty();  }

private:
	ObjectPtr m_object = nullptr;		// Pointer to a class object
	MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

// MakeDelegate function creates a delegate object. C++ template argument deduction
// means you can call MakeDelegate without manually specifying the template parameters. 

//N=0
template <class TClass, class RetType>
DelegateMemberSp<RetType(TClass(void))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)()) { 
	return DelegateMemberSp<RetType(TClass(void))>(object, func);
}

template <class TClass, class RetType>
DelegateMemberSp<RetType(TClass(void))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)() const) {
	return DelegateMemberSp<RetType(TClass(void))>(object, func);
}

//N=1
template <class TClass, class Param1, class RetType>
DelegateMemberSp<RetType(TClass(Param1))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1)) {
	return DelegateMemberSp<RetType(TClass(Param1))>(object, func);
}

template <class TClass, class Param1, class RetType>
DelegateMemberSp<RetType(TClass(Param1))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1) const) {
	return DelegateMemberSp<RetType(TClass(Param1))>(object, func);
}

//N=2
template <class TClass, class Param1, class Param2, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2)) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2))>(object, func);
}

template <class TClass, class Param1, class Param2, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2) const) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2))>(object, func);
}

//N=3
template <class TClass, class Param1, class Param2, class Param3, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2, Param3))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3)) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2, Param3))>(object, func);
}

template <class TClass, class Param1, class Param2, class Param3, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2, Param3))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3) const) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2, Param3))>(object, func);
}

//N=4
template <class TClass, class Param1, class Param2, class Param3, class Param4, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4)) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4))>(object, func);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4))>(object, func);
}

//N=5
template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5)) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5, class RetType>
DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(std::shared_ptr<TClass> object, RetType (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const) {
	return DelegateMemberSp<RetType(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func);
}

}

#endif