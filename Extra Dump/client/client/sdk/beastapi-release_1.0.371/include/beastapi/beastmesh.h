/*
* Copyright (c) 2008-2009 Illuminate Labs AB.
* 
* You may use this code for any project covered by a separate written
* license agreement with Illuminate Labs AB. You may also use this code as
* part of an evaluation of Illuminate Labs AB's software.
*
* You bear the entire risk of using this code. The code is provided as-is
* and Illuminate Labs AB gives no guarantees or warranties whatsoever.
* Illuminate Labs AB excludes the implied warranties of merchantability,
* fitness for a particular purpose and non-infringement.
*
*/

/** \file 
 * The api for specifying meshes in beast
 */ 
#ifndef BEASTMESH_H
#define BEASTMESH_H

#include "beastapitypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** 
 * Begins creation of a mesh
 * @param beastManager the beast manager this mesh will be associated with
 * @param uniqueName a name that must be unique withing the scene. Used to
 * look it up in the cache.
 * @param targetMesh a pointer to a Beast mesh object that will receive the created object
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBBeginMesh(ILBManagerHandle beastManager, 
	ILBConstString uniqueName, 
	ILBMeshHandle* targetMesh);

/** 
 * Finalizes a mesh.\n
 * After this call, it's possible to create instances from the mesh.\n
 * Will fail if any material group, uvLayer or colorLayer is unfinished
 * @param mesh the mesh to finalize
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBEndMesh(ILBMeshHandle mesh);

/** 
 * Finds a cached mesh.
 * @param beastManager the beast manager to check whether the mesh is available 
 * in
 * @param uniqueName the unique name for mesh.
 * @param target the mesh handle to store the mesh in
 * @return The result of the operation.\n
 * ILB_ST_SUCCESS if the mesh is available
 * ILB_ST_UNKNOWN_OBJECT if the mesh is not in the cache
 */
ILB_DLL_FUNCTION ILBStatus ILBFindMesh(ILBManagerHandle beastManager,
									 ILBConstString uniqueName,
									 ILBMeshHandle* target);

/** 
 * Erases a mesh from the cache.\n Will fail if there are any resources
 * in the beast manager referring to it (typically scenes).
 * If this function is successful handles to the erased mesh will
 * be invalidated and cause undefined behavior if used!
 * @param beastManager the beast manager to erase the mesh from.
 * @param uniqueName the name of the mesh to remove
 * @return The result of the operation. 
 * @bug The dependency check is done by checking if there are
 * any present scenes at all, could be more fine grained (i.e
 * are there any scenes refering to this mesh?)
 */
ILB_DLL_FUNCTION ILBStatus ILBEraseCachedMesh(ILBManagerHandle beastManager,
									  ILBConstString uniqueName);

/** 
 * Adds a chunk of vertex data to a mesh. This can be called multiple times 
 * to keep temporary buffer bounded.
 * @param mesh the mesh to add vertices on.
 * @param vertexData a pointer to an array of vertex positions
 * @param normalData a pointer to an array of vertex normals
 * @param vertexCount the number of vertices and normals specified with this
 * call.
 * Behavior is undefined if vertexData or normalsData contains less than
 * vertexCount vertices/normals
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBAddVertexData(ILBMeshHandle mesh, 
								  const ILBVec3* vertexData, 
								  const ILBVec3* normalData, 
								  int32 vertexCount);

/** 
 * Begins a material group
 * @param mesh the mesh to add a material group to
 * @param materialName name of the default material on this group
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBBeginMaterialGroup(ILBMeshHandle mesh, 
									   ILBConstString materialName);

/** 
* End a material group
* @param mesh the mesh to end the material group on
* @return The result of the operation.
*/
ILB_DLL_FUNCTION ILBStatus ILBEndMaterialGroup(ILBMeshHandle mesh);

