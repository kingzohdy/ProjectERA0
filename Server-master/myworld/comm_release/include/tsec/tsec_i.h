/** @file $RCSfile: tsec_i.h,v $
	general description of this module
	$Id: tsec_i.h,v 1.3 2012/07/27 05:09:23 kent Exp $
@author $Author: kent $
@date $Date: 2012/07/27 05:09:23 $
@version $Revision: 1.3 $
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/


#ifndef _TSEC_I_H
#define _TSEC_I_H

#include "tsec_macros.h"

/**
  @brief
  @retval
  @retval
  @param[in] a_pszBuffer -- parameters buffer needed to be parsed, format: "key1=value1;key2=value2;..."
  @param[in,out] a_pstArgs -- struct contained parsed buffer
  @param[in] a_iTotalItem -- max parsed struct item conunts
  @note
*/
int tsec_parse_args ( IN const char *a_pszBuffer, INOUT TSECVAL *a_pstArgs, IN const int a_iTotalItem ) ;


/**
  @brief
  @param[in] a_pszKey -- key to search its value
  @param[in,out] a_pszValue -- key's valude
  @param[in] a_iValLen -- value buffer len
  @param[in] astArgs -- struct containd all keys and their values
  @retval 0 -- ok
  @retval !0 -- failed
  @note
*/
int tsec_get_value ( IN const char *a_pszKey, INOUT char *a_pszValue, IN const ssize_t a_iValLen, IN const TSECARGS astArgs ) ;




#endif /**< _TSEC_I_H */

