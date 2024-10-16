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
 * Target entities, the relationship between an
 * instance and a bake target.
 * @todo Per job XML
 * @todo Introduce TargetInstance or similar.
 * Will allow us to set per instance bake uv set,
 * uv offset etc.
 * @todo More than one parent LOD to allow multiple high resolution
 * shapes like in turtle?
 */ 

#ifndef BEASTTARGETENTITY_H
#define BEASTTARGETENTITY_H

#include "beastapitypes.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/** 
	 * Sets the uv set to use when baking the target entity.
	 * Will use default if the uv set is not present in the instance.
	 * Only valid for texture and atlased target entities
	 * @param target the target entity to set the uv set for
	 * @param uvName the name of the uv set to use
	 * @return The result of the operation
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetBakeUVSet(ILBTargetEntityHandle target,
		ILBConstString uvName);


	/** 
	 * Sets the a uv transform for a bake shape. It will place
	 * the shape in a specific location of the texture.
	 * Only valid for texture target entities
	 * @param target the target entity to set the uv transform on
	 * @param offset the offset in uv space for the object
	 * @param scale the scale in uv space for the object
	 * @return The result of the operation
	 * @todo Should this be a matrix?
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetUVTransform(ILBTargetEntityHandle target,
		const ILBVec2* offset,
		const ILBVec2* scale);

	/** 
	 * Sets the requested resolution for the target entity.\n
	 * Only valid for atlased targets.
	 * Will fail if the resolution is higher than the maximum resolution
	 * of the atlased target.
	 * @param target the target entity to set the resolution on
	 * @param width the requested width
	 * @param height the requested height
	 * @return The result of the operation
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetBakeResolution(ILBTargetEntityHandle target, 
		int32 width, 
		int32 height);

	/** 
	 * Sets the relationship between world space coordinates and
	 * texels for an atlased target instance.
	 * Only valid for atlased target instance.
	 * Will fail if the resolution is higher than the maximum resolution
	 * of the atlased target.
	 * @param target the target entity to set the resolution on
	 * @param scale the number of texels each world space unit should cover
	 * @return The result of the operation
	 * @todo What happens if this is larger than the max resolution?
	 * @todo Should there be a default setting for this?
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetTexelScale(ILBTargetEntityHandle target, 
		float scale);


	/** 
	 * Gets the atlas information for a target entity
	 * Only valid on Atlas texture target entities and texture
	 * target entities
	 * @param te the target entity to get atlas information for
	 * @param framebufferIndex a pointer to write where the index of 
	 * the framebuffer 
	 * @param offset a pointer to write the offset in uv space of 
	 * the atlased object.
	 * @param scale a pointer to write the scale in uv space of 
	 * the atlased object.
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBGetAtlasInformation(ILBTargetEntityHandle te,
		int32* framebufferIndex,
		ILBVec2* offset,
		ILBVec2* scale);

	/**
	* Returns the minimum/maximum value in the selected entity. Used to
	* scale the LDR values read from the frame buffer to preserve the dynamic 
	* range.\n
	* The function will fail if the pass has not enabled normalization.\n
	* @param entity the entity for which scale factors should be returned.
	* @param pass the render pass to get normalization values from
	* @param channel the framebuffer channel to use (ignored for global normalization)
	* @param minValue the minimum value for the framebuffer/entity.
	* @param maxValue the maximum value for the framebuffer/entity.
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBGetNormalizationData(ILBTargetEntityHandle entity,
		ILBRenderPassHandle pass,
		int channel,
		float* minValue,
		float* maxValue);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus


#endif // BEASTTARGETENTITY_H
