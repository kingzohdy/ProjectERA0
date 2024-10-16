/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#ifndef _Resource_H__
#define _Resource_H__

#include "OgreBaseObject.h"
#include "OgreFixedString.h"

namespace Ogre
{
	class _OgreExport Resource : public BaseObject
	{
		DECLARE_RTTI_VIRTUAL(Resource)
		friend class ResourceManager;

	public:
		const FixedString &getResPath()
		{
			return m_ResPath;
		}
		void setResPath(const FixedString &path)
		{
			m_ResPath = path;
		}

		virtual void release();

		virtual void addRef();

		virtual int getRefCount();

		/*
		virtual bool load(const FixedString &path);
		virtual bool save(const FixedString &path);
		*/

		void save(const FixedString &path);

	protected:
		FixedString m_ResPath;
		volatile long m_RefNum;

		/** Protected unnamed constructor to prevent default construction. 
		*/
		Resource() : m_RefNum(1)
		{
		}

		virtual ~Resource()
		{
		}
	};

	_OgreExport MovableObject *createObjectFromResource(Resource *presdata);
	_OgreExport MovableObject *createObjectFromResource(const char *path);
}

#endif
