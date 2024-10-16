#ifndef DB_PUTTER_H
#define DB_PUTTER_H

#include "basictype.h"
#include "tdr/tdr.h"

namespace grav {

class DBPutter
{
public:
	//static const int kSQLBufferLen = 4096 * 1024;

	DBPutter();

	~DBPutter();

	void SetDBConnectionString(const std::string &value)
	{ db_conn_string_ = value; }

	void SetDBUser(const std::string &value)
	{ db_user_ = value; }

	void SetDBPasswd(const std::string &value)
	{ db_passwd_ = value; }

	bool Init(int *version, const std::string &meta_file_path);

	void SetVersion(int version) { version_ = version; }

	bool Connect(const std::string &db_name);

	bool Test(const std::string &meta_name)
	{
		LPTDRMETA stub = tdr_get_meta_by_name(meta_lib_, meta_name.c_str());
		return stub != NULL;
	}

	int NumberOfInserted() const { return number_of_inserted_; }

	bool Begin(const std::string &meta_name);

	bool DeleteAll()
	{
		char sql[1024];
		_snprintf(sql, CCH(sql), "DELETE FROM %s", tdr_get_meta_name(meta_));
		return Execute(sql);
	}

	bool InsertRow();

	bool Xml2Bin(const std::string &xml_string);

	void Rollback()
	{
		Execute("ROLLBACK");
		Execute("UNLOCK TABLES");
	}

	void Commit()
	{
		Execute("COMMIT");
		Execute("UNLOCK TABLES");
	}

private:
	bool Execute(const char *sql);

	LPTDRMETA meta_;
	LPTDRMETALIB meta_lib_;
	TDRDBHANDLE db_handle_;
	TDRDBRESULTHANDLE result_handle_;
	char *buffer_;
	int buffer_size_;
	int version_;
	int number_of_inserted_;
	std::string db_conn_string_;
	std::string db_user_;
	std::string db_passwd_;

	DISABLE_COPY_AND_ASSIGN(DBPutter);
};

} // namespace grav

#endif // DB_PUTTER_H
