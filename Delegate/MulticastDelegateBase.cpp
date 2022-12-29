#include "MulticastDelegateBase.h"

namespace DelegateLib {

//------------------------------------------------------------------------------
// operator+=
//------------------------------------------------------------------------------
void MulticastDelegateBase::operator+=(const DelegateBase& delegate)
{
	m_delegates.push_back(delegate.Clone());
}

//------------------------------------------------------------------------------
// operator-=
//------------------------------------------------------------------------------
void MulticastDelegateBase::operator-=(const DelegateBase& delegate)
{
	for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it)
	{
		if (*(&delegate) == *((*it)))
		{
			delete (*it);
			m_delegates.erase(it);
			break;
		}
	}
}

//------------------------------------------------------------------------------
// Clear
//------------------------------------------------------------------------------
void MulticastDelegateBase::Clear()
{
	auto it = m_delegates.begin();
	while (it != m_delegates.end())
	{
		delete (*it);
		it = m_delegates.erase(it);
	}
}

}

