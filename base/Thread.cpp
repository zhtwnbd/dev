/******************************************************************************
 * Name         : Thread.cpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#include <assert.h>

#if defined(__WINDOWS__)
#include <windows.h>
#elif defined(__LINUX__)
#error
#endif

#include "Runtime.hpp"
#include "Thread.hpp"
using namespace dev::base;

#if defined(__WINDOWS__)
unsigned __stdcall ThreadProc(void* argp)
{
	__DV_TRY
	{
		Thread* thread = static_cast<Thread*>(argp);
		if (thread == NULL)
			throw std::exception("argp 不是 Thread 的子类");

		thread->setStatus(Thread::RUNNING);
		thread->run();
		thread->exit(NULL);
		thread->setStatus(Thread::EXIT);
	}
	__DV_CATCH(std::exception& e)
	{
		__DV_DUMP(e);
	}
	return 0;
}
#elif defined(__LINUX__)
#error
#endif

Thread::Thread()
	: tid_(-1)
	, status_(NOTREADY)
{

}

Thread::~Thread()
{

}

void Thread::start()
{
	assert(status_ == NOTREADY);

#if defined(__WINDOWS__)
	status_ = READY;
	uintptr_t retVal = ::_beginthreadex(NULL, 0, ThreadProc, this, 0, &tid_);
	if (retVal == 0 || retVal == -1)
	{
		status_ = NOTREADY;
		throw std::exception("_beginthreadex 调用失败");
	}
	thandle_ = retVal;
#elif defined(__LINUX__)
#error
#endif
}

void Thread::exit(void* retval /* = NULL */)
{
	assert(status_ == RUNNING);
}

void Thread::join()
{
	assert(status_ != READY && status_ != NOTREADY);
#if defined(__WINDOWS__)
	assert(thandle_ != 0 && thandle_ != -1);
	::WaitForSingleObject((HANDLE)thandle_, INFINITE);
	::CloseHandle((HANDLE)thandle_);
#elif defined(__LINUX__)
#error
#endif
}

Thread::id Thread::currentTid()
{
#if defined(__WINDOWS__)
	return ::GetCurrentThreadId();
#elif defined(__LINUX__)
#error
#endif
}

void Thread::sleep(int ms)
{
#if defined(__WINDOWS__)
	Sleep(ms);
#elif defined(__LINUX__)
#error
#endif
}

Thread::id Thread::getTid() const
{
	return tid_;
}

Thread::Status Thread::getStatus() const
{
	return status_;
}

void Thread::setStatus(Status status)
{
	status_ = status;
}