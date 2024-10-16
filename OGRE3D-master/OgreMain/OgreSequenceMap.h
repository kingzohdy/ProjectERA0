
#ifndef __OGRE_SEQUENCEMAP_H__
#define __OGRE_SEQUENCEMAP_H__

#include "OgreSingleton.h"
#include <map>

namespace Ogre
{
	class SequenceMap : public Singleton<SequenceMap>
	{
	public:
		struct SeqDesc
		{
			int seqid;
			int loopmode;
			int priority;
			int subseq[2];
		};

	public:
		SequenceMap(const char *path);
		~SequenceMap();

		SeqDesc *findSequenceDesc(int seq);

	private:
		std::map<int, SeqDesc>m_SeqMap;
	};
}

#endif