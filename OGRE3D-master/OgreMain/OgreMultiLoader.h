#ifndef __MultiLoader_H__
#define __MultiLoader_H__

#include "OgrePrerequisites.h"
#include "OgreFixedString.h"
#include "OgreResource.h"
#include "OgreLoadWrap.h"
#include <map>

namespace Ogre
{

	class _OgreExport MultiLoader : public LoadWrap
	{
	private:
		std::map<ResourceHandle, Resource *>	m_loads;
		int		count;

	public:
		MultiLoader();
		virtual		~MultiLoader();

	private:
		void	testResult();

	protected:
		virtual void	onComplete(size_t, Resource **, ResourceHandle *) = 0;
		ResourceHandle		startLoad(const FixedString &filename, int prop);
		void			stopLoad(ResourceHandle h);

	private:
		void	ResourceLoaded(Resource *rec, ResourceHandle h);
	};

}

#endif
