#include "db_putter.h"

namespace grav {

/*static*/ const int kSQLBufferLen = 4096 * 1024;

DBPutter::DBPutter()
	: meta_(NULL)
	, meta_lib_(NULL)
	, db_handle_(NULL)
	, result_handle_(NULL)
	, buffer_(NULL)
	, buffer_size_(0)
	, version_(0)
	, number_of_inserted_(0)
{
}

DBPutter::~DBPutter()
{
	tdr_close_dbhanlde(&db_handle_);
	delete[] buffer_;
}

bool DBPutter::Init(int *version, const std::string &meta_file_path)
{
	if(tdr_load_metalib(&meta_lib_, meta_file_path.c_str()) < 0)
		return false;

	version_ = tdr_get_metalib_version(meta_lib_);
	if(version_ < 0)
		return false;

	*version = version_;
	return true;
}

bool DBPutter::Connect(const std::string &db_name)
{
	TDRDBMS dbms;

	std::memset(&dbms, 0, sizeof(dbms));
	std::strncpy(dbms.szDBMSName, TDR_DEFAULT_DBMS, CCH(dbms.szDBMSName));
	std::strncpy(dbms.szDBMSCurDatabaseName, db_name.c_str(),
	             CCH(dbms.szDBMSCurDatabaseName));
	std::strncpy(dbms.szDBMSConnectionInfo, db_conn_string_.c_str(),
	             CCH(dbms.szDBMSConnectionInfo));
	std::strncpy(dbms.szDBMSUser, db_user_.c_str(), CCH(dbms.szDBMSUser));
	std::strncpy(dbms.szDBMSPassword, db_passwd_.c_str(),
		         CCH(dbms.szDBMSPassword));
	dbms.iReconnectOpt = 1;

	std::string error_message;
	error_message.resize(1024, 0);
	int rv = tdr_open_dbhanlde(&db_handle_, &dbms, &error_message[0]);
	return rv < 0 ? false : true;
}

bool DBPutter::Begin(const std::string &meta_name)
{
	meta_ = tdr_get_meta_by_name(meta_lib_, meta_name.c_str());
	if(!meta_)
		return false;
	buffer_size_ = tdr_get_meta_size(meta_);
	if(buffer_size_ < 0)
		return false;
	delete[] buffer_;
	buffer_ = new char[buffer_size_];

	char sql[1024];
	_snprintf(sql, CCH(sql), "LOCK TABLES %s WRITE", meta_name.c_str());
	if(!Execute(sql))
		return false;
	if(!Execute("START TRANSACTION"))
	{
		Execute("UNLOCK TABLES");
		return false;
	}

	number_of_inserted_ = 0;
	return true;
}

bool DBPutter::Xml2Bin(const std::string &xml_string)
{
	TDRDATA out;
	out.iBuff = buffer_size_;
	out.pszBuff = buffer_;

	TDRDATA host;
	host.iBuff = static_cast<int>(xml_string.length());
	host.pszBuff = const_cast<char*>(&xml_string[0]);

	int rv = tdr_input(meta_, &out, &host, version_, TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME);
	return rv < 0 ? false : true;
}

bool DBPutter::InsertRow()
{
	char *buf = new char[kSQLBufferLen];

	TDRDATA sql;
	TDRDBOBJECT obj;

	sql.iBuff = kSQLBufferLen;
	sql.pszBuff = buf;

	obj.iVersion = version_;
	obj.iObjSize = buffer_size_;
	obj.pstMeta = meta_;
	obj.pszObj = buffer_;

	int rv = tdr_obj2sql(db_handle_, &sql, TDR_DBOP_INSERT, &obj, NULL);
	if(rv < 0 || !Execute(buf))
	{
		delete[] buf;
		return false;
	}
	++number_of_inserted_;
	delete[] buf;
	return true;
}

bool DBPutter::Execute(const char *sql)
{
	TDRDATA sql_data;
	sql_data.iBuff   = strlen(sql) + 1;
	sql_data.pszBuff = const_cast<char*>(sql);

	int rv = tdr_query_quick(&result_handle_, db_handle_, &sql_data);
	tdr_free_dbresult(&result_handle_);
	return rv < 0 ? false : true;
}

} // namespace grav
