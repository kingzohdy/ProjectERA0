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
 * The target definitions
 * @todo Per job XML
 * @todo A way of getting the atlas data from an atlased
 * job
 * @todo A way of getting the vertex buffer for a job.
 */ 
#ifndef BEASTTARGET_H
#define BEASTTARGET_H

#include "beastapitypes.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/** 
	 * Adds a texture baking target to a job
	 * @param job the job to add the target to
	 * @param name the name of the target
	 * @param width the width in pixels of the texture target
	 * @param height the height in pixels of the texture target
	 * @param target the handle to store the generated target in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreateTextureTarget(ILBJobHandle job, 
		ILBConstString name,
		int32 width,
		int32 height,
		ILBTargetHandle* target);

	/** 
	 * Adds an atlased texture baking target to a job
	 * @param job the job to add the target to
	 * @param name the name of the target
	 * @param maxWidth the maximum width in pixels of each generated texture
	 * @param maxHeight the maximum height in pixels of each generated texture
	 * @param maxTextures the maximum number of generated textures. 0 means don't care.
	 * @param target the handle to store the generated target in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreateAtlasedTextureTarget(ILBJobHandle job, 
		ILBConstString name,
		int32 maxWidth,
		int32 maxHeight,
		int32 maxTextures,
		ILBTargetHandle* target);

	/** 
	 * Sets the alignment on an atlased texture target
	 * @param target the target
	 * @param alignment the alignment
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetAtlasAlignment(ILBTargetHandle target,
		int32 alignment);

	/** 
	 * Sets the padding on an atlased texture target
	 * @param target the target
	 * @param padding the padding
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetAtlasPadding(ILBTargetHandle target,
		int32 padding);

	/** 
	 * Enables packing spatially close objects into the same texture
	 * @param target the target
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBEnableAtlasSpatial(ILBTargetHandle target);

	/** 
	 * Adds a vertex baking target
	 * @param job the job to add the target to
	 * @param name the name of the target
	 * @param target the handle to store the generated target in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreateVertexTarget(ILBJobHandle job, 
		ILBConstString name,
		ILBTargetHandle* target);

	/** 
	 * Adds a camera render target to a job
	 * @param job the job to add the target to
	 * @param name the name of the target
	 * @param camera handle to the camera to render from
	 * @param width the width in pixels of the image
	 * @param height the height in pixels of the image
	 * @param target the handle to store the generated target in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreateCameraTarget(ILBJobHandle job, 
		ILBConstString name,
		ILBCameraHandle camera,
		int32 width,
		int32 height,
		ILBTargetHandle* target);


	/** 
	 * Adds a point cloud target to a job
	 * @param job the job to add the target to
	 * @param name the name of the target
	 * @param target the handle to store the generated target in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreatePointCloudTarget(ILBJobHandle job, 
		ILBConstString name,
		ILBTargetHandle* target);


	/** 
	 * Adds an instance to bake to a texture or vertex bake target
	 * @param target the target to add the instance to
	 * @param bakeInstance the instance to bake
	 * @param targetEntity the targetEntity for this instance. Can be 0 if you don't care
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBAddBakeInstance(ILBTargetHandle target,
		ILBInstanceHandle bakeInstance,
		ILBTargetEntityHandle* targetEntity);

	/** 
	 * Adds a point cloud to bake
	 * @param target the target to add the point cloud to (only works for point cloud targets)
	 * @param pointCloud the point cloud to bake
	 * @param targetEntity the targetEntity for this instance. Can be 0 if you don't care
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBAddBakePointCloud(ILBTargetHandle target,
		ILBPointCloudHandle pointCloud,
		ILBTargetEntityHandle* targetEntity);


	/** 
	 * Gets the number of framebuffers associated with this target
	 * Is only valid on targets rendering images and if the
	 * target is done
	 * @param target the target to get the count for
	 * @param count a pointer to the variable to receive the framebuffer
	 * count
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBGetFramebufferCount(ILBTargetHandle target,
		int32* count);

	/** 
	 * Gets a framebuffer from a target
	 * Is only valid on targets rendering images and the
	 * target is done
	 * @param target the target to get framebuffer from
	 * @param pass the pass to get vertex data for
	 * @param index of the framebuffer to get
	 * @param fb pointer to the handle that should receive the framebuffer
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBGetFramebuffer(ILBTargetHandle target,
		ILBRenderPassHandle pass,
		int32 index,
		ILBFramebufferHandle* fb);

	/** 
	 * Gets a framebuffer with vertex data from a target
	 * Is only valid on targets rendering vertex data and the
	 * target is done
	 * @param target the target to get vertex buffer from
	 * @param pass the pass to get vertex data for
	 * @param te the target entity to get vertex data for
	 * @param fb pointer to the handle that should receive the framebuffer
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBGetVertexbuffer(ILBTargetHandle target,
		ILBRenderPassHandle pass,
		ILBTargetEntityHandle te,
		ILBFramebufferHandle* fb);

	/** 
	* Add a Render Pass to a target
	* @param target the target to add the pass to
	* @param pass the pass to add to the target
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBAddPassToTarget(ILBTargetHandle target,
		ILBRenderPassHandle pass);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


#endif // BEASTTARGET_H
