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
* Render Pass specification
*/ 

#ifndef BEASTRENDERPASS_H
#define BEASTRENDERPASS_H

#include "beastapitypes.h"

/**
* Illumination Modes
*/
typedef enum {
	/**
	* Only direct illumination (no indirect illumination)
	*/
	ILB_IM_DIRECT_ONLY = 0,

	/**
	* Only indirect illumination (no direct illumination)
	*/
	ILB_IM_INDIRECT_ONLY,

	/**
	* Both direct and indirect illumination
	*/
	ILB_IM_FULL,

	/**
	* Stores both direct+indirect and indirect separately
	*/
	ILB_IM_FULL_AND_INDIRECT,

} ILBIlluminationMode;

/**
* RNM Basis
*/
typedef enum {
	/**
	* Half-Life 2 compatible basis
	*/
	ILB_RB_HL2 = 0,

	/**
	* Unreal Engine 3 compatible basis
	*/
	ILB_RB_UE3,

	/**
	* Unreal Engine 3 basis in untouched order
	*/
	ILB_RB_UE3_FLIPPED,

	/**
	* Allows the user to enter the basis vectors manually
	*/
	ILB_RB_CUSTOM,
} ILBRNMBasis;

/**
* Allow Negative
*/
typedef enum {
	/**
	* Allows negative RNM values
	*/
	ILB_AN_ALLOW = 0,

	/**
	* Disallows negative RNM values
	*/
	ILB_AN_DISALLOW,

	/**
	* Disallows negative RNM values and culls
	* lights below the horizon of each triangle
	*/
	ILB_AN_DISALLOW_CULL_HORIZON,
} ILBRNMAllowNegative;

/**
* Self Occlusion Mode
*/
typedef enum {
	/**
	* Self Occluded rays will continue beyond the originating object
	*/
	ILB_SO_DISABLED = 0,

	/**
	* Self Occluded rays will be set to the environment
	*/
	ILB_SO_SET_ENVIRONMENT,

	/**
	* Objects can self occlude
	*/
	ILB_SO_ENABLED,
} ILBAOSelfOcclusion;

