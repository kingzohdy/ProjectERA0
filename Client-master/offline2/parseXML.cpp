#define TSF4G_STATIC
#define TDR_WITH_DBMS
#define TDR_WITH_XML
#include "parseXML.h"
#include "tdr/tdr.h"
#include "OgreLog.h"

#ifdef _DEBUG
#pragma comment(lib, "libtdr_dbms_xml_d.lib")
#else
#pragma comment(lib, "libtdr_dbms_xml.lib")
#endif


bool parseXmlData( char* metaBuf,char* input,char* output,int nSize )
{
	LPTDRMETA pMeta = tdr_get_meta_by_name( (LPTDRMETALIB)metaBuf,"CSDirWorldList" );
	if( pMeta == NULL )
	{
		return false;
	}
	TDRDATA xmldata;
	xmldata.iBuff = strlen( input );
	xmldata.pszBuff = input;
	TDRDATA dirdata;
	dirdata.pszBuff = output;
	dirdata.iBuff = nSize;
	int val = tdr_input(pMeta,&dirdata,&xmldata,0,TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME );
	if( val != 0 )
	{
		LOG_INFO("failed to parse dir xml, error string is %s\n",tdr_error_string(val) );
		return false;
	}
	return true;
}