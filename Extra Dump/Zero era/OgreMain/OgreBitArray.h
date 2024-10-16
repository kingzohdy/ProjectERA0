
#ifndef __OgreBitArray_H__
#define __OgreBitArray_H__

#include "OgrePrerequisites.h"

namespace Ogre
{

	class _OgreExport BitArray1D
	{
	public:
		BitArray1D() : m_nPixel(0), m_pBuffer(NULL)
		{
		}

		//bpp表示每个象素占用的bits数目，不超过32bit，因为速度原因，限制bpp必须为2^n
		BitArray1D( size_t npixel, size_t bpp );
		BitArray1D( const BitArray1D &rhs );
		~BitArray1D();

		void init(size_t npixel, size_t bpp);

		BitArray1D &operator=( const BitArray1D &rhs );

		void clear( uint data );

		uint *getBuffer() const
		{ 
			return m_pBuffer; 
		}

		size_t getBufLen() const
		{
			return ( (m_nPixel*m_Bpp + 31)/32 ) * sizeof(uint);
		}

		size_t getNumPixel() const
		{
			return m_nPixel;
		}

		size_t getBpp() const
		{
			return m_Bpp;
		}

		void setPixel( size_t x, uint data )
		{
			assert( x < m_nPixel );

			size_t bits = x*m_Bpp;
			size_t i = bits>>5;
			size_t off = bits&31;
			size_t mask = m_Mask << off;
			m_pBuffer[i] = (m_pBuffer[i]&~mask) | ((data<<off)&mask);
		}

		uint getPixel( size_t x ) const
		{
			assert( x < m_nPixel );

			size_t bits = x*m_Bpp;
			size_t i = bits>>5;
			size_t off = bits&31;
			return (m_pBuffer[i]>>off) & m_Mask;
		}

		void reduceBppTo( size_t bpp );

		friend _OgreExport Archive &operator<<(Archive &ar, BitArray1D &array);

	private:
		uint *m_pBuffer;
		size_t m_nPixel;
		size_t m_Bpp;
		size_t m_Mask;
	};

	class _OgreExport BitArray2D : private BitArray1D
	{
	public:
		BitArray2D() : m_Width(0), m_Height(0)
		{
		}

		BitArray2D( size_t width, size_t height, size_t bpp );
		BitArray2D( const BitArray2D &rhs ) : BitArray1D(rhs), m_Width(rhs.getWidth()), m_Height(rhs.getHeight()){}
		~BitArray2D();

		void init(size_t width, size_t height, size_t bpp);

		BitArray2D &operator=( const BitArray2D &rhs );

		void clear( uint data )
		{
			BitArray1D::clear(data);
		}

		uint *getBuffer() const
		{ 
			return BitArray1D::getBuffer(); 
		}

		size_t getBufLen() const
		{
			return BitArray1D::getBufLen();
		}

		size_t getWidth() const
		{
			return m_Width;
		}

		size_t getHeight() const
		{
			return m_Height;
		}

		size_t getBpp() const
		{
			return BitArray1D::getBpp();
		}

		void setPixel( size_t x, size_t y, uint data )
		{
			assert( x<m_Width && y<m_Height );

			BitArray1D::setPixel( y*m_Width+x, data );
		}

		uint getPixel( size_t x, size_t y ) const
		{
			assert( x<m_Width && y<m_Height );

			return BitArray1D::getPixel( y*m_Width+x );
		}

		void reduceBppTo( size_t bpp )
		{
			BitArray1D::reduceBppTo(bpp);
		}

		//dithering the data to resolution of bpp
		void dithering( size_t bpp );

		friend _OgreExport Archive &operator<<(Archive &ar, BitArray2D &array);

	private:
		size_t m_Width, m_Height;
	};


}

#endif