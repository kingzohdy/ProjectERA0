/**
*
* @file     tdr.h 
* @brief    TDR主头文件
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-03-26 
*
*
* Copyright (c)  2007-2008, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TDR_H
#define TDR_H

/* automatically include the correct library on windows */
#ifdef WIN32

# ifdef _DEBUG
#  define TDR_LIB_D    "_d"
# else
#  define TDR_LIB_D
# endif /* _DEBUG */

#ifdef TDR_WITH_DBMS
#define TDR_LIB_DBMS	"_dbms"
#else
#define TDR_LIB_DBMS
#endif

#ifdef TDR_WITH_XML
#define TDR_LIB_XML	"_xml"

#ifdef _DEBUG
#pragma comment(lib, "libexpat"TDR_LIB_D".lib")
#else
#pragma comment(lib, "libexpat.lib")
#endif

#else
#define TDR_LIB_XML
#endif /*TDR_WITH_XML*/


# if defined(TDR_LIB_D) || defined(TDR_LIB_DBMS) || defined(TDR_LIB_XML)
# pragma comment( lib, "libtdr"  TDR_LIB_DBMS TDR_LIB_XML TDR_LIB_D ".lib" )
# else
# pragma comment( lib, "libtdr.lib" )
# endif


#ifdef USE_TSF4G_PAL
#pragma comment(lib, "tpal.lib")
#endif

#endif

#include "tdr/tdr_external.h"
#include "tdr/tdr_types.h"
#include "tdr/tdr_ctypes_info.h"
#include "tdr/tdr_define.h"
#include "tdr/tdr_error.h"
#include "tdr/tdr_metalib_init.h"
#include "tdr/tdr_XMLtags.h"
#include "tdr/tdr_XMLMetaLib.h"
#include "tdr/tdr_metalib_to_hpp.h"
#include "tdr/tdr_data_io.h"
#include "tdr/tdr_data_sort.h"
#include "tdr/tdr_net.h"
#include "tdr/tdr_operate_data.h"
#include "tdr/tdr_metalib_manage.h"
#include "tdr/tdr_sql.h"
#include "tdr/tdr_meta_entries_index.h"


#endif /* TDR_H */
