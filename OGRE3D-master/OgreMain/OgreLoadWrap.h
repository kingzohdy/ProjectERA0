#ifndef __LoadWrap_H__
#define __LoadWrap_H__

#include "OgrePrerequisites.h"
#include "OgreFixedString.h"
#include "OgreResource.h"

namespace Ogre
{
	class _OgreExport LoadWrap
	{
	protected:
		virtual ~LoadWrap();
		ResourceHandle backgroundLoad(const FixedString &filename, int prop);
		void breakLoad(ResourceHandle h);

	private:
		virtual void ResourceLoaded(Resource *pres, ResourceHandle h) = 0;

	public:
		void ResourceLoad(Resource *pres, ResourceHandle h);
	};
}

#endif