/** 
 * Add triangles to a material group.
 * The indices refers to the vertices added with AddVertexData.
 * The triangles should be defined so the objects outside sees it
 * as counter clock wise to make sure the outside is visible if
 * rendering them single sided.
 * @param mesh the mesh on which to add the triangles to
 * @param indexData the indices of the triangles to add
 * @param indexCount the total index count. Must be a multiply of 3 
 * (i.e each batch must end in a complete triangle)
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBAddTriangleData(ILBMeshHandle mesh,
									const int32* indexData, 
									int32 indexCount);

/** 
 * Creates a new UV layer.\n
 * Use AddUVData to add UV coordinates.
 * @param mesh the mesh to add the UV layer to. Must not be
 * finalized yet.
 * @param layerName the UV layer name. Must be unique within the mesh
 */
ILB_DLL_FUNCTION ILBStatus ILBBeginUVLayer(ILBMeshHandle mesh,
								   ILBConstString layerName);

/** 
 * Ends the UV layer currently being created.\n Will fail if not
 * the current number of UV's is the same as the number of vertices
 * in the mesh.
 * @param mesh the mesh to finalize the UV layer on.
 */
ILB_DLL_FUNCTION ILBStatus ILBEndUVLayer(ILBMeshHandle mesh);


/** 
 * Adds a batch of UV coordinates to a mesh. It may be called
 * multiple times, but the total number of add UV's may never
 * be more than there are vertices in the mesh.
 * @param mesh the mesh to add UV data on.
 * @param uvData an array of UV coordinates to add to the UV layer
 * @param count the number of UV coordinates in the uvData array.
 */
ILB_DLL_FUNCTION ILBStatus ILBAddUVData(ILBMeshHandle mesh, 
							  const ILBVec2* uvData, 
							  int32 count);

/** 
 * Creates a new color layer.\n
 * @param mesh the mesh to add the color layer to
 * @param layerName the name of the layer, must be unique.
 * @return The result of the operation.
 * @bug Currently only one Color layer is supported
 */
ILB_DLL_FUNCTION ILBStatus ILBBeginColorLayer(ILBMeshHandle mesh,
								    ILBConstString layerName);

/** 
 * Finalizes a color layer. The total number of added colors must be
 * the same as the vertex count in the mesh.
 * @param mesh the mesh to finalize the color layer on.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBEndColorLayer(ILBMeshHandle mesh);


/** 
 * Add Color data to the active color set.
 * @param mesh the mesh to add color data to.
 * @param colorData a pointer to an array of color data.
 * @param count the number of colors in the array
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBAddColorData(ILBMeshHandle mesh, 
								 const ILBLinearRGBA* colorData, 
								 int32 count);

/** 
 * Begins adding tangents and bitangents to the mesh.\n
 * Use ILBAddTangentData to add tangent and bitanget data.
 * @param mesh the mesh to add the tangent layer to. Must not be
 * finalized yet.
 */
ILB_DLL_FUNCTION ILBStatus ILBBeginTangents(ILBMeshHandle mesh);

/** 
* Ends the tangent layer.\n
* Will fail if not the current number of tangents is the same 
* as the number of vertices in the mesh.
* @param mesh the mesh to finalize the tangent layer on.
*/
ILB_DLL_FUNCTION ILBStatus ILBEndTangents(ILBMeshHandle mesh);

/** 
* Adds a batch of tangents and bitangents (binormals) to a mesh. 
* It may be called multiple times, but the total number of 
* added tangents/bitangents may never be more than there are 
* vertices in the mesh.
* @param mesh the mesh to add tangent data on.
* @param tangentData an array of tangents to add 
* @param bitangentData an array of bitangents to add
* @param count the number of tangents in the tangentData and
* bitangentData arrays.
*/
ILB_DLL_FUNCTION ILBStatus ILBAddTangentData(ILBMeshHandle mesh, 
											 const ILBVec3* tangentData, 
											 const ILBVec3* bitangentData, 
											 int32 count);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTMESH_H
