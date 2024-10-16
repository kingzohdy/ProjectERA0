
#ifndef __BaseObject_H__
#define __BaseObject_H__

#include "OgrePrerequisites.h"
#include "OgreMemoryDelegation.h"

namespace Ogre
{
	class BaseObject;

	class _OgreExport RuntimeClass
	{
		typedef BaseObject *(*ObjectCreateFunction)();

	public:
		RuntimeClass(const char *pname, const RuntimeClass *pbase, int version, ObjectCreateFunction newfunc);

		const RuntimeClass *getBaseClass() const
		{
			return m_pBaseClass;
		}

		const char *getClassName() const
		{
			return m_pClassName; 
		}

		int getVersion() const
		{
			return m_Version;
		}

		BaseObject *newObject()
		{
			return (*m_pNewFunc)();
		}

		static RuntimeClass *fromName(const char *pname);

	private:
		const char *m_pClassName;
		const RuntimeClass *m_pBaseClass;
		int m_Version;
		ObjectCreateFunction m_pNewFunc;

		RuntimeClass *m_pNextClass;
	};

	class _OgreExport BaseObject : public MemoryDelegation
	{
	public:
		static const RuntimeClass m_RTTI;

		virtual const RuntimeClass *getRTTI() const
		{
			return &m_RTTI;
		}

		bool isExactKindOf( const RuntimeClass *prtti ) const
		{
			return (getRTTI() == prtti);
		}
		
		bool isKindOf( const RuntimeClass *prtti ) const;

		virtual void release()
		{
			m_RefCount--;
			if(m_RefCount <= 0 )
			{
				deleteThis();
			}
		}

		virtual void addRef()
		{
			m_RefCount++;
		}

		virtual int getRefCount()
		{
			return m_RefCount;
		}

		virtual void _serialize(Archive &ar, int version)
		{
		}

	protected:
		BaseObject() : m_RefCount(1)
		{
		}

		virtual ~BaseObject()
		{
		}

		virtual void deleteThis()
		{
			delete this;
		}

		int m_RefCount;
	};

#define DECLARE_RTTI(classname) \
public: \
	static const RuntimeClass m_RTTI; \
	virtual const RuntimeClass *getRTTI() const{ return &m_RTTI; } \
	static BaseObject *newObject(){ return new classname; }

#define IMPLEMENT_RTTI(classname, baseclassname, version) \
	const RuntimeClass classname::m_RTTI(#classname, &baseclassname::m_RTTI, version, classname::newObject);

#define DECLARE_RTTI_VIRTUAL(classname) \
public: \
	static const RuntimeClass m_RTTI; \
	virtual const RuntimeClass *getRTTI() const{ return &m_RTTI; } \

#define IMPLEMENT_RTTI_VIRTUAL(classname, baseclassname, version) \
	const RuntimeClass classname::m_RTTI(#classname, &baseclassname::m_RTTI, version, NULL);

#define IS_EXACT_KIND_OF(classname, pobj) \
	pobj->isExactKindOf( &classname::m_RTTI )

#define IS_KIND_OF(classname, pobj) \
	pobj->isKindOf( &classname::m_RTTI )

#define STATIC_CAST(classname, pobj) \
	((classname *)pobj)

#define DYNAMIC_CAST(classname, pobj) \
	(pobj->isKindOf(&classname::m_RTTI)? (classname *)this : NULL)

#define ASSIGN_OBJECT_PTR(p1, p2) if(p2) p2->addRef(); p1 = p2;
#define OGRE_ADDREF(p) p->addRef();
}

#endif