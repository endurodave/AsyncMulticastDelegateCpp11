#ifndef _DELEGATE_REMOTE_SEND_H
#define _DELEGATE_REMOTE_SEND_H

// DelegateRemoteSend.h
// @see https://www.codeproject.com/Articles/5262271/Remote-Procedure-Calls-using-Cplusplus-Delegates
// David Lafreniere, Mar 2020.

#include "Delegate.h"
#include "DelegateTransport.h"
#include "DelegateRemoteInvoker.h"

namespace DelegateLib {

// Declare DelegateRemoteSend as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class DelegateRemoteSend;

/// @brief Send a delegate to invoke a function on a remote system. 
template <class Param1>
class DelegateRemoteSend<void(Param1)> : public Delegate<void(Param1)> {
public:
    using ClassType = DelegateRemoteSend<void(Param1)>;

    DelegateRemoteSend(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) : 
        m_transport(transport), m_stream(stream), m_id(id) { }

	virtual ClassType* Clone() const override { return new ClassType(*this); }

	/// Invoke the bound delegate function. 
	virtual void operator()(Param1 p1) override {
        m_stream << m_id << std::ends;
        m_stream << p1 << std::ends;
        m_transport.DispatchDelegate(m_stream);
    }

	virtual bool operator==(const DelegateBase& rhs) const override {
		auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
		return derivedRhs &&
			m_id == derivedRhs->m_id &&
            &m_transport == &derivedRhs->m_transport; }

private:
	IDelegateTransport& m_transport;    // Object sends data to remote
    std::iostream& m_stream;            // Storage for remote message 
    DelegateIdType m_id = 0;                // Remote delegate identifier
};

template <class Param1, class Param2>
class DelegateRemoteSend<void(Param1, Param2)> : public Delegate<void(Param1, Param2)> {
public:
    using ClassType = DelegateRemoteSend<void(Param1, Param2)>;

    DelegateRemoteSend(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) :
        m_transport(transport), m_stream(stream), m_id(id) { }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Invoke the bound delegate function. 
    virtual void operator()(Param1 p1, Param2 p2) override {
        m_stream << m_id << std::ends;
        m_stream << p1 << std::ends;
        m_stream << p2 << std::ends;
        m_transport.DispatchDelegate(m_stream);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            &m_transport == &derivedRhs->m_transport;
    }

private:
    IDelegateTransport & m_transport;   // Object sends data to remote
    std::iostream& m_stream;            // Storage for remote message 
    DelegateIdType m_id = 0;                // Remote delegate identifier
};

template <class Param1, class Param2, class Param3>
class DelegateRemoteSend<void(Param1, Param2, Param3)> : public Delegate<void(Param1, Param2, Param3)> {
public:
    using ClassType = DelegateRemoteSend<void(Param1, Param2, Param3)>;

    DelegateRemoteSend(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) :
        m_transport(transport), m_stream(stream), m_id(id) { }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Invoke the bound delegate function. 
    virtual void operator()(Param1 p1, Param2 p2, Param3 p3) override {
        m_stream << m_id << std::ends;
        m_stream << p1 << std::ends;
        m_stream << p2 << std::ends;
        m_stream << p3 << std::ends;
        m_transport.DispatchDelegate(m_stream);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            &m_transport == &derivedRhs->m_transport;
    }

private:
    IDelegateTransport & m_transport;   // Object sends data to remote
    std::iostream& m_stream;            // Storage for remote message 
    DelegateIdType m_id = 0;                // Remote delegate identifier
};

template <class Param1, class Param2, class Param3, class Param4>
class DelegateRemoteSend<void(Param1, Param2, Param3, Param4)> : public Delegate<void(Param1, Param2, Param3, Param4)> {
public:
    using ClassType = DelegateRemoteSend<void(Param1, Param2, Param3, Param4)>;

    DelegateRemoteSend(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) :
        m_transport(transport), m_stream(stream), m_id(id) { }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Invoke the bound delegate function. 
    virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) override {
        m_stream << m_id << std::ends;
        m_stream << p1 << std::ends;
        m_stream << p2 << std::ends;
        m_stream << p3 << std::ends;
        m_stream << p4 << std::ends;
        m_transport.DispatchDelegate(m_stream);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            &m_transport == &derivedRhs->m_transport;
    }

private:
    IDelegateTransport & m_transport;   // Object sends data to remote
    std::iostream& m_stream;            // Storage for remote message 
    DelegateIdType m_id = 0;                // Remote delegate identifier
};

template <class Param1, class Param2, class Param3, class Param4, class Param5>
class DelegateRemoteSend<void(Param1, Param2, Param3, Param4, Param5)> : public Delegate<void(Param1, Param2, Param3, Param4, Param5)> {
public:
    using ClassType = DelegateRemoteSend<void(Param1, Param2, Param3, Param4, Param5)>;

    DelegateRemoteSend(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) :
        m_transport(transport), m_stream(stream), m_id(id) { }

    virtual ClassType* Clone() const override { return new ClassType(*this); }

    /// Invoke the bound delegate function. 
    virtual void operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) override {
        m_stream << m_id << std::ends;
        m_stream << p1 << std::ends;
        m_stream << p2 << std::ends;
        m_stream << p3 << std::ends;
        m_stream << p4 << std::ends;
        m_stream << p5 << std::ends;
        m_transport.DispatchDelegate(m_stream);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto* derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_id == derivedRhs->m_id &&
            &m_transport == &derivedRhs->m_transport;
    }

private:
    IDelegateTransport & m_transport;   // Object sends data to remote
    std::iostream& m_stream;            // Storage for remote message 
    DelegateIdType m_id = 0;                // Remote delegate identifier
};

//N=1
template <class Param1>
DelegateRemoteSend<void(Param1)> MakeDelegate(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) {
    return DelegateRemoteSend<void(Param1)>(transport, stream, id);
}

//N=2
template <class Param1, class Param2>
DelegateRemoteSend<void(Param1, Param2)> MakeDelegate(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) {
    return DelegateRemoteSend<void(Param1, Param2)>(transport, stream, id);
}

//N=3
template <class Param1, class Param2, class Param3>
DelegateRemoteSend<void(Param1, Param2, Param3)> MakeDelegate(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) {
    return DelegateRemoteSend<void(Param1, Param2, Param3)>(transport, stream, id);
}

//N=4
template <class Param1, class Param2, class Param3, class Param4>
DelegateRemoteSend<void(Param1, Param2, Param3, Param4)> MakeDelegate(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) {
    return DelegateRemoteSend<void(Param1, Param2, Param3, Param4)>(transport, stream, id);
}

//N=5
template <class Param1, class Param2, class Param3, class Param4, class Param5>
DelegateRemoteSend<void(Param1, Param2, Param3, Param4, Param5)> MakeDelegate(IDelegateTransport& transport, std::iostream& stream, DelegateIdType id) {
    return DelegateRemoteSend<void(Param1, Param2, Param3, Param4, Param5)>(transport, stream, id);
}

}

#endif
