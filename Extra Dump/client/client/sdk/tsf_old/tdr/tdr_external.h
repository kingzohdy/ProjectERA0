/**
*
* @file     tdr_external.h 
* @brief    输出API定义
* 
* @author steve jackyai  
* @version 1.0
* @date 2008-01-07 
*
*
* Copyright (c)  2007-2008, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef  TDR_EXTERNAL_H
#define TDR_EXTERNAL_H


#ifdef TAPI_EXTERNAL_H
	#define TDR_API TSF4G_API
#else

	#if defined(WIN32) && !defined(TSF4G_STATIC)
		#ifdef TSF4G_DLL_EXPORTS
			#define TDR_API __declspec(dllexport)
		#else
			#define TDR_API extern __declspec(dllimport)
		#endif /*TSF4G_DLL_EXPORTS*/
	#else
		#define TDR_API 
	#endif  /*#if defined(WIN32) && !defined(TSF4G_STATIC)*/

#endif /*TDR_EXTERNAL_H*/


  
#endif /*TDR_EXTERNAL_H*/


