#include "profile.h"
#include <fstream>
#include <cctype>

namespace grav {

namespace detail {

class Parser
{
public:
	Parser(std::istream *stream)
		: stream_(stream)
	{
	}

	~Parser() {}

	int Next()
	{
		int rv = NextToken(&first_);
		if(rv <= 0)
			return rv;
		rv = NextToken(&second_);
		return rv;
	}

	const std::string& CurrentKey() const { return first_; }

	const std::string& CurrentValue() const { return second_; }

	int Peek() { return stream_->get(); }

	int Skip(int n) { while(n--) stream_->get(); }

private:
	int NextToken(std::string *str)
	{
		str->clear();
		for(;;)
		{
			int ch = Peek();
			if(ch < 0)
				break;

			switch(ch)
			{
			case ':':
				return 1;
			case '\"':
				return ReadString(str);
			default:
				if(std::isspace(ch))
					continue;
				else if(std::isalpha(ch))
				{	str->append(1, ch); return ReadKey(str); }
				else
					return -1;
				break;
			}
		}
		return 0;
	}

	int ReadKey(std::string *str)
	{
		for(;;)
		{
			int ch =  Peek();
			if(ch < 0)
				break;

			switch(ch)
			{
			case ':':
				return 1;
			case '_': case '-': case '.': case '@':
				str->append(1, ch);
				break;
			default:
				if(isalpha(ch) || isdigit(ch))
					str->append(1, ch);
				else
					return -1;
				break;
			}
		}
		return 0;
	}

	int ReadString(std::string *str)
	{
		for(;;)
		{
			int ch = Peek();
			if(ch < 0)
				break;

			switch(ch)
			{
			case '\"':
				return 1;
			case '\n': case '\r':
				return -1;
			default:
				str->append(1, ch);
				break;
			}
		}
		return 0;
	}

	std::istream *stream_;
	std::string first_;
	std::string second_;

	DISABLE_COPY_AND_ASSIGN(Parser);
};

} // namespace detail

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
	std::ifstream stream(profile_name.c_str());
	if(!stream.is_open())
		return false;

	detail::Parser parser(&stream);
	for(;;)
	{
		int rv = parser.Next();
		switch(rv)
		{
		case 1:
			context_[parser.CurrentKey()] = parser.CurrentValue();
			break;
		case 0:
			goto out;
		default:
			return false;
		}
	}
out:
	return true;
}

bool Profile::Store(const std::string &profile_name) const
{
	std::ofstream stream(profile_name.c_str());
	if(!stream.is_open())
		return false;

	for(std::map<std::string, std::string>::const_iterator iter = context_.begin();
	    iter != context_.end(); ++iter)
		stream << iter->first << ":\t\t\"" << iter->second << "\"\n";
	return true;
}

/*static*/
Profile* Profile::Local()
{
	assert(top_);
	return top_;
}

} // namespace grav
