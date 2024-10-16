#include "db_fetcher.h"

namespace grav {

DBFetcher::DBFetcher()
	: meta_(NULL)
	, meta_lib_(NULL)
	, db_handle_(NULL)
	, result_handle_(NULL)
	, buffer_(NULL)
	, buffer_size_(0)
	, version_(0)
	, number_of_fetched_(0)
{
}

DBFetcher::~DBFetcher()
{
	tdr_close_dbhanlde(&db_handle_);
	delete[] buffer_;
}

bool DBFetcher::Init(int *version, const std::string &meta_file_path)
{
	if(tdr_load_metalib(&meta_lib_, meta_file_path.c_str()) < 0)
	{
		std::fprintf(stderr, "Can not open meta library.\n");
		return false;		
	}

	version_ = tdr_get_metalib_version(meta_lib_);
	if(version_ < 0)
		return false;

	*version = version_;
	return true;
}

bool DBFetcher::Connect(const std::string &db_name)
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
	if(tdr_open_dbhanlde(&db_handle_, &dbms, &error_message[0]) < 0)
	{
		std::fprintf(stderr, "Can not open dbhandle, reason: %s\n",
		        error_message.c_str());
		return false;
	}
	return true;
}

bool DBFetcher::Query(const std::string &meta_name)
{
	meta_ = tdr_get_meta_by_name(meta_lib_, meta_name.c_str());
	if(!meta_)
		return false;
	buffer_size_ = tdr_get_meta_size(meta_);
	if(buffer_size_ < 0)
		return false;
	buffer_ = new char[buffer_size_];

	std::string sql;
	sql.resize(1024, 0);
	int len = snprintf(&sql[0], sql.length(), "SELECT * FROM %s WHERE 1",
	                   meta_name.c_str());
	sql.resize(len);

	TDRDATA sql_data;
	sql_data.iBuff   = sql.length();
	sql_data.pszBuff = &sql[0];
	if(tdr_query_quick(&result_handle_, db_handle_, &sql_data) < 0)
		return false;

	number_of_fetched_ = 0;
	return true;
}

bool DBFetcher::Fetch()
{
	TDRDBOBJECT db_object;

	db_object.iVersion = version_;
	db_object.pstMeta = meta_;
	db_object.pszObj = buffer_;
	db_object.iObjSize = buffer_size_;

	int rv = tdr_fetch_row(result_handle_, &db_object);
	if(rv < 0)
		return false;

	++number_of_fetched_;
	return true;
}

bool DBFetcher::Bin2Xml(std::string *xml_string)
{
	xml_string->clear();
	xml_string->resize(40960, 0);

	TDRDATA out;
	out.iBuff = static_cast<int>(xml_string->length());
	out.pszBuff = &xml_string->at(0);

	TDRDATA host;
	host.iBuff = buffer_size_;
	host.pszBuff = buffer_;
	if(tdr_sprintf(meta_, &out, &host, version_) < 0)
	{
		xml_string->assign("<error>");
		return false;
	}
	xml_string->resize(std::strlen(out.pszBuff));
	return true;
}

} // namespace grav
