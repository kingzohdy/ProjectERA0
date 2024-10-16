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
 * The api for specifying light sources in beast
 */ 
#ifndef BEASTLIGHTSOURCE_H
#define BEASTLIGHTSOURCE_H

#include "beastapitypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Describes different falloff for light sources
 */
typedef enum {
	/**
	 * Computes falloff as <tt>(1.0f / distance) ^ exponent</tt>\n
	 * Note that an exponent of 0 gives no falloff
	 */
	ILB_FO_EXPONENT = 0,
	/**
	 * Computes falloff as 
	 <tt>(max((maxRange - distance), 0) / maxRange) ^ exponent</tt>\n
	 */
	ILB_FO_MAX_RANGE
} ILBFalloffType;

/** 
 * Add a point light to the scene
 * @param scene the scene the point light should be a part of
 * @param name the name of the point light, must be unique within the scene.
 * @param transform the object space to world space transform for the light
 * @param intensity the colored intensity of the light source
 * @param light the handle to store the generated light source in
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCreatePointLight(ILBSceneHandle scene, 
									   ILBConstString name, 
									   const ILBMatrix4x4* transform,
									   const ILBLinearRGB* intensity,
									   ILBLightHandle* light);

/** 
 * Add a spot light to the scene. It points in the negative Y-direction
 * by default, use the matrix to point it in a different direction.
 * The default cone angle is 90 degrees.
 * @param scene the scene the light should be a part of
 * @param name the name of the light, must be unique within the scene.
 * @param transform the object space to world space transform for the light.
 * @param intensity the colored intensity of the light source
 * @param light the handle to store the generated light source in
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCreateSpotLight(ILBSceneHandle scene, 
									  ILBConstString name, 
									  const ILBMatrix4x4* transform,
									  const ILBLinearRGB* intensity,
									  ILBLightHandle* light);

/** 
 * Add a directional light to the scene
 * It points in negative Y direction by default, use the matrix to change its
 * direction.
 * @param scene the scene the directional light should be a part of
 * @param name the name of the directional light, must be unique within the scene.
 * @param transform the object space to world space transform for the light. 
 * @param intensity the colored intensity of the light source
 * @param light the handle to store the generated light source in
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCreateDirectionalLight(ILBSceneHandle scene, 
											 ILBConstString name, 
											 const ILBMatrix4x4* transform,
											 const ILBLinearRGB* intensity,
											 ILBLightHandle* light);

/** 
 * Add an area light to the scene
 * It points in negative Y direction by default, use the matrix to change its
 * direction.
 * The area light extends -1.0 to 1.0 in the X/Z dimensions, use scaling
 * to control its area
 * @param scene the scene the light should be a part of
 * @param name the name of the light, must be unique within the scene.
 * @param transform the object space to world space transform for the light. 
 * It controls the area of the light as well.
 * @param intensity the colored intensity of the light source
 * @param light the handle to store the generated light source in
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCreateAreaLight(ILBSceneHandle scene, 
									  ILBConstString name, 
									  const ILBMatrix4x4* transform,
									  const ILBLinearRGB* intensity,
									  ILBLightHandle* light);

/** 
 * Add a window light to the scene
 * It points in negative Y direction by default, use the matrix to change its
 * direction.
 * The window light extends -1.0 to 1.0 in the X/Z dimensions, use scaling
 * to control its area
 * @param scene the scene the light should be a part of
 * @param name the name of the light, must be unique within the scene.
 * @param transform the object space to world space transform for the light. 
 * It controls the area of the light as well
 * @param intensity the colored intensity of the light source
 * @param light the handle to store the generated light source in
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCreateWindowLight(ILBSceneHandle scene, 
										ILBConstString name, 
										const ILBMatrix4x4* transform,
										const ILBLinearRGB* intensity,
										ILBLightHandle* light);



/**
 * Flags whether the light cast shadows or not.
 * Disabled by default
 * @param light the light source in question.
 * @param castShadows sets if shadow casting should be enabled or not
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetCastShadows(ILBLightHandle light, ILBBool castShadows);

/**
 * Sets a radius for the light source as a shadow caster.
 * Only valid for point and spot lights.
 * The radius is 0 by default.
 * @param light the light source to set the radius on.
 * @param radius the radius.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetShadowRadius(ILBLightHandle light, float radius);

/**
 * Sets the angle covered of the sky for a directional light 
 * or window light for shadow casting purposes.
 * The angle is 0 by default
 * @param light the light source to set the radius on.
 * @param angleRadians the angle in radians.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetShadowAngle(ILBLightHandle light, float angleRadians);

/**
 * Sets the maximum number of shadow samples for the light source.
 * Set to 1 by default
 * @param light the light source to set the radius on.
 * @param samples the number of samples.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetShadowSamples(ILBLightHandle light, int32 samples);

/** 
 * Adds a light centric light link list
 * @param light the light source to add light links to
 * @param mode sets whether to link or unlink the light to the instances
 * @param instances an array of instances to link to the light
 * @param count the number of instances present in the instances
 * array
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBAddLightLightLinks(ILBLightHandle light, 
										 ILBLightLinkMode mode,
										 const ILBInstanceHandle* instances, 
										 int32 count);

/**
 * Sets the falloff for a light source.
 * Not valid for directional lights.
 * By default falloff is disabled.
 * By default clamping is enabled.
 * @param light the light source to set falloff for
 * @param type the falloff type to use
 * @param exponent sets the exponent for the falloff
 * @param cutoff sets the influence range for the light source.
 * It affects both falloff types, for exponent it's a hard cutoff
 * where the light stops affecting at all.
 * For max range it sets where the light intensity fades
 * to zero
 * @param clampToOne sets whether to clamp the falloff to be lower or equal
 * to one. If set to false, the falloff is allowed to scale the intensity of
 * the light up as well as down.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetFalloff(ILBLightHandle light, 
										 ILBFalloffType type, 
										 float exponent, 
										 float cutoff,
										 ILBBool clampToOne);


/**
 * Sets the cone angle for a spotlight.
 * The cone is given in radians for the entire cone (as opposed to the angle 
 * towards the forward direction). The penumbra angle is the angle from the
 * edge of the cone over which the intensity falls off to zero.
 * The effective spread of the cone is
 * max( angleRadians, angleRadians + 2*penumbraAngleRadians ) since the
 * penumbra angle can be both positive and negative.
 * The default cone angle is PI / 2 (90 degrees), 
 * The default is penumbra angle 0
 * The default penumbra exponent is 1
 * @param light the light source to set cone angle for
 * @param angleRadians the angle in radians for the cone
 * @param penumbraAngleRadians the angle of the penumbra of the spot light.
 * It's given as the difference from the cone angle and can be both
 * negative and positive.
 * @param penumbraExponent the exponent for the gradient in the penumbra
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetSpotlightCone(ILBLightHandle light, 
									   float angleRadians,
									   float penumbraAngleRadians,
									   float penumbraExponent);

/**
 * Sets scale for direct and indirect light intensity for a light source.
 * @param light the light source to set intensities
 * @param directIntensity direct light intensity
 * @param indirectIntensity indirect light intensity
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetIntensityScale(ILBLightHandle light, 
									   float directIntensity,
									   float indirectIntensity);

/**
 * Adds a light ramp entry for falloff calculation.
 * The ramp extends from 0 to 1 in light space, use the transformation
 * matrix to control the scale.\n
 * Works on point and spot lights.
 * @param light light source to manipulate
 * @param position position in the ramp. Must be greater than 0 and 
 * greater than the last position.
 * @param value color of the given position in the ramp.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetLightRampEntry(ILBLightHandle light,
										float position,
										const ILBLinearRGB* value);

/**
 * Sets a projected texture for a light source (gobo).\n
 * Only works for spot lights
 * @param light light to add gobo on
 * @param texture texture to use as gobo
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBSetLightProjectedTexture(ILBLightHandle light,
													   ILBTextureHandle texture);


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTLIGHTSOURCE_H 
