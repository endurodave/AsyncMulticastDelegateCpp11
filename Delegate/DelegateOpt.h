#ifndef _DELEGATE_OPT_H
#define _DELEGATE_OPT_H

// Define USE_CXX17 to enable C++17 specific delegate features; otherwise C++11 feature set is used.
// Define either USE_WIN32_THREADS or USE_STD_THREADS to specify WIN32 or std::thread threading model.
// Define USE_XALLOCATOR to use fixed block memory allocation.

#define USE_CXX17

#if _MSC_VER >= 1700
	#define USE_STD_THREADS	1
	//#define USE_WIN32_THREADS 1 
#elif _MSC_VER >= 1500
	#error Unsupported Visual Studio version 
#endif

#if __GNUC__ >= 5
	#define USE_STD_THREADS	1
#elif __GNUC__ == 4 
	#error Unsupported GCC version
#endif

#if defined(USE_STD_THREADS) && defined(USE_WIN32_THREADS)
	#error Define only one. Either USE_STD_THREADS or USE_WIN32_THREADS
#endif

#if !defined(USE_STD_THREADS) && !defined(USE_WIN32_THREADS)
	#error Must define either USE_STD_THREADS or USE_WIN32_THREADS
#endif

#if defined(__GNUC__) && defined(USE_WIN32_THREADS)
	#error GCC does not support WIN32 API. Define USE_STD_THREADS.
#endif

// To make the delegate library use a fixed block memory allocator uncomment the include
// line below and the XALLOCATOR line. This could speed new/delete operations and eliminates
// the possibility of a heap fragmentation fault. Use is completely optional. 
// @see https://www.codeproject.com/Articles/1084801/Replace-malloc-free-with-a-Fast-Fixed-Block-Memory
//#define USE_XALLOCATOR 1

#endif
