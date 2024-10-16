#include "profile.h"

namespace grav {

Profile *top_ = NULL;

Profile::Profile(Linker::Value)
{
	assert(!top_);
	top_ = this;
}

Profile::~Profile()
{
	assert(top_);
	top_ = NULL;
}

bool Profile::Load(const std::string &profile_name)
{
	std::FILE *fp = std::fopen(profile_name.c_str(), "r");
	if(!fp)
		return false;
	char buf[128];
	while(std::fscanf(fp, "%s", buf) != EOF)
	{
		std::string *value = &context_[buf];
		if(std::fscanf(fp, "%s", buf) == EOF)
		{
			std::fclose(fp);
			return false;
		}
		value->assign(buf);
	}
	std::fclose(fp);
	return true;
}

/*static*/
Profile* Profile::Local()
{
	assert(top_);
	return top_;
}

} // namespace grav
