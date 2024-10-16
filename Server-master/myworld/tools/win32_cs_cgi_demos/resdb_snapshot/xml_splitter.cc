#include "xml_splitter.h"

namespace grav {

namespace detail {

class LLexer
{
public:
	enum Token {
		TK_ERROR,
		TK_LINE,
		TK_END_LINE,
		TK_EOF,
	};

	LLexer(std::istream *stream, std::string *segment);

	~LLexer();

	Token Next();

private:
	int Read();

	Token ReadLine();

	std::istream *stream_;
	std::string *segment_;

	DISABLE_COPY_AND_ASSIGN(LLexer);
};

LLexer::LLexer(std::istream *stream, std::string *segment)
	: stream_(stream)
	, segment_(segment)
{
}

LLexer::~LLexer()
{
}

LLexer::Token LLexer::Next()
{
	for(;;)
	{
		int ch = Read();
		if(ch < 0)
			break;
			
		switch(ch)
		{
		case '<':
			return ReadLine();

		default:
			break;
		}
	}
	return TK_EOF;
}

int LLexer::Read()
{
	int ch = stream_->get();
	if(ch < 0)
		return -1;
	segment_->append(1, static_cast<char>(ch));
	return ch;
}

LLexer::Token LLexer::ReadLine()
{
	Token ret = TK_LINE;
	for(;;)
	{
		int ch = Read();
		if(ch < 0)
		{
			ret = TK_ERROR;
			break;
		}
			
		switch(ch)
		{
		case '/':
			ret = TK_END_LINE;
			break;
			
		case '>':
			goto out;
			
		default:
			break;
		}
	}
out:
	return ret;
}

} // namespace detail



XMLSplitter::XMLSplitter(std::istream *stream)
	: stream_(stream)
{
}

XMLSplitter::~XMLSplitter()
{
}

bool XMLSplitter::Next()
{
	int nested = 0;

	segment_.clear();
	detail::LLexer lexer(stream_, &segment_);

reentry:
	int token = lexer.Next();
	switch(token)
	{
	case detail::LLexer::TK_EOF:
		return false;
		
	case detail::LLexer::TK_ERROR:
		segment_ = "<Error/>";
		goto out;

	case detail::LLexer::TK_LINE:
		++nested;
		goto reentry;

	case detail::LLexer::TK_END_LINE:
		if(--nested > 0)
			goto reentry;
		goto out;

	default:
		assert(0); // Noreached!
		break;
	}
	
out:
	return true;
}


} // namespace grav
