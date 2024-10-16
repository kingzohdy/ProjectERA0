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
 * The api for specifying instances of meshes in Beast
 */ 
#ifndef BEASTINSTANCE_H
#define BEASTINSTANCE_H

#include "beastapitypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	/**
	 * Beast render stats
	 */
	typedef enum {
		/**
		 * Controls whether the object should be visible for primary rays
		 * All other effects such as shadow casting and occlusion casting
		 * is unaffected.\n
		 * Default: Enabled
		 */
		ILB_RS_PRIMARY_VISIBILITY =			0x00000001,

		/**
		 * Controls whether the object should cast shadows.\n
		 * Default: Enabled
		 */
		ILB_RS_CAST_SHADOWS =				0x00000002,

		/**
		 * Controls whether the object should receive shadows.\n
		 * Default: Enabled
		 */
		ILB_RS_RECEIVE_SHADOWS =			0x00000004,

		/// @cond IGNORE_IN_DOCUMENTATION
		ILB_RS_RESERVED_1 =					0x00000008,
		ILB_RS_RESERVED_2 =					0x00000010,
		/// @endcond
		
		/**
		 * Controls whether the object should be visible for reflection rays\n
		 * Default: Enabled
		 */
		ILB_RS_VISIBLE_IN_REFLECTIONS =		0x00000020,

		/**
		 * Controls whether the object should be visible for refraction rays\n
		 * Default: Enabled
		 */
		ILB_RS_VISIBLE_IN_REFRACTIONS =		0x00000040,

		/**
		 * Controls whether the object should be visible for final gather rays\n
		 * Default: Enabled
		 */
		ILB_RS_VISIBLE_IN_FINAL_GATHER =	0x00000080,

		/**
		 * Controls whether the object should be single sided or double sided.
		 * If single sided, only polygons that are defined CCW from the ray 
		 * are visible.\n
		 * Default: Enabled
		 */
		ILB_RS_DOUBLE_SIDED =				0x00000100,

		/**
		 * Only applies when single sided. It flips the test for rejecting 
		 * single sided polygons.\n
		 * Default: Disabled
		 */
		ILB_RS_OPPOSITE =					0x00000200,

		/**
		 * If this is set to false, the object is black for GI purposes.
		 * It's still in the scene and casts occlusion, but no light bounces off
		 * it and it casts no color bleeding on neighbor objects. \n
		 * Default: Enabled
		 */
		ILB_RS_CAST_GI =					0x00000400,

		/**
		* Controls whether the object is receiving any GI. If disabled it only
		* gets direct light.\n
		* Default: Enabled
		*/
		ILB_RS_RECEIVE_GI =					0x00000800,

		/// @cond IGNORE_IN_DOCUMENTATION
		ILB_RS_RESERVED_3 =					0x00001000,
		ILB_RS_RESERVED_4 =					0x00002000,
		/// @endcond

		/**
		 * Controls whether the object is an occluder when rendering ambient
		 * occlusion.
		 * Default: Enabled
		 */
		ILB_RS_CAST_OCCLUSION =				0x00004000,

		/**
		 * Controls whether the object is receiving any occlusion when rendering
		 * ambient occlusion.\n
		 * Default: Enabled
		 */
		ILB_RS_RECEIVE_OCCLUSION =			0x00008000,
		/// @cond IGNORE_IN_DOCUMENTATION
		ILB_RS_RESERVED_5 =					0x00010000,
		ILB_RS_RESERVED_6 =					0x00020000,
		/// @endcond

		/**
		 * Controls whether the object cast shadows on itself.\n
		 * Default: Enabled
		 */
		ILB_RS_SELF_OCCLUSION =				0x00040000,

		/**
		 * If enabled shadow rays will be biased in order to get smooth shadows on 
		 * smoothed meshes.\n
		 * Default: Disabled
		 */
		 ILB_RS_SHADOW_BIAS =				0x00080000,
	} ILBRenderStats;
	
	/**
	 * Selects if the render stats should be enabled or disabled
	 */
	typedef enum {
		/**
		 * Sets the render stats supplied to true
		 */
		ILB_RSOP_ENABLE,
		/**
		 * Sets the render stats supplied to false
		 */
		ILB_RSOP_DISABLE,
	} ILBRenderStatOperation;

	/**
	 * Type representing multiple renderstats.
	 * Combine renderstats using the or operator (|). \n Example:\n
	 * ILBRenderStatsMask rsMask = ILB_RS_SELF_OCCLUSION | ILB_RS_PRIMARY_VISIBILITY;
	 */
	typedef uint32 ILBRenderStatsMask;

	/** 
	 * Add an instance to the scene.
	 * @param scene the scene the instance should be a part of
	 * @param mesh the mesh the instance should reference
	 * @param name the name of the instance, must be unique within the scene.
	 * @param transform the object space to world space transform for this mesh
	 * @param instance the handle to store the generated instance in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreateInstance(ILBSceneHandle scene, 
		ILBMeshHandle mesh,
		ILBConstString name, 
		const ILBMatrix4x4* transform,
		ILBInstanceHandle* instance);

	/**
	 * Sets render stats on an instance.
	 * @param instance the instance to set the render stats on
	 * @param stats the stats to modify. Can be multiple render stats or:ed
	 * together
	 * @param operation selects whether to enable or disable the
	 * selected render stats.
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetRenderStats(ILBInstanceHandle instance, 
									   ILBRenderStatsMask stats,
									   ILBRenderStatOperation operation);



	/** 
	 * Adds an object centric light link list
	 * @param instance the instance to add light links to
	 * @param mode sets whether the light links are inclusive or exclusive
	 * @param lightSources an array of light sources that should be linked
	 * @param count the number of light sources present in the lightSources
	 * array
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBAddInstanceLightLinks(ILBInstanceHandle instance, 
												ILBLightLinkMode mode,
												const ILBLightHandle* lightSources, 
												int32 count);
	
	/** 
	 * Overrides materials for this instance.
	 * @param instance the instance to override materials for
	 * @param materials an array of materials to override the one specified 
	 * in the mesh with. It should be in the same order as the material 
	 * groups in the mesh. A 0 pointer will keep the default material.
	 * If the material lists size doesn't match, unassigned materials
	 * will stay default and overflowing materials will be ignored.
	 * @param materialCount the number of override materials available in the materials
	 * array.
	 * @return The result of the operation.
	 * @bug We currently only support one material override for the entire
	 * shape.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetMaterialOverrides(ILBInstanceHandle instance, 
											   ILBMaterialHandle* materials,
											   int32 materialCount);
	/** 
	 * Connects two meshes so one acts as a high resolution instances and one
	 * as a lod. This is used for normal map generation etc.
	 * When connected, the low resolution mesh will automatically be set as
	 * invisible using renderstats. 
	 * There can be many high resolution instances for every low
	 * resolution instance. An instance can act as high resolution mesh for
	 * many low resolution meshes
	 * @param lowRes the instance to act as low resolution mesh in this
	 * lod relationship. Will be hidden by this call.
	 * @param highRes the high resolution mesh in this lod relationship
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBAddLODInstance(ILBInstanceHandle lowRes, 
		ILBInstanceHandle highRes);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif / / BEASTINSTANCE_H 
