/*
**  @file $RCSfile: tapi_external.h,v $
**   External API definitions
**  $Id ttypes.h,v 1.0 2008/01/10 16:53:32 jackyai Exp $
**  @author jackyai $
**  @date 2008/01/10 16:53:32 $
**  @version 1.0$
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/


#ifndef  TAPI_EXTERNAL_H
#define TAPI_EXTERNAL_H

/** 这里通过TSF4G_API宏来设置各平台动态或静态库输出接口的 一些约定声明。
* 在Windows平台 生成和使用API静态库，则在本头文件之前定义TSF4G_STATIC宏；
* 使用API动态库，则在本头文件之前定义定义TSF4G_DLL_EXPORTS
*/


#if defined(WIN32) && !defined(TSF4G_STATIC)
	#ifdef TSF4G_DLL_EXPORTS
		#define TSF4G_API __declspec(dllexport)
	#else
		#define TSF4G_API extern __declspec(dllimport)
	#endif /*TSF4G_DLL_EXPORTS*/
#else
	#define TSF4G_API 
#endif  /*#if defined(WIN32) && !defined(TSF4G_STATIC)*/


#endif /*TAPI_EXTERNAL_H*/


