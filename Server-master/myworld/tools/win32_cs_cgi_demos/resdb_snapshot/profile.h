#ifndef PROFILE_H
#define PROFILE_H

#include "basictype.h"
#include <map>

namespace grav {

class Profile
{
public:
	Profile(Linker::Value);

	~Profile();

	bool Load(const std::string &profile_name);

	bool Store(const std::string &profile_name) const;

	const std::string& Value(const std::string& key)
	{ return context_[key]; }

	std::string* MutableValue(const std::string& key)
	{ return &context_[key]; }

	static Profile* Local();

private:
	std::map<std::string, std::string> context_;

	DISABLE_COPY_AND_ASSIGN(Profile);
};



#define CONF_EXPORT(key)   (::grav::Profile::Local()->Value(#key))
#define CONF_EXPORT_Z(key) (::grav::Profile::Local()->Value(#key).c_str())

#define CONF_IMPORT(key)   (::grav::Profile::Local()->MutableValue(#key))

} // namespace grav

#endif // PROFILE_H