/**
* Light Pass Type
*/
typedef enum {
	/**
	* Stores the incoming light in the light map.
    */
	ILB_LP_LIGHTMAP = 0,

	/**
	* Stores the shadow mask. The individual light mask intensity will be 
	* proportional to the light source intensity.
    */
	ILB_LP_SHADOWMAP,

	/**
	* Stores the full shading in the light map.
    */
	ILB_LP_FULLSHADING,
} ILBLightPassType;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	/** 
	* Creates a Full Shading render pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateFullShadingPass(ILBJobHandle job, 
		ILBConstString name,
		ILBRenderPassHandle* pass);

	/** 
	* Creates an RNM render pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param mode Selects Direct Illumination Only, Indirect Illumination 
	* only or both.
	* @param samples Number of samples for non-adaptive RNM. Set to 0 samples to turn 
	* on adaptivity (recommended).
	* @param basis The RNM basis to use
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateRNMPass(ILBJobHandle job, 
		ILBConstString name,
		ILBIlluminationMode mode,
		int32 samples,
		ILBRNMBasis basis,
		ILBRenderPassHandle* pass);

	/** 
	* Creates a Light render pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param type the lighting mode
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateLightPass(ILBJobHandle job, 
		ILBConstString name,
		ILBLightPassType type,
		ILBRenderPassHandle* pass);

	/** 
	* Makes a light pass use signed distance field shadow maps. Each resulting
	* baked pixel will store the distance to the closest shadow transition.
	* @param pass the light pass
	* @param pixelFilterSize the maximum search range (in pixels).
	* Default value is 20.
	* @param maxWorldDistance the maximum world distance to be stored. Default 
	* value is 1.0f.
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBEnableSignedDistanceField(ILBRenderPassHandle pass,
		int32 pixelFilterSize, float maxWorldDistance);

	/** 
	* Creates a Light Pass Entry
	* @param pass the light pass to create the entry on
	* @param entry the created entry
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateLightPassEntry(ILBRenderPassHandle pass,
		ILBLightPassEntryHandle* entry);

	/** 
	* Add a light to a light pass
	* @param entry the light pass entry to add the light to
	* @param light the light to add
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBAddLightToPass(ILBLightPassEntryHandle entry,
		ILBLightHandle light);

	/** 
	* Add an affected target entity to a light pass
	* @param entry the light pass entry to add the target entity to
	* @param target the target to add
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBAddTargetToPass(ILBLightPassEntryHandle entry, 
		ILBTargetEntityHandle target);

	/** 
	* Add a light to be fully baked to a FullAndIndirectIllumination Pass.
	* @param pass the illumination pass to add the light to
	* @param light the light to add
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBAddFullyBakedLight(ILBRenderPassHandle pass, 
		ILBLightHandle light);

	/** 
	* Creates a Normal render pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateNormalPass(ILBJobHandle job, 
		ILBConstString name,
		ILBRenderPassHandle* pass);

	/** 
	* Creates an Ambient Occlusion render pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param maxDistance the maximum distance to check for occlusion. 0 for infinite.
	* @param coneAngle the cone angle. Default is 180
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateAmbientOcclusionPass(ILBJobHandle job, 
		ILBConstString name,
		float maxDistance,
		float coneAngle,
		ILBRenderPassHandle* pass);

	/** 
	* Enables adaptivity on an AO pass
	* @param pass the pass to enable adaptivity on, must be an AO pass
	* @param accuracy adaptive accuracy, default is 1
	* @param smooth smooth value, default is 1
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetAOAdaptive(ILBRenderPassHandle pass,
		float accuracy, 
		float smooth);

	/** 
	* Sets the number of rays to use in an AO pass
	* @param pass the affected pass, must be an AO pass
	* @param minRay the minimum number of rays to sample for each point, default is 64
	* @param maxRay the maximum number of rays to sample for each point, default is 300
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetAONumRays(ILBRenderPassHandle pass,
		int32 minRay, 
		int32 maxRay);

	/** 
	* Sets the contrast and scale of an AO pass
	* @param pass the affected pass, must be an AO pass
	* @param contrast the desired contrast of the AO pass. Default = 1.0f
	* @param scale scale of occlusion values. Default = 1.0f
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetAOContrast(ILBRenderPassHandle pass,
		float contrast, 
		float scale);

	/** 
	* Enables Uniform Sampling on an AO pass. When Uniform Sampling is enabled
	* the sampling is not cos()-weighted.
	* @param pass the affected pass, must be an AO pass
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetAOUniformSampling(ILBRenderPassHandle pass);

	/** 
	* Sets how the AO pass should react to self occlusion.
	* @param pass the affected pass, must be an AO pass
	* @param selfOcclusion the self occlusion mode. Default is ILB_SO_ENABLED.
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetAOSelfOcclusion(ILBRenderPassHandle pass,
		ILBAOSelfOcclusion selfOcclusion);

	/** 
	* Calculates the "bent normal" (most visible direction).
	* If this is used, sampling cannot be adaptive.
	* The put will contain normals in RGB and occlusion in A
	* @param pass the affected pass, must be an AO pass
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBEnableAOBentNormals(ILBRenderPassHandle pass);


	/** 
	* Creates an Illumination render pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param mode Selects Direct Illumination Only, Indirect Illumination 
	* only or both.
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateIlluminationPass(ILBJobHandle job, 
		ILBConstString name,
		ILBIlluminationMode mode,
		ILBRenderPassHandle* pass);

	/** 
	* Creates a LUA pass
	* @param job the job to add the pass to
	* @param name the name of the pass
	* @param scriptFile the file name of the script
	* @param pass the handle to store the generated target in
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBCreateLuaPass(ILBJobHandle job,
		ILBConstString name,
		ILBConstString scriptFile,
		ILBRenderPassHandle* pass);

	/** 
	* Enable lambertian clamp on an RNM pass.
	* @param pass the pass
	* @param val the lambertian clamp value
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetLambertianClamp(ILBRenderPassHandle pass,
		float val);

	/** 
	* Sets whether to allow negative coefficients on an RNM pass.
	* @param pass the pass
	* @param allow the allow value (default is ILB_AN_DISALLOW_CULL_HORIZON)
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBSetAllowNegative(ILBRenderPassHandle pass,
		ILBRNMAllowNegative allow);

	/**
	* Enables inclusion of a normal component in the RNM pass.
	* @param pass the pass
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBIncludeNormalComponent(ILBRenderPassHandle pass);

	/**
	* Scales the RNM values to the amplitude of the normal component.
	* @param pass the pass
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBRNMMatchNormalIntensity(ILBRenderPassHandle pass);

	/**
	* Normalizes the texture values to the 0..1 range. Stores the original range per entity
	* which can be collected with the getNormalization* functions.
	* @param pass the pass
	* @param perChannel if enabled normalization will be done individually for each channel
	* @return The result of the operation.
	*/
	ILB_DLL_FUNCTION ILBStatus ILBNormalizeTextures(ILBRenderPassHandle pass,
		bool perChannel);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif//BEASTRENDERPASS_H
