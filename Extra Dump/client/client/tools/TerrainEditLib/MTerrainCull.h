
#pragma once

#include "mterrain.h"
#include "TerrainTile.h"

#include <map>

namespace TE
{
#define CHUNK_SHOW_SIZE_MAX 1024
#define SHOW_MODEL_SIZE_MAX (1024*16)

	class MTerrainCull
	{
	public:
		MTerrainCull(void);
		~MTerrainCull(void);

		void  Cull(MTerrain* pTreeain);

	public:
		MChunk*                                 m_ShowChunk[CHUNK_SHOW_SIZE_MAX];
		unsigned int                            m_ShowChunkNum ;

		TerrModel*                            m_ShowModel[SHOW_MODEL_SIZE_MAX];
		unsigned int                            m_ShowModelNum ;
	};
}