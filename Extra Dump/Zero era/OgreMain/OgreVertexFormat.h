
#ifndef __VertexFormat_H__
#define __VertexFormat_H__

#include "OgreBaseObject.h"

namespace Ogre
{
	/// Vertex element semantics, used to identify the meaning of vertex buffer contents
	enum VertexElementSemantic {
		/// Position, 3 reals per vertex
		VES_POSITION = 1,
		/// Blending weights
		VES_BLEND_WEIGHTS = 2,
		/// Blending indices
		VES_BLEND_INDICES = 3,
		/// Normal, 3 reals per vertex
		VES_NORMAL = 4,
		/// Diffuse colours
		VES_COLOR = 5,
		/// Texture coordinates
		VES_TEXCOORD = 7,
		/// Binormal (Y axis if normal is Z)
		VES_BINORMAL = 8,
		/// Tangent (X axis if normal is Z)
		VES_TANGENT = 9,

		//transformed position
		VES_POSITIONT = 10,
	};

	/// Vertex element type, used to identify the base types of the vertex contents
	enum VertexElementType
	{
		VET_FLOAT1 = 0,
		VET_FLOAT2 = 1,
		VET_FLOAT3 = 2,
		VET_FLOAT4 = 3,
		/// alias to more specific colour type - use the current rendersystem's colour packing
		VET_COLOUR = 4,
		VET_SHORT1 = 5,
		VET_SHORT2 = 6,
		VET_SHORT3 = 7,
		VET_SHORT4 = 8,
		VET_UBYTE4 = 9,
	};

	class VertexElement
	{
	public:
		uint getSize() const
		{
			return getTypeSize(m_Type);
		}

		bool operator==(const VertexElement &rhs) const
		{
			assert(sizeof(VertexElement) == 4);
			return *(const uint *)this == *(const uint *)&rhs;
		}

		static uint getTypeSize(VertexElementType etype);
		static uint getTypeCount(VertexElementType etype);

	public:
		uint m_StreamSource : 4;
		uint m_Offset : 8;
		VertexElementType m_Type : 8;
		VertexElementSemantic m_Semantic : 8;
		uint m_Index : 4;
	};

	class _OgreExport VertexFormat
	{
	public:
		VertexFormat();
		VertexFormat(const VertexFormat &rhs);
		~VertexFormat();

		void reserveElements(size_t nelement);
		void addElement(VertexElementType type, VertexElementSemantic semantic, uint index=0, uint stream=0, uint offset=-1);
		uint getNumElements() const{ return (uint)m_Elements.size(); }
		const VertexElement &getElement(uint i) const{ return m_Elements[i]; }
		const VertexElement *getElementBySemantic(VertexElementSemantic semantic, int index=-1) const;

		uint getStride() const;
		bool hasElement(VertexElementSemantic semantic);

		bool operator==(const VertexFormat &vertfmt) const;
		VertexFormat &operator=(const VertexFormat &vertfmt);

		friend Archive &operator<<(Archive &ar, VertexFormat &vertfmt);

	private:
		std::vector<VertexElement>m_Elements;
	};
}

#endif