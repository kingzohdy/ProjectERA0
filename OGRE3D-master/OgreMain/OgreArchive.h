
#ifndef __Archive_H__
#define __Archive_H__

#include "OgrePrerequisites.h"
#include "OgreDataStream.h"
#include "OgreBaseObject.h"

namespace Ogre
{
	class _OgreExport Archive
	{
	public:
		enum
		{
			MODE_STORE = 0,
			MODE_LOAD = 1
		};

	public:
		Archive(DataStream *pstream, int mode) : m_pStream(pstream), m_Mode(mode)
		{
		}

		virtual ~Archive()
		{
		}

		bool isLoading()
		{
			return m_Mode==MODE_LOAD;
		}

		Archive& operator<<(String &str);
		Archive& operator<<(FixedString &str);
		Archive& operator<<(std::vector<String> &strarray);

		template<class T>
		Archive& operator<<(T *&pobj)
		{
			if(isLoading())
			{
				BaseObject *p = readObject();
				if(p == NULL) pobj = NULL;
				else
				{
					assert(IS_KIND_OF(T, p));
					pobj = static_cast<T *>(p);
				}
			}
			else
			{
				writeObject(pobj);
			}

			return *this;
		}

		template<class T>
		Archive& operator<<(std::vector<T *> &array)
		{
			uint num;

			if(isLoading())
			{
				read(&num, sizeof(num));
				array.resize(num);
				for(uint i=0; i<num; i++)
				{
					BaseObject *pobj = readObject();
					if(pobj == NULL) array[i] = NULL;
					else
					{
						assert(IS_KIND_OF(T, pobj));
						array[i] = static_cast<T *>(pobj);
					}
				}
			}
			else
			{
				num = (uint)array.size();
				write(&num, sizeof(num));
				for(uint i=0; i<num; i++)
				{
					writeObject(array[i]);
				}
			}

			return *this;
		}

		BaseObject *readObject();
		void writeObject(BaseObject *pobj);

		template<typename T>
		void serializeRawType(T &value)
		{
			serialize(&value, sizeof(T));
		}

#define SERIALIZE_PRIMITIVE_FUNC(type)   \
		Archive &operator<<(type &value) \
		{ \
			serialize(&value, sizeof(type)); \
			return *this;  \
		}

		SERIALIZE_PRIMITIVE_FUNC(char)
		SERIALIZE_PRIMITIVE_FUNC(short)
		SERIALIZE_PRIMITIVE_FUNC(int)
		SERIALIZE_PRIMITIVE_FUNC(int64)
		SERIALIZE_PRIMITIVE_FUNC(uchar)
		SERIALIZE_PRIMITIVE_FUNC(ushort)
		SERIALIZE_PRIMITIVE_FUNC(uint)
		SERIALIZE_PRIMITIVE_FUNC(uint64)
		SERIALIZE_PRIMITIVE_FUNC(bool)
		SERIALIZE_PRIMITIVE_FUNC(float)

		template<typename T>
		Archive& serializeRawArray(std::vector<T> &array)
		{
			uint num;
			if(isLoading())
			{
				read(&num, sizeof(num));
				array.resize(num);
				if(num > 0) read(&array[0], num*sizeof(T));
			}
			else
			{
				num = (uint)array.size();
				write(&num, sizeof(num));
				if(num > 0) write(&array[0], num*sizeof(T));
			}

			return *this;
		}

		template<class T>
		Archive &serializeArray(std::vector<T>&array)
		{
			uint num = (uint)array.size();
			*this << num;

			if(isLoading()) array.resize(num);
			for(uint i=0; i<num; i++)
			{
				*this << array[i];
			}

			return *this;
		}

		Archive &serialize(void *pbuf, size_t len)
		{
			if(isLoading()) m_pStream->read(pbuf, len);
			else m_pStream->write(pbuf, len);

			return *this;
		}

		void read(void *pbuf, size_t len)
		{
			m_pStream->read(pbuf, len);
		}

		void write(const void *pbuf, size_t len)
		{
			m_pStream->write(pbuf, len);
		}

	private:
		DataStream *m_pStream;
		int m_Mode;
	};
}

#endif