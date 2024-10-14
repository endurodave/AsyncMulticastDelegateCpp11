#ifndef _DELEGATE_REMOTE_RECV_H
#define _DELEGATE_REMOTE_RECV_H

// DelegateRemoteRecv.h
// @see https://github.com/endurodave/RemoteDelegate
// David Lafreniere, Mar 2020.

#include "Delegate.h"
#include "DelegateTransport.h"
#include "DelegateRemoteInvoker.h"

namespace DelegateLib {

template <class Param>
class RemoteParam
{
public:
    Param& Get() { return m_param; }
private:
    Param m_param;
};

template <class Param>
class RemoteParam<Param*>
{
public:
    Param* Get() { return &m_param; }
private:
    Param m_param;
};

template <class Param>
class RemoteParam<Param**>
{
public:
    RemoteParam() { m_pParam = &m_param; }
    Param ** Get() { return &m_pParam; }
private:
    Param m_param;
    Param* m_pParam;
};

template <class Param>
class RemoteParam<Param&>
{
public:
    Param & Get() { return m_param; }
private:
    Param m_param;
};

// Declare DelegateMemberRemoteRecv as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateMemberRemoteRecv;

/// @brief Receive a delegate from a remote system and invoke the bound function. 
template <class TClass, class Param1>
class DelegateMemberRemoteRecv<void(TClass(Param1))> : public DelegateMember<void(TClass(Param1))>, public DelegateRemoteInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Param1);
    typedef void (TClass::*ConstMemberFunc)(Param1) const;
    using ClassType = DelegateMemberRemoteRecv<void(TClass(Param1))>;
    using BaseType = DelegateMember<void(TClass(Param1))>;

    // Contructors take a class instance, member function, and delegete id 
    DelegateMemberRemoteRecv(ObjectPtr object, MemberFunc func, DelegateIdType id) : 
        BaseType(object, func),
        DelegateRemoteInvoker(id) { Bind(object, func, id); }
    DelegateMemberRemoteRecv(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) : 
        BaseType(object, func),
        DelegateRemoteInvoker(id) { Bind(object, func, id); }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;

        Param1 p1 = param1.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

    private:
        DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class TClass, class Param1, class Param2>
class DelegateMemberRemoteRecv<void(TClass(Param1, Param2))> : public DelegateMember<void(TClass(Param1, Param2))>, public DelegateRemoteInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Param1, Param2);
    typedef void (TClass::*ConstMemberFunc)(Param1, Param2) const;
    using ClassType = DelegateMemberRemoteRecv<void(TClass(Param1, Param2))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2))>;

    // Contructors take a class instance, member function, and delegete id 
    DelegateMemberRemoteRecv(ObjectPtr object, MemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }
    DelegateMemberRemoteRecv(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class TClass, class Param1, class Param2, class Param3>
class DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3))> : public DelegateMember<void(TClass(Param1, Param2, Param3))>, public DelegateRemoteInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Param1, Param2, Param3);
    typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3) const;
    using ClassType = DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2, Param3))>;

    // Contructors take a class instance, member function, and delegete id 
    DelegateMemberRemoteRecv(ObjectPtr object, MemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }
    DelegateMemberRemoteRecv(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this);
    }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;
        RemoteParam<Param3> param3;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();
        Param3 p3 = param3.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p3;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2, p3);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class TClass, class Param1, class Param2, class Param3, class Param4>
class DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4))> : public DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>, public DelegateRemoteInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4);
    typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4) const;
    using ClassType = DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2, Param3, Param4))>;

    // Contructors take a class instance, member function, and delegete id 
    DelegateMemberRemoteRecv(ObjectPtr object, MemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }
    DelegateMemberRemoteRecv(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;
        RemoteParam<Param3> param3;
        RemoteParam<Param4> param4;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();
        Param3 p3 = param3.Get();
        Param4 p4 = param4.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p3;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p4;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2, p3, p4);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4, Param5))> : public DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>, public DelegateRemoteInvoker {
public:
    typedef TClass* ObjectPtr;
    typedef void (TClass::*MemberFunc)(Param1, Param2, Param3, Param4, Param5);
    typedef void (TClass::*ConstMemberFunc)(Param1, Param2, Param3, Param4, Param5) const;
    using ClassType = DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4, Param5))>;
    using BaseType = DelegateMember<void(TClass(Param1, Param2, Param3, Param4, Param5))>;

    // Contructors take a class instance, member function, and delegete id 
    DelegateMemberRemoteRecv(ObjectPtr object, MemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }
    DelegateMemberRemoteRecv(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) :
        BaseType(object, func),
        DelegateRemoteInvoker(id) {
        Bind(object, func, id);
    }

    /// Bind a member function to a delegate. 
    void Bind(ObjectPtr object, MemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    /// Bind a const member function to a delegate. 
    void Bind(ObjectPtr object, ConstMemberFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(object, func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this);
    }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;
        RemoteParam<Param3> param3;
        RemoteParam<Param4> param4;
        RemoteParam<Param5> param5;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();
        Param3 p3 = param3.Get();
        Param4 p4 = param4.Get();
        Param4 p5 = param5.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p3;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p4;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p5;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2, p3, p4, p5);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

// Declare DelegateFreeRemoteRecv as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateFreeRemoteRecv;

template <class Param1>
class DelegateFreeRemoteRecv<void(Param1)> : public DelegateFree<void(Param1)>, public DelegateRemoteInvoker {
public:
    typedef void(*FreeFunc)(Param1);
    using ClassType = DelegateFreeRemoteRecv<void(Param1)>;
    using BaseType = DelegateFree<void(Param1)>;

    // Contructors take a free function and delegete id 
    DelegateFreeRemoteRecv(FreeFunc func, DelegateIdType id) : 
        BaseType(func), 
        DelegateRemoteInvoker(id) { Bind(func, id); }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;

        Param1 p1 = param1.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class Param1, class Param2>
class DelegateFreeRemoteRecv<void(Param1, Param2)> : public DelegateFree<void(Param1, Param2)>, public DelegateRemoteInvoker {
public:
    typedef void(*FreeFunc)(Param1, Param2);
    using ClassType = DelegateFreeRemoteRecv<void(Param1, Param2)>;
    using BaseType = DelegateFree<void(Param1, Param2)>;

    // Contructors take a free function and delegete id 
    DelegateFreeRemoteRecv(FreeFunc func, DelegateIdType id) : 
        BaseType(func), 
        DelegateRemoteInvoker(id) { Bind(func, id); }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class Param1, class Param2, class Param3>
class DelegateFreeRemoteRecv<void(Param1, Param2, Param3)> : public DelegateFree<void(Param1, Param2, Param3)>, public DelegateRemoteInvoker {
public:
    typedef void(*FreeFunc)(Param1, Param2, Param3);
    using ClassType = DelegateFreeRemoteRecv<void(Param1, Param2, Param3)>;
    using BaseType = DelegateFree<void(Param1, Param2, Param3)>;

    // Contructors take a free function and delegete id 
    DelegateFreeRemoteRecv(FreeFunc func, DelegateIdType id) : 
        BaseType(func), 
        DelegateRemoteInvoker(id) { Bind(func, id); }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;
        RemoteParam<Param3> param3;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();
        Param3 p3 = param3.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p3;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2, p3);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class Param1, class Param2, class Param3, class Param4>
class DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4)> : public DelegateFree<void(Param1, Param2, Param3, Param4)>, public DelegateRemoteInvoker {
public:
    typedef void(*FreeFunc)(Param1, Param2, Param3, Param4);
    using ClassType = DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4)>;
    using BaseType = DelegateFree<void(Param1, Param2, Param3, Param4)>;

    // Contructors take a free function and delegete id 
    DelegateFreeRemoteRecv(FreeFunc func, DelegateIdType id) : 
        BaseType(func), 
        DelegateRemoteInvoker(id) { Bind(func, id); }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;
        RemoteParam<Param3> param3;
        RemoteParam<Param4> param4;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();
        Param3 p3 = param3.Get();
        Param4 p4 = param4.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p3;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p4;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2, p3, p4);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

