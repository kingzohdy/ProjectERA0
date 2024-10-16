
#ifndef __Textue_H__
#define __Textue_H__

#include "OgreResource.h"
#include "OgrePixelFormat.h"
#include "OgreRenderTypes.h"
#include "OgreLocker.h"

namespace Ogre
{
	enum TextureType
	{
		TEXTURE_2D = 0,
		TEXTURE_VOLUME,
		TEXTURE_CUBE
	};

	struct TextureDesc
	{
		TextureType type;
		uint width;
		uint height;
		uint depth;
		uint nmipmap;
		PixelFormat format;
		uint cubefaces;
	};

	class _OgreExport SurfaceData : public BaseObject
	{
		DECLARE_RTTI(SurfaceData)
	public:
		SurfaceData();
		SurfaceData(PixelFormat fmt, uint width, uint height, uint depth);
		bool loadFromDDSStream(DataStream *pstream);
		void *getBits()
		{
			return &m_Pixels[0];
		}

		void *getRowBits(uint y, uint z=0) const;

		void setPixel(const void *pixel_value, uint x, uint y, uint z=0);
		void getPixel(void *pixel_value, uint x, uint y, uint z=0);
		void bitBlt(uint dest_x, uint dest_y, const SurfaceData *psrc, uint src_x, uint src_y, uint src_width, uint src_height);

	public:
		PixelFormat m_Format;
		uint m_Width;
		uint m_Height;
		uint m_Depth;
		uint m_Stride;
		uint m_RowPitch;
		uint m_SlicePitch;
		std::vector<char>m_Pixels;

	private:
		virtual void _serialize(Archive &ar, int version);
	};

	class _OgreExport DummyTexture : public Texture
	{
		DECLARE_RTTI(DummyTexture)
	public:
		virtual void getDesc(TextureDesc &desc)
		{
			assert(0);
		}

		virtual HardwareTexture *getHardwareTexture()
		{
			return NULL;
		}

		virtual void *lock(size_t face, size_t level, bool readonly, LockResult &lockresult)
		{
			return NULL;
		}

		virtual void unlock(size_t face, size_t level)
		{
		}

	private:
		~DummyTexture()
		{
		}
	};

	class _OgreExport TextureData : public Texture
	{
		DECLARE_RTTI(TextureData)
	public:
		TextureData();
		TextureData(const TextureDesc &desc, bool is_static=true);

		bool loadFromImageFile(const String &path);
		bool loadFromImageFile(const String &path, DataStream *pstream);
		void saveToDDS(const char *path);

		virtual HardwareTexture *getHardwareTexture();
		virtual void *lock(size_t face, size_t level, bool readonly, LockResult &lockresult);
		virtual void unlock(size_t face, size_t level);
		
		void getDesc(TextureDesc &desc)
		{
			desc = m_Desc;
		}

		uint getWidth()
		{
			return m_Desc.width;
		}

		uint getHeight()
		{
			return m_Desc.height;
		}

		PixelFormat getFormat()
		{
			return m_Desc.format;
		}

		// 将图片转换到buffer，用户负责删除内部分配的内侧
		// 只获得第1级别的数据
		// 工具做场景烘焙，解析dds使用
		// format == 0    IL_BGR
		// format == 1    IL_BGRA
		// format == 3    PF_A8R8G8B8
		// format == 4    PF_B8G8R8A8
		// format == 5    else
		void *convertToARGBBuffer (int &format);

		void *convertDDSFile (std::string name, int &format);

		SurfaceData *lockSurface(size_t face, size_t level, bool readonly);
		void unlockSurface(size_t face, size_t level);
		
		void setStatic( bool is_static )
		{
			m_isStatic = is_static;
		}
		void setHardwareTextureinvild();
	private:
		virtual void _serialize(Archive &ar, int version);
		bool loadFromDDS(const String &path, DataStream *pstream);
		bool loadFromOtherImage(const String &path, DataStream *pstream);
		SurfaceData *newSurface(int level);

	protected:
		virtual ~TextureData();
		void createSurfaceByDesc();

		TextureDesc m_Desc;
		std::vector<SurfaceData *>m_Surfaces; //[nface][minlevel]
		HardwareTexture *m_pTexture;

		enum
		{
			LOCK_NULL,
			LOCK_READONLY,
			LOCK_WRITE
		};
		int m_LockFlag;
		bool m_isStatic;

		std::string m_strFileName;

	public:
		// 在编辑器处理情况下不要删除static内存数据
		static bool msNowIsForEditor;
		static LockSection m_ilLoadLMutex;
	};

	class _OgreExport RT_TEXTURE : public Texture
	{
		DECLARE_RTTI(RT_TEXTURE)
	public:
		RT_TEXTURE() : m_pTexture(NULL){}
		RT_TEXTURE(const TextureDesc &desc,const HardwareBufferUsage& usage);

		virtual HardwareTexture *getHardwareTexture(){return m_pTexture;}
		virtual void *lock(size_t face, size_t level, bool readonly, LockResult &lockresult);
		virtual void unlock(size_t face, size_t level);

		void getDesc(TextureDesc &desc)
		{
			desc = m_Desc;
		}

	protected:
		virtual ~RT_TEXTURE();

		TextureDesc m_Desc;
		HardwareTexture *m_pTexture;
	};

	_OgreExport void SerializeExternalTexture(Archive &ar, TextureData *&ptexdata);
	_OgreExport TextureData *LoadTextureFromFile(const char *filename);
}

#endif
