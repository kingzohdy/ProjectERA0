#ifndef XML_SPLITTER_H
#define XML_SPLITTER_H

#include "basictype.h"
#include <iostream>

namespace grav {

class XMLSplitter
{
public:
	XMLSplitter(std::istream *stream);

	~XMLSplitter();

	bool Next();

	const std::string& CurrentSegment() const { return segment_; }

private:
	std::istream *stream_;
	std::string segment_;

	DISABLE_COPY_AND_ASSIGN(XMLSplitter);
};

} // namespace grav

#endif // XML_SPLITTER_H
