#ifndef _SINGLECAST_DELEGATE_H
#define _SINGLECAST_DELEGATE_H

#include "Delegate.h"

namespace DelegateLib {

// Declare SinglecastDelegate as a class template. It will be specialized for all number of arguments.
template <typename Signature>
class SinglecastDelegate;

/// @brief SinglecastDelegate holds a single Delegate<> base class. When the 
/// SinglecastDelegate is invoked the delegate instance is called.
template<class RetType>
class SinglecastDelegate<RetType(void)>
{
public:
	SinglecastDelegate() : m_delegate(0) {}
	~SinglecastDelegate() { Clear(); }

	void operator=(const Delegate<RetType(void)>& delegate) {
		Clear();
		m_delegate = delegate.Clone();	// Create a duplicate delegate
	}

	void operator=(const Delegate<RetType(void)>* delegate) {
		Clear();
		if (delegate)
			m_delegate = delegate->Clone();  // Create a duplicate delegate
	}

	RetType operator()() {
		return (*m_delegate)();	} // Invoke delegate callback

	bool Empty() const { return !m_delegate; }
	void Clear() { 
		if (m_delegate)	{
			delete m_delegate;
			m_delegate = 0; } 
	}

	explicit operator bool() const { return !Empty();  }

private:
	// Prevent copying objects
	SinglecastDelegate(const SinglecastDelegate&);
	SinglecastDelegate& operator=(const SinglecastDelegate&);

	Delegate<RetType(void)>* m_delegate;
};

template<class RetType, class Param1>
class SinglecastDelegate<RetType(Param1)>
{
public:
	SinglecastDelegate() : m_delegate(0) {}
	~SinglecastDelegate() { Clear(); }

	void operator=(const Delegate<RetType(Param1)>& delegate) {
		Clear();
		m_delegate = delegate.Clone();	// Create a duplicate delegate
	}

	void operator=(const Delegate<RetType(Param1)>* delegate) {
		Clear();
		if (delegate)
			m_delegate = delegate->Clone();  // Create a duplicate delegate
	}

	RetType operator()(Param1 p1) {
		return (*m_delegate)(p1); }	// Invoke delegate callback

	bool Empty() const { return !m_delegate; }
	void Clear() { 
		if (m_delegate)	{
			delete m_delegate;
			m_delegate = 0; } 
	}

	explicit operator bool() const { return !Empty();  }

private:
	// Prevent copying objects
	SinglecastDelegate(const SinglecastDelegate&);
	SinglecastDelegate& operator=(const SinglecastDelegate&);

	Delegate<RetType(Param1)>* m_delegate;
};

template<class RetType, class Param1, class Param2>
class SinglecastDelegate<RetType(Param1, Param2)>
{
public:
	SinglecastDelegate() : m_delegate(0) {}
	~SinglecastDelegate() { Clear(); }

	void operator=(const Delegate<RetType(Param1, Param2)>& delegate) {
		Clear();
		m_delegate = delegate.Clone();	// Create a duplicate delegate
	}

	void operator=(const Delegate<RetType(Param1, Param2)>* delegate) {
		Clear();
		if (delegate)
			m_delegate = delegate->Clone();  // Create a duplicate delegate
	}

	RetType operator()(Param1 p1, Param2 p2) {
		return (*m_delegate)(p1, p2); }	// Invoke delegate callback

	bool Empty() const { return !m_delegate; }
	void Clear() { 
		if (m_delegate)	{
			delete m_delegate;
			m_delegate = 0; } 
	}

	explicit operator bool() const { return !Empty();  }

private:
	// Prevent copying objects
	SinglecastDelegate(const SinglecastDelegate&);
	SinglecastDelegate& operator=(const SinglecastDelegate&);

