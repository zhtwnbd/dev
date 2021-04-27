/******************************************************************************
 * Name         : Runtime.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_RUNTIME_HPP_
#define BASE_RUNTIME_HPP_

#include <stdio.h>

// 调试相关
#if defined(_DEBUG)
#define __DV_ENTER  printf("enter %s,%s\n", __FILE__, __FUNCTION__);
#define __DV_LEAVE  printf("leave %s,%s\n", __FILE__, __FUNCTION__);
#else
#define __DV_ENTER
#define __DV_LEAVE
#endif

// 异常处理
#define __DV_TRY		try
#define __DV_CATCH		catch
#define __DV_DUMP(x)	printf("%s,%s,%s\n", __FILE__, __FUNCTION__, (x).what());

#endif /* BASE_RUNTIME_HPP_ */
