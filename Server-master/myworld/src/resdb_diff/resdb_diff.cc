#include "tdr/tdr.h"
#include <sys/stat.h>
#include <errno.h>
#include <time.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <map>
#include <functional>


// Utils:
#define DISABLE_COPY_AND_ASSIGN(name) \
	name(const name&); \
	void operator = (const name&)

template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define CCH(array) (sizeof(ArraySizeHelper(array)))

#define OFFSET_OF(base, member) \
	(reinterpret_cast<ptrdiff_t>(&reinterpret_cast<base*>(16)->member) - 16)

template<class T>
inline void ignore_result(T) {}

template<class T>
inline void delete_and_clear_container(T *array)
{
	for(typename T::iterator iter = array->begin();
	    iter != array->end(); ignore_result(++iter))
		delete (*iter);
	array->clear();
}


namespace {

class ConfPref;

ConfPref *top_conf_ = NULL;

const char kOriginDBName[] = "resdb";
const char kConfFilePath[] = "resdb_diff.conf";

void Usage()
{
	std::puts("Print a table's diff in resdb");
	std::puts("Usage:");
	std::puts("resdb_diff dbname metafile");
}

class ConfPref
{
public:
	static const int kMaxArgs = 3;

	ConfPref(int)
		: table_names_(NULL)
		, number_of_table_names_(0)
	{
		assert(!top_conf_);
		top_conf_ = this;
	}

	~ConfPref()
	{
		assert(top_conf_);
		delete [] table_names_;
		top_conf_ = NULL;
	}

	bool Parse(int argc, char *argv[])
	{
		if(argc < kMaxArgs)
			return false;
		if(!Load())
			return false;
		int i = 1;
		db_name_ = argv[i++];
		meta_file_ = argv[i++];
		table_names_ = new std::string[argc - i];
		for(; i < argc; ++i)
			table_names_[number_of_table_names_++] = argv[i];
		MakePath();
		return true;
	}

	static const ConfPref* Local() { return top_conf_; }

	const std::string& DBName() const { return db_name_; }
	const std::string& DBHostName() const { return db_connect_; }
	const std::string& DBUser() const { return db_user_; }
	const std::string& DBPassword() const { return db_passwd_; }
	const std::string& OriginMetaFilePath() const { return current_meta_file_; }
	const std::string& MetaFilePath() const { return meta_file_; }
	const std::string& OutputDirectory() const { return output_directory_; }
	const std::string& OutputPrefix() const { return output_prefix_; }

	const std::string& TableNames(int index) const
	{ return table_names_[index]; }

	int NumberOfTableNames() const { return number_of_table_names_; }

private:
	struct ConfEntry
	{
		const char *name;
		ptrdiff_t offset;
	};

	void MakePath()
	{
		time_t jiff = time(NULL);
		struct tm current;
		localtime_r(&jiff, &current);

		std::string stmp;
		stmp.resize(1024);
		int len = std::snprintf(
			&stmp[0], stmp.length(), "%04d%02d%02d%02d%02d%02d",
			current.tm_year + 1900, current.tm_mon + 1, current.tm_mday,
			current.tm_hour, current.tm_min, current.tm_sec);
		stmp.resize(len);
		output_prefix_ = OutputDirectory() + "diff_" + stmp;
	}

	bool Load()
	{
		std::FILE *fp = std::fopen(kConfFilePath, "r");
		if(!fp)
			return false;
		char buf[128];
		while(std::fscanf(fp, "%s", buf) != EOF)
		{
			bool found = false;
			const ConfEntry *i = entries_;
			for(; i->name != NULL; ++i)
			{
				if(std::strcmp(buf, i->name) == 0)
				{
					found = true;
					break;
				}
			}
			if(!found)
				continue;
			if(std::fscanf(fp, "%s", buf) == EOF)
				break;
			reinterpret_cast<std::string*>(
				reinterpret_cast<char*>(this) + i->offset)->assign(buf);
		}
		std::fclose(fp);
		return true;
	}

	std::string *table_names_;
	int number_of_table_names_;
	std::string output_prefix_;
	std::string db_name_;
	std::string db_connect_;
	std::string db_user_;
	std::string db_passwd_;
	std::string current_meta_file_;
	std::string meta_file_;
	std::string output_directory_;
	static const ConfEntry entries_[];

	DISABLE_COPY_AND_ASSIGN(ConfPref);
};

const ConfPref::ConfEntry ConfPref::entries_[] =
{
	{ "db_connect",        OFFSET_OF(ConfPref, db_connect_), },
	{ "db_user",           OFFSET_OF(ConfPref, db_user_), },
	{ "db_passwd",         OFFSET_OF(ConfPref, db_passwd_), },
	{ "current_meta_file", OFFSET_OF(ConfPref, current_meta_file_), },
	{ "output_directory",  OFFSET_OF(ConfPref, output_directory_), },
	{ NULL, -1, },
};

class DBFetcher
{
public:
	DBFetcher()
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

	~DBFetcher()
	{
		tdr_close_dbhanlde(&db_handle_);
		delete[] buffer_;
	}

