/******************************************************************************
 * Name         : Runtime.hpp
 * Author       : 671643387
 * Date         : 2015年9月6日
 * Description  :
 *****************************************************************************/

#ifndef BASE_RUNTIME_HPP_
#define BASE_RUNTIME_HPP_

#include <stdio.h>

// 异常处理
#define __DV_TRY		try
#define __DV_CATCH		catch
#define __DV_DUMP(x)	printf("%s,%s,%s\n", __FILE__, __FUNCTION__, (x).what());

#endif /* BASE_RUNTIME_HPP_ */
