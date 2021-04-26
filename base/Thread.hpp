/******************************************************************************
 * Name         : Thread.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_THREAD_HPP_
#define BASE_THREAD_HPP_

#include <boost/noncopyable.hpp>
#if defined(__WINDOWS__)
#include <process.h>
#elif defined(__LINUX__)
#error
#endif

namespace dev
{
	namespace base
	{

		/* @berif 线程类
		*/
		class Thread : public boost::noncopyable
		{
		public:
#if defined(__WINDOWS__)
			typedef unsigned	id;
#elif defined(__LINUX__)
#error
#endif

			enum Status
			{
				NOTREADY = 1,
				READY,
				RUNNING,
				EXIT
			};

		public:
			Thread(void);
			virtual ~Thread(void);

		public:

			/* @brief 启动线程，失败会抛出标准异常
			*/
			void start(void);

			/* @brief 停止线程
			*/
			virtual void stop(void) = 0;

			/* @brief 线程执行函数(子类重载此函数实现线程要执行的逻辑)
			*/
			virtual void run(void) = 0;

			/* @brief 线程退出自动调用
			*/
			void exit(void* retval = NULL);

			/* @brief 等待线程退出
			*/
			void join(void);

		public:
			/* @brief 获取当前执行线程的ID
			*/
			static id currentTid(void);

			/* @brief 线程休眠
			* @param ms	休眠时间(毫秒)
			*/
			static void sleep(int ms);

			/* @brief 获取这个线程的ID
			*/
			id getTid(void) const;

			/* @brief 获取线程状态
			*/
			Status getStatus(void) const;

			/* @brief 设置线程状态
			* @param status	线程当前状态
			*/
			void setStatus(Status status);

		private:
			id tid_;
#if defined(__WINDOWS__)
			::uintptr_t thandle_;
#elif defined(__LINUX__)
#error
#endif
			Status status_;
		};
	}
}

#endif /* BASE_THREAD_HPP_ */