template <class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4, Param5)> : public DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>, public DelegateRemoteInvoker {
public:
    typedef void(*FreeFunc)(Param1, Param2, Param3, Param4, Param5);
    using ClassType = DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4, Param5)>;
    using BaseType = DelegateFree<void(Param1, Param2, Param3, Param4, Param5)>;

    // Contructors take a free function and delegete id 
    DelegateFreeRemoteRecv(FreeFunc func, DelegateIdType id) : 
        BaseType(func), 
        DelegateRemoteInvoker(id) { Bind(func, id); }

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func, DelegateIdType id) {
        m_id = id;
        BaseType::Bind(func);
    }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Called by the remote system to invoke the delegate function
    virtual void DelegateInvoke(std::istream& stream) override {
        RemoteParam<Param1> param1;
        RemoteParam<Param2> param2;
        RemoteParam<Param3> param3;
        RemoteParam<Param4> param4;
        RemoteParam<Param5> param5;

        Param1 p1 = param1.Get();
        Param2 p2 = param2.Get();
        Param3 p3 = param3.Get();
        Param4 p4 = param4.Get();
        Param5 p5 = param5.Get();

        stream >> m_id;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p1;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p2;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p3;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p4;
        stream.seekg(stream.tellg() + std::streampos(1));
        stream >> p5;
        stream.seekg(stream.tellg() + std::streampos(1));

        BaseType::operator()(p1, p2, p3, p4, p5);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            BaseType::operator == (rhs);
    }

private:
    DelegateIdType m_id = 0;               // Remote delegate identifier
};

//N=1
template <class TClass, class Param1>
DelegateMemberRemoteRecv<void (TClass(Param1))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1), DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1))>(object, func, id);
}

template <class TClass, class Param1>
DelegateMemberRemoteRecv<void(TClass(Param1))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1) const, DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1))>(object, func, id);
}

template <class Param1>
DelegateFreeRemoteRecv<void(Param1)> MakeDelegate(void(*func)(Param1 p1), DelegateIdType id) {
    return DelegateFreeRemoteRecv<void(Param1)>(func, id);
}

//N=2
template <class TClass, class Param1, class Param2>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2), DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2))>(object, func, id);
}

template <class TClass, class Param1, class Param2>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2) const, DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2))>(object, func, id);
}

template <class Param1, class Param2>
DelegateFreeRemoteRecv<void(Param1, Param2)> MakeDelegate(void(*func)(Param1 p1, Param2 p2), DelegateIdType id) {
    return DelegateFreeRemoteRecv<void(Param1, Param2)>(func, id);
}

//N=3
template <class TClass, class Param1, class Param2, class Param3>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3), DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3))>(object, func, id);
}

template <class TClass, class Param1, class Param2, class Param3>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3) const, DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3))>(object, func, id);
}

template <class Param1, class Param2, class Param3>
DelegateFreeRemoteRecv<void(Param1, Param2, Param3)> MakeDelegate(void(*func)(Param1 p1, Param2 p2, Param3 p3), DelegateIdType id) {
    return DelegateFreeRemoteRecv<void(Param1, Param2, Param3)>(func, id);
}

//N=4
template <class TClass, class Param1, class Param2, class Param3, class Param4>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4))>(object, func, id);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4) const, DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4))>(object, func, id);
}

template <class Param1, class Param2, class Param3, class Param4>
DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4)> MakeDelegate(void(*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4), DelegateIdType id) {
    return DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4)>(func, id);
}

//N=5
template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, id);
}

template <class TClass, class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4, Param5))> MakeDelegate(TClass* object, void (TClass::*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) const, DelegateIdType id) {
    return DelegateMemberRemoteRecv<void(TClass(Param1, Param2, Param3, Param4, Param5))>(object, func, id);
}

template <class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4, Param5)> MakeDelegate(void(*func)(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5), DelegateIdType id) {
    return DelegateFreeRemoteRecv<void(Param1, Param2, Param3, Param4, Param5)>(func, id);
}

}

#endif
