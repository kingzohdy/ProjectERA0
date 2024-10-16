#include "tdr_define.h"
#include "tdr_types.h"
#include <stdio.h>

/**
 * @function:    generate a XML template for a given Meta
 *
 * @para[in]
 *    a_pstMeta,  ptr to Meta whose template will be output
 * @para[out]
 *    a_fpFile,   ptr to file where template will be output
 * @para[in]
 *    a_iCutOffVersion, decide if an entry will be output
 *                entries with smaller version will be output
 *    a_iXmlDataFormat, define format of template, can be
 *                         TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
 *                      or TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME 
 */

int tdr_gen_xmltmplt(IN LPTDRMETA a_pstMeta, OUT FILE *a_fpFile,
        IN int a_iCutOffVersion, IN int a_iXmlDataFormat);
