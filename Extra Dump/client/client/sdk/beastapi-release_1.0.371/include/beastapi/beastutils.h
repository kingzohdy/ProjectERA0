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
* Utility functions for the Beast API
*/ 
#ifndef BEASTUTILS_H
#define BEASTUTILS_H

#include "beastapitypes.h"
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
  * Converts an error code into a string for human readable error reporting.
  * @param error the error code to convert
  * @param targetString the string to receive the message.
  * @returns Result of the operation
  */
ILB_DLL_FUNCTION ILBStatus ILBErrorToString(ILBStatus error, 
								  ILBStringHandle* targetString);

/**
 * Returns the last error that happened in this thread.\n
 * Error strings returned from this function may 
 * potentially be invalidated and undefined by other calls
 * to the beast api from this thread.
 * @param targetString the string to receive the message.
 * @returns Result of the operation
 */
ILB_DLL_FUNCTION ILBStatus ILBGetExtendErrorInformation(ILBStringHandle* targetString);

/**
 * Dumps the memory stats of the dll to the Debug console and puts the API
 * in an undefined state. NEVER CALL ANY OTHER BEAST API FUNCTIONS AFTER THIS!
 * Only works on debug builds, this means that it CAN only be used
 * internally for now. 
 * @returns ILB_ST_SUCCESS if there are no leaks or it's called running 
 * from a Release build
 * @todo Should we hide this function?
 */
ILB_DLL_FUNCTION ILBStatus ILBDumpMemoryStats();


#ifdef __cplusplus
}
#endif // __cplusplus


#endif // BEASTUTILS_H
