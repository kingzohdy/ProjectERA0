/**
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
 * The Beast framebuffer functions
 */ 

#ifndef BEASTFRAMEBUFFER_H
#define BEASTFRAMEBUFFER_H

#include "beastapitypes.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Default selections of channels when reading out
 * pixel/vertex data
 */
enum ILBChannelSelection {
	ILB_CS_R = 0,
	ILB_CS_G,
	ILB_CS_B,
	ILB_CS_A,
	ILB_CS_Z,
	ILB_CS_RGB,
	ILB_CS_RGBA,
	ILB_CS_RGBAZ,
	ILB_CS_ALL,
};

/** 
 * Gets the number of channels in the framebuffer
 * @param fb the framebuffer to get the channel count for
 * @param channels pointer to the variable to receive the number of channels
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBGetChannelCount(ILBFramebufferHandle fb, int32* channels);

/** 
 * Gets the name for a channel
 * @param fb the framebuffer to get the channel from
 * @param index the index of the channel to get
 * @param name pointer to the string handle to receive the channel name
 * @return The result of the operation.
 * @todo Should this get a vector of channels instead to avoid call overhead? 
 */
ILB_DLL_FUNCTION ILBStatus ILBGetChannelName(ILBFramebufferHandle fb, 
											 int32 index,
											 ILBStringHandle* name);

/** 
 * Gets the resolution for a framebuffer
 * @param fb the framebuffer to get the resolution of
 * @param width pointer to where the width shall be written
 * @param height pointer to where the height shall be written
 * @return The result of the operation.
 * @todo Should we introduce an integer ILBVec2 instead?
 */
ILB_DLL_FUNCTION ILBStatus ILBGetResolution(ILBFramebufferHandle fb, 
										    int32* width,
										    int32* height);

/** 
 * Reads back a region in a framebuffer.
 * The region is specified as inclusive for the lower part and exclusive for
 * the higher part. I.E to read a 512x512 buffer use 0, 0, 512, 512
 * @param fb the framebuffer to read from
 * @param minX the left limit of the region
 * @param minY the lower limit of the region
 * @param maxX the right limit of the region
 * @param maxY the upper limit of the region
 * @param channels what channels to read out from the buffer
 * @param target the buffer to write the result to. 
 * Must be (maxY - minY) * (maxX - minX) * channelCount floats big
 * @return The result of the operation.
 * @todo Should we introduce an integer ILBVec2 instead?
 */
ILB_DLL_FUNCTION ILBStatus ILBReadRegionHDR(ILBFramebufferHandle fb, 
										    int32 minX,
										    int32 minY,
										    int32 maxX,
										    int32 maxY,
											ILBChannelSelection channels,
										    float* target);

/** 
 * Reads back a region in a framebuffer.
 * The region is specified as inclusive for the lower part and exclusive for
 * the higher part. I.E to read a 512x512 buffer use 0, 0, 512, 512
 * @param fb the framebuffer to read from
 * @param minX the left limit of the region
 * @param minY the lower limit of the region
 * @param maxX the right limit of the region
 * @param maxY the upper limit of the region
 * @param channels what channels to read out from the buffer
 * @param gamma the gamma space to encode the image data in
 * @param target the buffer to write the result to. 
 * Must be (maxY - minY) * (maxX - minX) * channelCount bytes big
 * @return The result of the operation.
 * @todo Should we introduce an integer ILBVec2 instead?
 * @todo What about gamma?
 */
ILB_DLL_FUNCTION ILBStatus ILBReadRegionLDR(ILBFramebufferHandle fb, 
											int32 minX,
											int32 minY,
											int32 maxX,
											int32 maxY,
											ILBChannelSelection channels,
											float gamma,
											unsigned char* target);


/** 
 * Destroys and frees all memory related to a framebuffer.
 * The framebuffer handle will be invalid afterwards.\n
 * Note this will happen automatically when the job is 
 * destroyed. Use this function to avoid using more temporary
 * memory than necessary when importing results.
 * @param fb the framebuffer to erase
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBDestroyFramebuffer(ILBFramebufferHandle fb);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // BEASTFRAMEBUFFER_H
