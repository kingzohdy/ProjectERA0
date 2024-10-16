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
* The beast job function definitions
*/ 
#ifndef BEASTJOB_H
#define BEASTJOB_H
#include "beastapitypes.h"

/**
* Status codes for Beast API calls
*/
typedef enum {
	/**
	* This was a triumph! I'm making a note here; Huge Success!
	*/
	ILB_JS_SUCCESS = 0,

	/**
	* Job was aborted by external means.
	*/
	ILB_JS_CANCELLED,

	/**
	* Beast does not have a valid license.
	*/
	ILB_JS_INVALID_LICENSE,

	/**
	* Error parsing the command line
	*/
	ILB_JS_CMDLINE_ERROR,

	/**
	* Error parsing the config files
	*/
	ILB_JS_CONFIG_ERROR,

	/**
	* Beast crashed, sorry.
	*/
	ILB_JS_CRASH,

	/**
	* Other Error
	*/
	ILB_JS_OTHER_ERROR = 0x10000001

} ILBJobStatus;

/**
 * Sets how to handle the Beast window when rendering
 */
typedef enum {
	/**
	 * Don't display the render window
	 */
	ILB_SR_NO_DISPLAY = 0,

	/**
	* Show the render window and close it when the rendering is done.
	*/
	ILB_SR_CLOSE_WHEN_DONE,

	/**
	* Show the render window and keep it open until the user closes it or the job is destroyed.
	*/
	ILB_SR_KEEP_OPEN
} ILBShowResults;

/**
 * Sets how beast should render distributed
 */
typedef enum {
	/**
	 * Force a local render
	 */
	ILB_RD_FORCE_LOCAL = 0,

	/**
	 * Render distributed if possible, otherwise
	 * fallback on local rendering
	 */
	ILB_RD_AUTODETECT,

	/**
	 * Force a distributed render, fails if distribution is not
	 * available
	 */
	ILB_RD_FORCE_DISTRIBUTED
} ILBDistributionType;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/** 
 * @param beastManager the beast manager to create the job for
 * @param uniqueName a unique name for the job
 * @param scene the scene to render
 * @param jobXML the config XML file to use. Use empty string for default
 * rendering (no GI or supersampling)
 * @param job pointer to where the job handle should be stored
 */
ILB_DLL_FUNCTION ILBStatus ILBCreateJob(ILBManagerHandle beastManager,
		ILBConstString uniqueName, 
		ILBSceneHandle scene, 
		ILBConstString jobXML, 
		ILBJobHandle* job);

/** 
 * Destroys a job
 * @param job the job to destroy
 */
ILB_DLL_FUNCTION ILBStatus ILBDestroyJob(ILBJobHandle job);

/**
* Sets the output directory fpr the job. If this function is not called output files will
* end up in the cache hierarchy.
* @param job the job to set directory for
* @param path the path to the output directory
*/
ILB_DLL_FUNCTION ILBStatus ILBSetJobOutputPath(ILBJobHandle job, ILBConstString path);

/** 
 * Starts a job
 * @param job the job to start
 * @param showResults Specifies the behaviour of the render window
 * @param distribution Sets how to distribute the rendering
 */
ILB_DLL_FUNCTION ILBStatus ILBStartJob(ILBJobHandle job, ILBShowResults showResults, ILBDistributionType distribution);

/** 
* Waits until a job is done or until there is progress updates
* @param job The job to wait for
* @param timeout The maximum time to wait in milliseconds
*/
ILB_DLL_FUNCTION ILBStatus ILBWaitJobDone(ILBJobHandle job, int32 timeout);

/** 
* Checks if the job is running.
* @param job The job to check
* @param result Is set to true if job is running, false otherwise
*/
ILB_DLL_FUNCTION ILBStatus ILBIsJobRunning(ILBJobHandle job, ILBBool* result);

/** 
* Checks if the job is completed. Note that a running job can be completed if
* the user has selected to keep the render window open. A job that is not running
* might not have finished if it was aborted or had errors.
* @param job the job to check
* @param result set to true if the job is completed, false otherwise
*/
ILB_DLL_FUNCTION ILBStatus ILBIsJobCompleted(ILBJobHandle job, ILBBool* result);

/**
* Returns the result of the job as a JobStatus
* @param job the job to get the result for
* @param status pointer to the JobStatus
*/
ILB_DLL_FUNCTION ILBStatus ILBGetJobResult(ILBJobHandle job, ILBJobStatus* status);

/**
 * Cancels a running job
 * @param job the job to cancel
 */
ILB_DLL_FUNCTION ILBStatus ILBCancelJob(ILBJobHandle job);

/**
 * Gets the current status of a job.
 * @param job the job to get progress for
 * @param jobName pointer to a string object that receives the name of job being executed
 * Set to 0 to ignore this parameter. 
 * @param progress to the completion percentage of the current activity
 */
ILB_DLL_FUNCTION ILBStatus ILBGetJobProgress(ILBJobHandle job, ILBStringHandle* jobName, int32* progress);

/**
 * Checks if the progress of a Job has been updated since the last time ILBGetJobProgress was called.
 * @param job The job to check if it has progress
 * @param newActivity set to true if a new activity has started
 * @param newProgress set to true if the progress has been updated
 */
ILB_DLL_FUNCTION ILBStatus ILBJobHasNewProgress(ILBJobHandle job, ILBBool* newActivity, ILBBool* newProgress);

/**
 * Convenience function to execute a Beast job.
 * Blocks until the job is done or fails
 * @param bm the beast manager to use
 * @param job the job to execute
 * @param showResults sets how the beast window should be handled
 * @param distribution Sets how to distribute the rendering
 * @param status the result of the rendering
 */
ILB_DLL_FUNCTION ILBStatus ILBExecuteBeast(ILBManagerHandle bm, 
								   ILBJobHandle job,
								   ILBShowResults showResults,
								   ILBDistributionType distribution, 
								   ILBJobStatus* status);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif//BEASTJOB_H
