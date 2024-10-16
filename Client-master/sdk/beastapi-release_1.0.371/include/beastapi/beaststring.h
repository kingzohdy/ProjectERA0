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
 * Beast strings is objects encapsulating strings returned
 * from Beast API functions
 */ 
#ifndef BEASTSTRING_H
#define BEASTSTRING_H

#include "beastapitypes.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * Returns the length of the string in number of characters.\n
 * The size of each character is specified by the currently
 * used string encoding. I.E if you get 5 as length and use
 * utf 16 your string should be 10 bytes large.
 * Note the length includes the terminating 0.
 * This method shall never return 0, an empty string is
 * returned as a single termination character
 * @param string the string to query length from
 * @param length a pointer to an integer receiving the
 * length
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBGetLength(ILBStringHandle string,
								int32* length);

/**
 * Copies the contained string to a buffer.
 * The length is defined in the same way as in ILBGetLength
 * @param string the string to copy from
 * @param target a buffer to copy the string to.
 * @param length the size of the buffer. Specified as the 
 * number of characters (that may be different for different
 * encodings) as opposed to number of bytes.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBCopy(ILBStringHandle string,
						   ILBString target,
						   int32 length);

/**
 * Releases a string object.\n Note that strings are not managed
 * through the Beast Manager so it must be released manually
 * or it will be a memory leak.
 * @param string the string to release.
 * @return The result of the operation.
 */
ILB_DLL_FUNCTION ILBStatus ILBReleaseString(ILBStringHandle string);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTSTRING_H

