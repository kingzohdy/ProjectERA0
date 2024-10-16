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
 * The api for specifying materials in beast
 */ 
#ifndef BEASTMATERIAL_H
#define BEASTMATERIAL_H
#include "beastapitypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Defines the different channels in the Beast
 * API shading model
 */
typedef enum {
	ILB_CC_DIFFUSE = 0,
	ILB_CC_SPECULAR,
	ILB_CC_EMISSIVE,
	ILB_CC_TRANSPARENCY,
	ILB_CC_SHININESS,
	ILB_CC_REFLECTION,
	ILB_CC_TOTAL_CHANNELS
} ILBMaterialChannel;

/**
 * Add a material to the scene
 * @param scene the scene the material should be a part of
 * @param name the name of the material, must be unique within the scene.
 * @param material a pointer to the material handle where the created 
 * material should be stored
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCreateMaterial(ILBSceneHandle scene, 
	ILBConstString name,
	ILBMaterialHandle* material);

/**
 * Checks if the material exists, if it does, it returns its handle
 * @param scene the scene the material is a part of
 * @param name the name of the material
 * @param material a pointer to the material handle to store the result
 * in
 * @return ILB_ST_SUCCESS if the material was found, 
 * ILB_ST_UNKNOWN_OBJECT if it wasn't
 */
ILB_DLL_FUNCTION ILBStatus ILBFindMaterial(ILBSceneHandle scene, 
									 ILBConstString name,
									 ILBMaterialHandle* material);

/**
 * Sets the color for a channel on a material. Note that color is 
 * not supported for shininess or reflectivity.
 * @param material the material to apply the color on
 * @param channel the channel to set the color on
 * @param color pointer to the color to set
 */
ILB_DLL_FUNCTION ILBStatus ILBSetMaterialColor(ILBMaterialHandle material, 
									 ILBMaterialChannel channel,
									 const ILBLinearRGBA* color);

/**
 * Sets a scale for a channel on a material
 * Will be multiplied with both colors and textures on the channel
 * Can also be used to set the shininess of the material. The default value
 * of the scale is 1.0f.
 * @param material the material to apply the color on
 * @param channel the channel to set the color on
 * @param scale the scale to set
 */
ILB_DLL_FUNCTION ILBStatus ILBSetMaterialScale(ILBMaterialHandle material, 
											   ILBMaterialChannel channel,
											   float scale);

/**
 * Sets the texture for a channel on a material.
 * Overrides anything set with ILBSetMaterialColor (regardless if called
 * before or after).
 * @param material the material to apply the texture on
 * @param channel the channel to set the texture on.
 * @param texture the texture to apply
 */
ILB_DLL_FUNCTION ILBStatus ILBSetMaterialTexture(ILBMaterialHandle material, 
									   ILBMaterialChannel channel,
									   ILBTextureHandle texture);

/**
 * Sets the material to use vertex color for a channel.
 * This overrides anything set with ILBSetMaterialColor or ILBSetMaterialTexture
 * (regardless if called before or after)
 * @param material the material to apply vertex colors on
 * @param channel the channel to set the vertex colors on
 * support multiple vertex color sets
 */
ILB_DLL_FUNCTION ILBStatus ILBSetMaterialUseVertexColors(ILBMaterialHandle material, 
											   ILBMaterialChannel channel);

/**
 * Sets what UV layer should be used for the texture
 * for a color channel.
 * @param material the material to change texture channel for
 * @param channel the channel to set the texture on
 * @param uvLayerName the name of the UV layer on the mesh to use.
 * This parameter can not be checked at call time and if the
 * UV layer is not present it will fall back on default
 */
ILB_DLL_FUNCTION ILBStatus ILBSetChannelUVLayer(ILBMaterialHandle material, 
										ILBMaterialChannel channel,
										ILBConstString uvLayerName);

/**
* Sets if the material should use the alpha value of the diffuse channel
* as transparency. This is disabled by default.
* @param material the material to change the setting on
* @param alphaAsTransparency the new setting
*/
ILB_DLL_FUNCTION ILBStatus ILBSetAlphaAsTransparency(ILBMaterialHandle material, 
									   ILBBool alphaAsTransparency);


/**
* Sets color correction on the primary GI calculations on the material. Default
* values are 1.0 for intensity and 1.0 for saturation.
* @param material the material to change the setting on
* @param intensity the desired primary GI intensity
* @param saturation the desired primary GI saturation
*/
ILB_DLL_FUNCTION ILBStatus ILBSetPrimaryGICorrection(ILBMaterialHandle material, 
									   float intensity,
									   float saturation);

/**
* Sets color correction on the secondary GI calculations on the material. Default
* values are 1.0 for intensity and 1.0 for saturation.
* @param material the material to change the setting on
* @param intensity the desired secondary GI intensity
* @param saturation the desired secondary GI saturation
*/
ILB_DLL_FUNCTION ILBStatus ILBSetSecondaryGICorrection(ILBMaterialHandle material, 
									   float intensity,
									   float saturation);


/**
* Sets boost and scaling factors for different aspects of the material in GI 
* calculations. Default values are all 1.0.
* @param material the material to change the setting on
* @param diffuseBoost the value to boost the diffuse value with in GI calculations
* @param emissiveScale the value to scale the emissive value with in GI calculations
* @param specularScale the value to scale the specular value with in GI calculations
*/
ILB_DLL_FUNCTION ILBStatus ILBSetGIScale(ILBMaterialHandle material, 
									   float diffuseBoost,
									   float emissiveScale,
									   float specularScale);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTMATERIAL_H