	Delegate<RetType(Param1, Param2)>* m_delegate;
};

template<class RetType, class Param1, class Param2, class Param3>
class SinglecastDelegate<RetType(Param1, Param2, Param3)>
{
public:
	SinglecastDelegate() : m_delegate(0) {}
	~SinglecastDelegate() { Clear(); }

	void operator=(const Delegate<RetType(Param1, Param2, Param3)>& delegate) {
		Clear();
		m_delegate = delegate.Clone();	// Create a duplicate delegate
	}

	void operator=(const Delegate<RetType(Param1, Param2, Param3)>* delegate) {
		Clear();
		if (delegate)
			m_delegate = delegate->Clone();  // Create a duplicate delegate
	}

	RetType operator()(Param1 p1, Param2 p2, Param3 p3) {
		return (*m_delegate)(p1, p2, p3); }	// Invoke delegate callback

	bool Empty() const { return !m_delegate; }
	void Clear() { 
		if (m_delegate)	{
			delete m_delegate;
			m_delegate = 0; } 
	}

	explicit operator bool() const { return !Empty();  }

private:
	// Prevent copying objects
	SinglecastDelegate(const SinglecastDelegate&);
	SinglecastDelegate& operator=(const SinglecastDelegate&);

	Delegate<RetType(Param1, Param2, Param3)>* m_delegate;
};

template<class RetType, class Param1, class Param2, class Param3, class Param4>
class SinglecastDelegate<RetType(Param1, Param2, Param3, Param4)>
{
public:
	SinglecastDelegate() : m_delegate(0) {}
	~SinglecastDelegate() { Clear(); }

	void operator=(const Delegate<RetType(Param1, Param2, Param3, Param4)>& delegate) {
		Clear();
		m_delegate = delegate.Clone();	// Create a duplicate delegate
	}

	void operator=(const Delegate<RetType(Param1, Param2, Param3, Param4)>* delegate) {
		Clear();
		if (delegate)
			m_delegate = delegate->Clone();  // Create a duplicate delegate
	}

	RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4) {
		return (*m_delegate)(p1, p2, p3, p4); }	// Invoke delegate callback

	bool Empty() const { return !m_delegate; }
	void Clear() { 
		if (m_delegate)	{
			delete m_delegate;
			m_delegate = 0; } 
	}

	explicit operator bool() const { return !Empty();  }

private:
	// Prevent copying objects
	SinglecastDelegate(const SinglecastDelegate&);
	SinglecastDelegate& operator=(const SinglecastDelegate&);

	Delegate<RetType(Param1, Param2, Param3, Param4)>* m_delegate;
};

template<class RetType, class Param1, class Param2, class Param3, class Param4, class Param5>
class SinglecastDelegate<RetType(Param1, Param2, Param3, Param4, Param5)>
{
public:
	SinglecastDelegate() : m_delegate(0) {}
	~SinglecastDelegate() { Clear(); }

	void operator=(const Delegate<RetType(Param1, Param2, Param3, Param4, Param5)>& delegate) {
		Clear();
		m_delegate = delegate.Clone();	// Create a duplicate delegate
	}

	void operator=(const Delegate<RetType(Param1, Param2, Param3, Param4, Param5)>* delegate) {
		Clear();
		if (delegate)
			m_delegate = delegate->Clone();  // Create a duplicate delegate
	}

	RetType operator()(Param1 p1, Param2 p2, Param3 p3, Param4 p4, Param5 p5) {
		return (*m_delegate)(p1, p2, p3, p4, p5); }	// Invoke delegate callback

	bool Empty() const { return !m_delegate; }
	void Clear() { 
		if (m_delegate)	{
			delete m_delegate;
			m_delegate = 0; } 
	}

	explicit operator bool() const { return !Empty();  }

private:
	// Prevent copying objects
	SinglecastDelegate(const SinglecastDelegate&);
	SinglecastDelegate& operator=(const SinglecastDelegate&);

	Delegate<RetType(Param1, Param2, Param3, Param4, Param5)>* m_delegate;
};

}

#endif
