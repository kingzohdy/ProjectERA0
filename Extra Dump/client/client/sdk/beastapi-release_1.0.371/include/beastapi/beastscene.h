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
 * The api for specifying scenes in beast
 */ 
#ifndef BEASTSCENE_H
#define BEASTSCENE_H

#include "beastapitypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


	/** 
	 * Begins creation of a scene.
	 * @param beastManager the beast manager this scene will be associated with
	 * @param uniqueName a unique name for the scene.
	 * @param target a pointer to a Beast scene object that will receive the created object
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBBeginScene(ILBManagerHandle beastManager,
		ILBConstString uniqueName,
		ILBSceneHandle* target);

	/** 
	 * Releases the scene data.\n
	 * All handles created in this scene will be invalid after this call.
	 * @param scene the scene to to release.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBReleaseScene(ILBSceneHandle scene);
	
	/** 
	 * Finalizes this scene
	 * Any future call to modify this scene or any of its objects will fail
	 * @param scene the scene to finalize
	 * @return The result of the operation.
	 */
	ILB_DLL_FUNCTION ILBStatus ILBEndScene(ILBSceneHandle scene);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTSCENE_H
