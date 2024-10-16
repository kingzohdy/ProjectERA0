#include "db_fetcher.h"
#include "profile.h"
#include "basictype.h"

#include <sys/stat.h>
#include <stdlib.h>

const char kProfileName[] = "resdb_daily.conf";

namespace {

void Usage()
{
	puts("resdb_daily: Convert DB data to XML file(s)");
	puts("Usage:");
	puts("    resdb_daily output table_1 [[[table_2] table_3] ...]");
	exit(1);
}

} // namespace

// resdb_daily output table_1 [[[table_2] table_3] ...]
//
int main(int argc, char *argv[])
{
	if(argc < 3) Usage();

	grav::Profile profile_scope(grav::Linker::INITIALIZER);
	if(!profile_scope.Load(kProfileName))
	{
		std::fprintf(stderr, "Can not open configure file: %s\n",
		             kProfileName);
		return -1;
	}

	std::string output(argv[1]);
	struct stat buf;
	if(stat(output.c_str(), &buf) < 0)
	{
		std::fprintf(stderr, "Output directory: %s is not exist.\n",
		             output.c_str());
		return -1;
	}

	if(output[output.length() - 1] != '/')
		output += '/';

	grav::DBFetcher fetcher;
	fetcher.SetDBConnectionString(CONF_EXPORT(db_connect));
	fetcher.SetDBUser(CONF_EXPORT(db_user));
	fetcher.SetDBPasswd(CONF_EXPORT(db_passwd));

	int version = 0;
	if(!fetcher.Init(&version, CONF_EXPORT(tdr_file)))
	{
		std::fprintf(stderr, "Can not initialize db fetcher.\n");
		return -1;
	}

	fetcher.SetVersion(version);
	std::printf("Set version: %d\n", version);

	if(!fetcher.Connect(CONF_EXPORT(db_name)))
	{
		std::fprintf(stderr, "Can not connect to database.\n");
		return -1;
	}

	for(int i = 2; i < argc; ++i)
	{
		if(!fetcher.Query(argv[i]))
		{
			std::fprintf(stderr, "Query meta: %s fail.\n", argv[i]);
			continue;
		}

		std::string file_name = output + argv[i] + ".txt";
		std::FILE *fp = std::fopen(file_name.c_str(), "w");
		if(!fp)
		{
			std::fprintf(stderr, "Can not open file: %s for writen.",
			             file_name.c_str());
			continue;
		}

		while(fetcher.Fetch())
		{
			std::string buf;
			fetcher.Bin2Xml(&buf);
			std::fprintf(fp, "%s", buf.c_str());
		}
		std::fclose(fp);
	}
	return 0;
}

