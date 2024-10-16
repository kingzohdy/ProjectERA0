#define TSF4G_STATIC
#define TDR_WITH_DBMS
#define TDR_WITH_XML
#include "parseXML.h"
#include "tdr/tdr.h"
#ifdef _DEBUG
#pragma comment(lib, "libtdr_dbms_xml_d.lib")
#else
#pragma comment(lib, "libtdr_dbms_xml.lib")
#endif


bool parseXmlData( char* metaBuf, char* metaName, char* input,char* output,int nSize )
{
	LPTDRMETA pMeta = tdr_get_meta_by_name( (LPTDRMETALIB)metaBuf,metaName );
	if( pMeta == NULL )
	{
		return false;
	}
	TDRDATA xmldata;
	xmldata.iBuff = strlen( input );
	xmldata.pszBuff = input;
	TDRDATA outdata;
	outdata.pszBuff = output;
	outdata.iBuff = nSize;
	int val = tdr_input(pMeta,&outdata,&xmldata,0,TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME );
	if( val != 0 )
	{
		return false;
	}
	return true;
}