#ifndef DB_EXPORTER_H
#define DB_EXPORTER_H

#include "basictype.h"
#include "mysql/mysql.h"
#include <sstream>

namespace grav {

class DBConnection
{
public:
	DBConnection() : conn_(NULL) {}

	~DBConnection() { if(conn_) mysql_close(conn_); }

	void SetDBConnectionString(const std::string &value)
	{ db_conn_string_ = value; }

	void SetDBUser(const std::string &value)
	{ db_user_ = value; }

	void SetDBPasswd(const std::string &value)
	{ db_passwd_ = value; }

	bool Init()
	{
		conn_ = mysql_init(NULL);
		return !!conn_;
	}

	bool Connect(const std::string &db_name)
	{
		MYSQL *rv = mysql_real_connect(
			conn_, db_conn_string_.c_str(), db_user_.c_str(),
			db_passwd_.c_str(), db_name.c_str(), 0, NULL,
			CLIENT_COMPRESS);
		return !!rv;
	}

	bool Execute(const std::string &sql)
	{
		int err = mysql_query(conn_, sql.c_str());
		return err ? false : true;
	}

	bool Begin(const std::string &table_name)
	{
		char sql[1024];
		_snprintf(sql, CCH(sql), "LOCK TABLES %s WRITE", table_name.c_str());
		if(!Execute(sql))
			return false;
		if(!Execute("START TRANSACTION"))
		{
			Execute("UNLOCK TABLES");
			return false;
		}
		return true;
	}

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

protected:
	MYSQL *conn_;

private:
	std::string db_conn_string_;
	std::string db_user_;
	std::string db_passwd_;
};

///////////////////////////////////////////////////////////////////////////////////////////////

class DBImporter : public DBConnection
{
public:
	DBImporter();

	~DBImporter();

	bool Begin() { return DBConnection::Begin(table_name_); }

	bool Drop()
	{
		char sql[1024];
		_snprintf(sql, CCH(sql), "DELETE FROM %s", table_name_.c_str());
		return Execute(sql);
	}

	void SetTableName(const std::string &name) { table_name_ = name; }

	bool ImportFromCSV(std::istream *stream);

	bool Succeeded() const { return succeeded_; }

private:
	std::string table_name_;
	bool succeeded_;
};


///////////////////////////////////////////////////////////////////////////////////////////////

class DBExporter : public DBConnection
{
public:
	DBExporter();

	~DBExporter();

	bool Query(const std::string &name)
	{
		std::string sql = "SELECT * FROM " + name;
		int err = mysql_query(conn_, sql.c_str());
		if(err)
			return false;
		res_ = mysql_use_result(conn_);
		return !!res_;
	}

	bool Fetch()
	{
		row_ = mysql_fetch_row(res_);
		return !!row_;
	}

	bool Fields2CSV(const std::string &name,std::ostream *stream)
	{
		unsigned int num_fields;
		unsigned int i;
		MYSQL_FIELD *fields;
		MYSQL_RES *result;
			
		result = mysql_list_fields(conn_, name.c_str(), NULL);
		if(!result)
		{
			return false;
		}

		num_fields = mysql_num_fields(result);
		fields = mysql_fetch_fields(result);

		for(i = 0; i < num_fields; i++)
		{
			if(i)
				*stream << ',';

			*stream << fields[i].name;
		}

		mysql_free_result(result);

		if(num_fields) *stream << '\n';

		return !!num_fields;
	}

	bool Row2CSV(std::string *csv)
	{
		std::ostringstream stream;
		if(!Export2CSV(&stream))
			return false;
		*csv = stream.str();
		return true;
	}

	bool Export2CSV(std::ostream *stream);

	bool Export2TXT(std::ostream *stream);

private:
	MYSQL_RES *res_;
	MYSQL_ROW row_;
};

} // namespace grav

#endif // DB_EXPORTER_H
