#include "DelegateAsync.h"

namespace DelegateLib {

DelegateFreeAsync<void(void)> MakeDelegate(void (*func)(), DelegateThread* thread) { 
	return DelegateFreeAsync<void(void)>(func, thread);
}

}