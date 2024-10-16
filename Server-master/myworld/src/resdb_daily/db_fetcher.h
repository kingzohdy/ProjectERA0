#ifndef DB_FETCHER
#define DB_FETCHER

#include "basictype.h"
#include "tdr/tdr.h"

namespace grav {

class DBFetcher
{
public:
	DBFetcher();

	~DBFetcher();

	void SetDBConnectionString(const std::string &value)
	{ db_conn_string_ = value; }

	void SetDBUser(const std::string &value)
	{ db_user_ = value; }

	void SetDBPasswd(const std::string &value)
	{ db_passwd_ = value; }

	bool Init(int *version, const std::string &meta_file_path);

	void SetVersion(int version) { version_ = version; }

	int NumberOfFetched() const { return number_of_fetched_; }

	bool Connect(const std::string &db_name);

	bool Test(const std::string &meta_name)
	{
		LPTDRMETA stub = tdr_get_meta_by_name(meta_lib_, meta_name.c_str());
		return stub != NULL;
	}

	bool Query(const std::string &meta_name);

	bool Fetch();

	bool Bin2Xml(std::string *xml_string);

private:
	LPTDRMETA meta_;
	LPTDRMETALIB meta_lib_;
	TDRDBHANDLE db_handle_;
	TDRDBRESULTHANDLE result_handle_;
	char *buffer_;
	int buffer_size_;
	int version_;
	int number_of_fetched_;
	std::string db_conn_string_;
	std::string db_user_;
	std::string db_passwd_;

	DISABLE_COPY_AND_ASSIGN(DBFetcher);
};

} // namespace grav

#endif // DB_FETCHER
