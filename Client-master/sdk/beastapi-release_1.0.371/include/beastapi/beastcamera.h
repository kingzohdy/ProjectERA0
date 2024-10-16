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
 * The beast camera function definitions
 */ 
#ifndef BEASTCAMERA_H
#define BEASTCAMERA_H
#include "beastapitypes.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	
	/**
	 * Environment camera types
	 */
	typedef enum {
		/**
		 * Cubemap environment camera.
		 */
		ILB_ECT_CUBEMAP = 0,
		
		/**
		 * Ball environment camera.
		 */
		ILB_ECT_BALL,

		/**
		 * Latlong environment camera.
		 */
		ILB_ECT_LATLONG
	} ILBEnvironmentCameraType;

	/** 
	 * Add a camera to the scene.
	 * The camera looks in the negative z direction, positive x in camera
	 * space maps to right in screen space.
	 * positive y maps to up i screen space.
	 * @param scene the scene the camera should be a part of
	 * @param name the name of the camera, must be unique within the scene.
	 * @param transform the object space to world space transform for this camera
	 * @param camera the handle to store the generated camera in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreatePerspectiveCamera(ILBSceneHandle scene, 
		ILBConstString name, 
		const ILBMatrix4x4* transform,
		ILBCameraHandle* camera);

	/**
	 * Add an environment camera to the scene.
	 * @param scene the scene the camera should be a part of
	 * @param name the name of the camera, must be unique within the scene.
	 * @param transform the object space to world space transform for this camera
	 * @param type the type of environment camera, i.e Ball, Cubemap or Latlong
	 * @param camera the handle to store the generated camera in
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBCreateEnvironmentCamera(ILBSceneHandle scene, 
		ILBConstString name, 
		const ILBMatrix4x4* transform,
		ILBEnvironmentCameraType type,
		ILBCameraHandle* camera);

	/** 
	 * Sets the fov of the camera.
	 * Only works on perspective cameras.
	 * The vertical fov will be generated from the horizontal fov and the image 
	 * dimensions
	 * @param camera the camera to set fov for
	 * @param horizontalFovRadians the field of view in the X direction in radians.\n
	 * Note that it refers to the complete field of vision, not the angle to the
	 * forward direction.
	 * Negative horizontalFovRadians is currently not supported
	 * @param pixelAspectRatio the aspect ratio of a pixel, expressed as the x / y
	 * @bug The aspect ratio currently only works with 4/3 image sizes.
	 * It needs to be compensated for the resolution given in config.xml
	 */
	ILB_DLL_FUNCTION ILBStatus ILBSetFov(ILBCameraHandle camera, 
								 float horizontalFovRadians,
								 float pixelAspectRatio);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTCAMERA_H


