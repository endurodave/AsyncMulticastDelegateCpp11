#ifndef _MULTICAST_DELEGATE_BASE_H
#define _MULTICAST_DELEGATE_BASE_H

#include "Delegate.h"
#include <list>

namespace DelegateLib {

/// @brief A non-template base class for the multicast delegates. 
class MulticastDelegateBase
{
public:
	/// Constructor
	MulticastDelegateBase() = default;

	/// Destructor
	virtual ~MulticastDelegateBase() { Clear(); }

	/// Any registered delegates?
	bool Empty() const { return m_delegates.empty(); }

	/// Removal all registered delegates.
	void Clear();

    explicit operator bool() const { return !Empty(); }

	/// Insert a delegate into the invocation list. A delegate argument 
	/// pointer is not stored. Instead, the DelegateBase derived object is 
	/// copied (cloned) and saved in the invocation list.
	/// @param[in] delegate - a delegate to register. 
	void operator+=(const DelegateBase& delegate);

	/// Remove a delegate previously registered delegate from the invocation
	/// list. 
	/// @param[in] delegate - a delegate to unregister. 
	void operator-=(const DelegateBase& delegate);

protected:
	/// Get the head of the delegate invocation list. 
	/// @return Pointer to the head of the invocation list. 
	const std::list<DelegateBase*>& Delegates() const { return m_delegates; }

private:
	// Prevent copying objects
	MulticastDelegateBase(const MulticastDelegateBase&) = delete;
	MulticastDelegateBase& operator=(const MulticastDelegateBase&) = delete;

	/// List of registered delegates
	std::list<DelegateBase*> m_delegates;
};

}

#endif