	bool Init(int *version, const std::string &meta_file_path)
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

	void SetVersion(int version) { version_ = version; }

	int NumberOfFetched() const { return number_of_fetched_; }

	bool Connect(const std::string &db_name)
	{
		TDRDBMS dbms;

		std::memset(&dbms, 0, sizeof(dbms));
		std::strncpy(dbms.szDBMSName, TDR_DEFAULT_DBMS,
		             CCH(dbms.szDBMSName));
		std::strncpy(dbms.szDBMSCurDatabaseName,
			         db_name.c_str(),
		             CCH(dbms.szDBMSCurDatabaseName));
		std::strncpy(dbms.szDBMSConnectionInfo,
		             ConfPref::Local()->DBHostName().c_str(),
		             CCH(dbms.szDBMSConnectionInfo));
		std::strncpy(dbms.szDBMSUser,
		             ConfPref::Local()->DBUser().c_str(),
		             CCH(dbms.szDBMSUser));
		std::strncpy(dbms.szDBMSPassword,
		             ConfPref::Local()->DBPassword().c_str(),
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

	bool Test(const std::string &meta_name)
	{
		LPTDRMETA stub = tdr_get_meta_by_name(meta_lib_, meta_name.c_str());
		return stub != NULL;
	}

	bool Query(const std::string &meta_name)
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

	bool Fetch()
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

	bool Bin2Xml(std::string *xml_string)
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

private:
	LPTDRMETA meta_;
	LPTDRMETALIB meta_lib_;
	TDRDBHANDLE db_handle_;
	TDRDBRESULTHANDLE result_handle_;
	char *buffer_;
	int buffer_size_;
	int version_;
	int number_of_fetched_;

	DISABLE_COPY_AND_ASSIGN(DBFetcher);
};


class DifferenceArray
{
public:
	typedef std::vector<const std::string*> DiffSequanceType;
	typedef std::multimap<int, int> DiffMapType;

	DifferenceArray(DiffSequanceType *sequance)
		: xml_string_seq_(sequance)
	{}

	~DifferenceArray() {}

	int NumberOfDifference() const
	{
		return count_if(xml_string_seq_->begin(), xml_string_seq_->end(),
		                std::bind2nd(std::not_equal_to<const std::string*>(), NULL));
	}

	bool Insert(const std::string &xml_string)
	{
		int code = Hash(xml_string);
		const std::string *key = new std::string(xml_string);
		xml_string_seq_->push_back(key);
		xml_string_map_.insert(std::make_pair(code,
			static_cast<int>(xml_string_seq_->size() - 1)));
		return true;
	}

	bool RemoveUnique(const std::string &xml_string)
	{
		int code = Hash(xml_string), index = -1;
		DiffMapType::iterator iter = xml_string_map_.find(code);
		if(iter != xml_string_map_.end())
		{
			for(;iter->first == code; ++iter)
			{
				const std::string *found = xml_string_seq_->at(iter->second);
				if(found && *found == xml_string)
				{
					index = iter->second;
					break;
				}
			}
		}
		if(index < 0)
			return false;
		delete xml_string_seq_->at(index);
		xml_string_seq_->at(index) = NULL;
		xml_string_map_.erase(iter);
		return true;
	}

private:
	// Javascript string hash:
	static int Hash(const std::string &data)
	{
		int l = data.length();
		int h = l; // seed
		int step = (l >> 5) + 1;
		for(int i = l; i >= step; i -= step)
			h = h ^ ((h << 5) + (h >> 2) + data[i - 1]);
		return h;
	}
	
	DiffSequanceType *xml_string_seq_;
	DiffMapType xml_string_map_;

	DISABLE_COPY_AND_ASSIGN(DifferenceArray);
};


class DifferencePrinter
{
public:
	DifferencePrinter(const std::vector<const std::string*> &origin_diffs,
	                  const std::vector<const std::string*> &diffs,
	                  const std::string table_name)
		: origin_diffs_(origin_diffs)
		, diffs_(diffs)
		, table_name_(table_name)
	{}

	bool Init()
	{
		std::string path_prefix_ = ConfPref::Local()->OutputPrefix();
		if(!SafeMakeDirectory(path_prefix_))
			return false;
		path_prefix_ += '/';

		origin_dir_ = path_prefix_ + kOriginDBName;
		if(!SafeMakeDirectory(origin_dir_))
			return false;
		origin_dir_ += '/';

		dir_ = path_prefix_ + ConfPref::Local()->DBName();
		if(!SafeMakeDirectory(dir_))
			return false;
		dir_ += '/';
		return true;
	}

	struct LinePrinter
	{
		LinePrinter(std::FILE *fp) : fp_(fp) {}

		void operator () (const std::string *element) const
		{
			assert(element != NULL);
			//fprintf(fp_, "[%x]:%s", element, element->c_str());
			fprintf(fp_, "%s", element->c_str());
		}

		std::FILE *fp_;
	};

	bool Print()
	{
		std::string file_name = origin_dir_ + table_name_ + "_diff_only.txt";
		std::FILE *file = std::fopen(file_name.c_str(), "w");
		if(!file)
		{
			fprintf(stderr, "Can not open %s, %s\n", file_name.c_str(),
			        strerror(errno));
			return false;
		}
		for_each(origin_diffs_.begin(), origin_diffs_.end(), LinePrinter(file));
		std::fclose(file);

		file_name = dir_ + table_name_ + "_diff_only.txt";
		file = std::fopen(file_name.c_str(), "w");
		if(!file)
		{
			fprintf(stderr, "Can not open %s, %s\n", file_name.c_str(),
			        strerror(errno));
			return false;
		}
		for_each(diffs_.begin(), diffs_.end(), LinePrinter(file));
		std::fclose(file);
		return true;
	}

private:
	bool SafeMakeDirectory(const std::string &path)
	{
		if(mkdir(path.c_str(), 0777) < 0)
		{
			if(errno != EEXIST)
			{
				std::fprintf(stderr, "Can not create directory: %s, %s\n",
				             path.c_str(), strerror(errno));
				return false;
			}
		}
		chmod(path.c_str(), 0775);
		return true;
	}
	
	const std::vector<const std::string*> &origin_diffs_;
	const std::vector<const std::string*> &diffs_;
	const std::string table_name_;
	std::string path_prefix_;
	std::string origin_dir_;
	std::string dir_;

	DISABLE_COPY_AND_ASSIGN(DifferencePrinter);
};

class DifferenceParser
{
public:
	DifferenceParser(DBFetcher *origin_fetcher, DBFetcher *fetcher)
		: origin_fetcher_(origin_fetcher)
		, fetcher_(fetcher)
	{}

	~DifferenceParser() {}

	bool Test(const std::string &table_name) const
	{
		return origin_fetcher_->Test(table_name) &&
		       fetcher_->Test(table_name);
	}

	bool Parse(const std::string &table_name)
	{
		bool ok = false;
		std::string xml_string;
		DifferenceArray diff_array(&origin_diffs_);
		if(!origin_fetcher_->Query(table_name))
			goto final;
		while(origin_fetcher_->Fetch())
		{
			if(origin_fetcher_->Bin2Xml(&xml_string))
				diff_array.Insert(xml_string);
		}

		if(!fetcher_->Query(table_name))
			goto final;
		while(fetcher_->Fetch())
		{
			if(fetcher_->Bin2Xml(&xml_string))
			{
				if(!diff_array.RemoveUnique(xml_string))
					diffs_.push_back(new std::string(xml_string));
			}
		}

		ok = Final(table_name);
	final:
		delete_and_clear_container(&origin_diffs_);
		delete_and_clear_container(&diffs_);
		return ok;
	}

private:
	bool Final(const std::string &table_name)
	{
		static const std::string* const kNullStringPointer = NULL;
		origin_diffs_.erase(remove(origin_diffs_.begin(), origin_diffs_.end(),
		                           kNullStringPointer),
		                    origin_diffs_.end());

		printf("Table: %s ", table_name.c_str());
		printf("Additon: %d ", origin_fetcher_->NumberOfFetched() - 
		       fetcher_->NumberOfFetched());
		int number_of_mod = std::max(origin_diffs_.size(), diffs_.size());
		printf("Modification: %d\n", number_of_mod);

		if(!number_of_mod)
			return true;
		DifferencePrinter printer(origin_diffs_, diffs_, table_name);
		if(!printer.Init())
			return false;
		return printer.Print();
	}

	DBFetcher *origin_fetcher_;
	DBFetcher *fetcher_;
	std::vector<const std::string*> origin_diffs_;
	std::vector<const std::string*> diffs_;

	DISABLE_COPY_AND_ASSIGN(DifferenceParser);
};

} // namespace 

int main(int argc, char *argv[])
{
	ConfPref scope_conf(0);

	if(!scope_conf.Parse(argc, argv))
	{
		Usage();
		return -1;
	}

	DBFetcher origin_fetcher;
	int origin_version = -1;
	if(!origin_fetcher.Init(&origin_version,
	                        scope_conf.OriginMetaFilePath()))
		return -1;

	DBFetcher old_fetcher;
	int old_version = -1;
	if(!old_fetcher.Init(&old_version,
	                     scope_conf.MetaFilePath()))
		return -1;

	if(!origin_fetcher.Connect(kOriginDBName) ||
	   !old_fetcher.Connect(scope_conf.DBName()))
		return -1;

	int version = std::min(origin_version, old_version);
	origin_fetcher.SetVersion(version);
	old_fetcher.SetVersion(version);

	std::printf("Output path: %s\n", scope_conf.OutputPrefix().c_str());
	DifferenceParser parser(&origin_fetcher, &old_fetcher);
	for(int i = 0; i < scope_conf.NumberOfTableNames(); ++i)
	{
		if(!parser.Test(scope_conf.TableNames(i)))
			continue;
		if(!parser.Parse(scope_conf.TableNames(i)))
			return -1;
	}

	return 0;
}

