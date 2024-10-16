/** @file $RCSfile: tsec_ip.h,v $
  general description of this module
  $Id: tsec_ip.h,v 1.3 2012/07/27 05:09:23 kent Exp $
@author $Author: kent $
@date $Date: 2012/07/27 05:09:23 $
@version $Revision: 1.3 $
@note Editor: Vim 6.3, Gcc 4.0.2, tab=4
@note Platform: Linux
*/


#ifndef _TSEC_IP_H
#define _TSEC_IP_H


#define TSEC_IP_CUR_VERSION 1

#define TSEC_IPACL_META "IPACL"
#define TSEC_IP_ALLOW   "ALLOW"
#define TSEC_IP_DENY    "DENY"

#define TSEC_MAX_IP_RULES 10240

#define TSEC_IP_ALLOW_IDX 0
#define TSEC_IP_DENY_IDX  -1


typedef struct st_tsec_iprule {
  unsigned long llSrcStart ;
  unsigned long llSrcEnd ;

  unsigned long llDstStart ;
  unsigned long llDstEnd ;

  int iLimit ; /**< TSEC_IP_ALLOW, TSEC_IP_DENY, >0-max connection */

  int iCurCnt ; /**< current source IP connections count */
} TSECIPRULE ;


typedef struct st_tsec_ipacl {
  int iDefault ; /**< TSEC_IP_ALLOW, TSEC_IP_DENY */

  int iLimitCnt;
  TSECIPRULE stLimitIPs[TSEC_MAX_IP_RULES];

} TSECIPACL ;


/*@{ @name ip verify function key

  description of defined macro
*/
#define IP_VERIFY_TYPE "type"
#define IP_SRC         "srcip"
#define IP_DST         "dstip"
#define IP_RULEFILE    "rulesxml"
#define IP_DESCFILE    "descxml"
/*@}*/


/*@{ @name ip verify function method

  description of defined macro
*/
#define TSEC_IP_REG   "reg"
#define TSEC_IP_UNREG "unreg"
#define TSEC_IP_RESET "reset"
#define TSEC_IP_SRCIP "srcip"
#define TSEC_IP_DSTIP "dstip"
/*@}*/

/*@{ @name ip connection counter macro

  description of defined macro
*/
#define IP_ADD_COUNTER 0
#define IP_DEC_COUNTER 1
/*@}*/

#endif /**< _TSEC_IP_H */

