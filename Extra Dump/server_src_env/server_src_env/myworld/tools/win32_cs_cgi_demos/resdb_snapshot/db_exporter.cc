#include "win32utils.h"
#include "db_exporter.h"
#include <ctype.h>


namespace grav {

namespace detail {

inline bool HasSpecial(const char *p)
{
	while(*p)
	{
		switch(*p)
		{
		case '\r': case '\n': case '\t': case ',':
			return true;
		}
		++p;
	}
	return false;
}

} // namespace detail

///////////////////////////////////////////////////////////////////////////////////////////////
class CSVSplitter
{
public:
	CSVSplitter(std::istream *stream, std::string *res, MYSQL *conn)
		: stream_(stream)
		, res_(res)
		, conn_(conn)
		, number_of_row_(0)
		, succeeded_(true) {}

	~CSVSplitter() {}

	bool Next()
	{
		for(;;)
		{
			int ch = Peek();
			if(ch < 0)
				return false;

			switch(ch)
			{
			case ' ':
				if(!ReadRow())
					return false;
				break;
			case '\"':
				if(!ReadString())
					return false;
				break;
			case ',':
				if(!ReadNullIfExist())
					return false;
				break;

			case '\r':
				if(Read() != '\r') return Fail();
				if(Read() != '\n') return Fail();
				goto out;

			case '\n':
				if(Read() != '\n') return Fail();
				goto out;

			default:
				if(isspace(ch))
					Read();
				else if(!ReadRow())
					return false;
				break;
			}
		}
	out:
		succeeded_ = stream_->good();
		return true;
	}

	bool Succeeded() const { return succeeded_; }

private:
	int Read() { return stream_->get(); }

	int Peek() { return stream_->peek(); }

	bool ReadNullIfExist ()
	{
		if(Read() != ',')
			return Fail();

		if(!number_of_row_)
		{
			res_->append("NULL");
			++number_of_row_;
			goto out;
		}

		for(;;)
		{
			int ch = Peek();
			if(ch < 0)
				return false;

			switch(ch)
			{
			case '\"':
				goto out;

			case '\r':
				if(Read() != '\r') return Fail();
				if(Peek() != '\n') return Fail();
			case '\n':
			case ',':
				if(number_of_row_)
					res_->append(1, ',');
				res_->append("NULL");
				++number_of_row_;
				goto out;

			default:
				if(isspace(ch))
					goto out;
				else
					goto out;
				break;
			}
		}
	out:
		succeeded_ = stream_->good();
		return true;
	}

	bool ReadRow()
	{
		if(number_of_row_)
			res_->append(1, ',');
		res_->append(1, '\'');

		std::string raw;
		for(;;)
		{
			int ch = Peek();
			if(ch < 0)
				return false;

			switch(ch)
			{
			case ',': case '\n':
				goto out;

			case '\r':
				if(Read() != '\r') return Fail();
				if(Peek() != '\n') return Fail();
				goto out;

			default:
				raw.append(1, Read());
				break;
			}
		}
	out:
		res_->append(BuildSQLEscapeString(raw));
		res_->append(1, '\'');
		succeeded_ = stream_->good();
		++number_of_row_;
		return true;
	}

	bool ReadString()
	{
		if(number_of_row_)
			res_->append(1, ',');
		if(Read() != '\"')
			return Fail();
		res_->append(1, '\'');

		std::string raw;
		for(;;)
		{
			int ch = Peek();
			if(ch < 0)
				return false;

			switch(ch)
			{
			case '\"':
				if(Read() != '\"')
					return Fail();
				if(Peek() != '\"')
					goto out;
				if(Read() != '\"')
					return Fail();
				raw.append(1, '\"');
				break;

			default:
				raw.append(1, Read());
				break;
			}
		}
	out:
		res_->append(BuildSQLEscapeString(raw));
		res_->append(1, '\'');
		succeeded_ = stream_->good();
		++number_of_row_;
		return true;
	}

	std::string BuildSQLEscapeString(const std::string &raw)
	{
		if(raw.empty())
			return std::string();

		std::string buf;
		buf.resize(raw.length() * 2 + 1);

		std::string::size_type size = mysql_real_escape_string(
			conn_, &buf[0], raw.c_str(), raw.length());
		buf.resize(size);
		return buf;
	}

	bool Fail() { succeeded_ = false; return false; }

	bool succeeded_;
	std::istream *stream_;
	std::string *res_;
	MYSQL *conn_;
	int number_of_row_;
};

DBImporter::DBImporter()
	: succeeded_(false)
{
}

DBImporter::~DBImporter()
{
}

bool DBImporter::ImportFromCSV(std::istream *stream)
{
	std::string sql;
	sql.append("INSERT INTO ");
	sql.append(table_name_);
	sql.append(" VALUES (");

	CSVSplitter splitter(stream, &sql, conn_);
	bool rv = splitter.Next();
	succeeded_ = splitter.Succeeded();
	if(!rv)
		return false;

	sql.append(")");
	succeeded_ = Execute(sql);
	return succeeded_;
}

///////////////////////////////////////////////////////////////////////////////////////////////

std::string BuildCSVLine(const std::string &row)
{
	std::string res;
	bool injected = row.empty();

	for(std::string::const_iterator iter = row.begin(); iter != row.end();
	    ++iter)
	{
		char ch = *iter;
		switch(ch)
		{
		case '\"':
			res.append("\"\"");
			injected = true;
			break;

		case ',':
			res.append(1, ',');
			injected = true;
			break;

		default:
			if(isspace(ch))
				injected = true;
			res.append(1, ch);
			break;
		}
	}
	if(injected)
	{
		res.insert(0, 1, '\"');
		res.append(1, '\"');
	}
	return res;
}

DBExporter::DBExporter()
	: res_(NULL)
	, row_(NULL)
{
}

DBExporter::~DBExporter()
{
	if(res_) mysql_free_result(res_);
}

bool DBExporter::Export2CSV(std::ostream *stream)
{
	const unsigned int n = mysql_num_fields(res_);
	for(unsigned int i = 0; i < n; ++i)
	{
		if(!row_[i])
		{
			*stream << ",";
			continue;
		}

		if(i)
			*stream << ',';
		*stream << BuildCSVLine(row_[i]);
	}
	if(n) *stream << '\n';
	return !!n;
}

bool DBExporter::Export2TXT(std::ostream *stream)
{
	const unsigned int n = mysql_num_fields(res_);
	for(unsigned int i = 0; i < n; ++i)
	{
		if(!row_[i])
		{
			*stream << "\t";
			continue;
		}

		if(detail::HasSpecial(row_[i]))
			*stream << '\"' << row_[i] << "\"\t";
		else
			*stream << row_[i] << '\t';
	}
	if(n) *stream << '\n';
	return !!n;
}

} // namespace grav