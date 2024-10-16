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
 * This header is the base for getting platform consistent types for the
 * Beast API
 */ 
#ifndef BEASTAPITYPES_H
#define BEASTAPITYPES_H

#ifdef __GNUC__
#include <stdint.h>
#endif // __GNUC__

// To enforce a certain string encoding use any of
//#define ILB_STRING_UTF8
//#define ILB_STRING_UTF16
//#define ILB_STRING_UTF32
//#define ILB_STRING_ANSI

/**
 *	Defines the different supported string encodings
 */
typedef enum {
	/**
	 * Ansi encoding, the one used by default for windows
	 * source files
	 */
	ILB_SE_ANSI,
	/**
	 * UTF-8 encoding
	 */
	ILB_SE_UTF8,
	
	/**
	 * UTF-16 encoding
	 */
	ILB_SE_UTF16,

	/**
	 * UTF-32 encoding.\n
	 * Currently not supported
	 */
	ILB_SE_UTF32,
} ILBStringEncoding;

/**
 * \def ILB_STRING_ENCODING
 * Defined to be the string encoding that is set.
 * Don't set directly, use one of the following defines:\n
 * ILB_STRING_UTF8\n
 * ILB_STRING_UTF16\n
 * ILB_STRING_UTF32\n
 * ILB_STRING_ANSI\n
 */
/**
 * \typedef ILBCharType
 * Will be typedef:ed to the proper character type depending
 * on the selected string encoding
 */

/// @cond IGNORE_IN_DOCUMENTATION
// Try to auto detect a string encoding if no was supplied
#if !(defined(ILB_STRING_UTF8) || defined(ILB_STRING_UTF16) || defined(ILB_STRING_UTF32) || defined(ILB_STRING_ANSI))
#if defined(WIN32)
#ifdef UNICODE
#define ILB_STRING_UTF16
#else // UNICODE
#define ILB_STRING_ANSI
#endif // UNICODE
#else // WIN32
#define ILB_STRING_UTF8
#endif // WIN32
#endif // !(defined(ILB_STRING_UTF8) || defined(ILB_STRING_UTF16) || defined(ILB_STRING_UTF32) || defined(ILB_STRING_ANSI))
/// @endcond

/**
 * Character type for 8 bit strings
 */
typedef char ILBChar8;

#if defined(WIN32)
/**
 * Character type for 16 bit strings
 */
typedef wchar_t ILBChar16;
#else // defined(WIN32)
/**
 * Character type for 16 bit strings
 */
typedef unsigned short ILBChar16;
#endif // defined(WIN32)

/**
 * Character type for 32 bit strings.
 * May need special treatment for native utf32
 * platforms (i.e wchar_t instead of int32)
 */
typedef unsigned int ILBChar32;

#ifdef beastapi_EXPORTS
// This is for inside the API. By setting char type
// to ILBDummyChar we guarantee that someone accidentially
// converts strings or string constants to ILBString 
// or ILBConstString without thinking about encoding
struct ILBDummyChar {
	int unused;
};

/**
 * The type that's used for characters in strings
 * Will be different depending on selected 
 * string encoding
 */
typedef ILBDummyChar ILBCharType;
/// @cond IGNORE_IN_DOCUMENTATION
#define ILB_STRING_ENCODING ILB_SE_ANSI
/// @endcond 
#else // beastapi_EXPORTS

#if defined(ILB_STRING_ANSI)
typedef ILBChar8 ILBCharType;
#if defined(WIN32)
#define ILB_STRING_ENCODING ILB_SE_ANSI
#else // WIN32
#error ANSI strings only supported on Windows platforms
#endif // WIN32
#elif defined(ILB_STRING_UTF8)
typedef ILBChar8 ILBCharType;
#define ILB_STRING_ENCODING ILB_SE_UTF8
#elif defined(ILB_STRING_UTF16)
typedef ILBChar16 ILBCharType;
#define ILB_STRING_ENCODING ILB_SE_UTF16
#elif defined(ILB_STRING_UTF32)
#error UTF32 currently not supported
typedef ILBChar32 ILBCharType;
#define ILB_STRING_ENCODING ILB_SE_UTF32
#else
#error No string type defined
#endif //ILB_STRING_UTF8

#endif //beastapi_EXPORTS


/**
  * Status codes for Beast API calls
  */
typedef enum {
	/**
	* The call was successfully completed!
	*/
	ILB_ST_SUCCESS = 0,
	
	/**
	 * One or more parameters were not in valid range or in some other way
	 * not valid for this call.
	 */
	ILB_ST_INVALID_PARAMETER, 

	/**
	 * Beast failed to allocate memory somewhere down the line of this call
	 */
	ILB_ST_MEMORY_ALLOC_ERROR,

	/**
	 * An object with the same name already existed
	 */
	ILB_ST_DUPLICATE_NAME_ERROR,

	/**
	 * This function is not implemented yet. Should only happen internally
	 */
	ILB_ST_FUNCTION_NOT_IMPLEMENTED,

	/**
	 * The object the function was called on was in a state where
	 * the function isn't valid to call.
	 */
	ILB_ST_INVALID_OBJECT_STATE,
	
	/**
	 * The object handle used is not valid.
	 */
	ILB_ST_INVALID_HANDLE,
	
	/**
	 * There was some kind of file problem (invalid filename, permission
	 * etc).
	 */
	ILB_ST_FILE_IO_ERROR,

	/**
	 * A handle to an unknown object was requested
	 */
	ILB_ST_UNKNOWN_OBJECT,

	/**
	 * The requested functionality is not supported in the current configuration
	 */
	ILB_ST_NOT_SUPPORTED,

	/**
	 * The api generated an exception we didn't expect.
	 */
	ILB_ST_UNHANDLED_EXCEPTION,

	/**
	 * An external tool returned an error
	 */
	ILB_ST_JOB_EXECUTION_FAILURE,

	/**
	* Atlasing failed.
	*/
	ILB_ST_ATLAS_EXECUTION_FAILURE,


	/**
	 * Dummy entry to be able to loop over all errors
	 */
	ILB_ST_LAST_ERROR,
} ILBStatus;

/**
 * Beast api string type. Represents different
 * things depending on the selected string encoding to
 * make sure it's compatible with string constants.
 */
typedef ILBCharType* ILBString;

/**
 * Beast api const string type. Represents different
 * things depending on the selected string encoding to
 * make sure it's compatible with string constants.
 */
typedef const ILBCharType* ILBConstString;


#ifdef WIN32
/**
  * Integer defined to be 32 bits regardless of environment
  */
typedef __int32 int32;

/**
  * Integer defined to be 64 bits regardless of environment
  */
typedef __int64 int64;

/**
 * Unsigned integer defined to be 32 bits regardless of environment
 */
typedef unsigned __int32 uint32;

/**
 * Unsigned integer defined to be 64 bits regardless of environment
 */
typedef unsigned __int64 uint64;

#elif defined(__GNUC__)

typedef int32_t int32;
typedef int64_t int64;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uint32 DWORD;
#else
#error Could not define int32 and friends.
#endif

/**
 * Bool type with a well defined size to avoid
 * compatibility-problems.
 */
typedef int32 ILBBool;

/**
 * Two dimensional geometric vector type.\n
 * Used for both points and vectors
 */
typedef struct ILBVec2 {
	/// x
	float x;
	/// y
	float y;
#ifdef __cplusplus
	ILBVec2() {}
	ILBVec2(float _x, float _y): x(_x), y(_y) {}
#endif // __cplusplus
} ILBVec2;

/**
 * Three dimensional geometric vector type.\n
 * Used for both points and vectors, what is what
 * should be obvious from the context.
 */
typedef struct ILBVec3 {
	/// x
	float x;
	/// y
	float y;
	/// z
	float z;
#ifdef __cplusplus
	ILBVec3() {}
	ILBVec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}
#endif // __cplusplus
} ILBVec3;

/**
 * Color definition
 * All colors are expressed in linear space as opposed to gamma corrected.\n
 * High dynamic range supported.
 */
typedef struct ILBLinearRGB {
	/// Red component
	float r;
	/// Green component
	float g;
	/// Blue component
	float b;
#ifdef __cplusplus
	ILBLinearRGB() {}
	ILBLinearRGB(float _r, float _g, float _b): r(_r), g(_g), b(_b) {}
#endif // __cplusplus
} ILBLinearRGB;

/**
 * Color with alpha definition 
 * All colors are expressed in linear space as opposed to gamma corrected.\n
 * Alpha values must be between 0 and 1.0.
 */
typedef struct ILBLinearRGBA {
	/// Red component
	float r;
	/// Green component
	float g;
	/// Blue component
	float b;
	/// Alpha
	float a;
#ifdef __cplusplus
	ILBLinearRGBA() {}
	ILBLinearRGBA(float _r, float _g, float _b, float _a): r(_r), g(_g), b(_b), a(_a) {}
#endif // __cplusplus
} ILBLinearRGBA;

/**
 * Matrix for transformations.\n
 * The data is stored line by line (the opposite of OpenGL)\n 
 * Indices are laid out like this:\n
 * <tt>
 * (00 01 02 03)\n
 * (04 05 06 07)\n
 * (08 09 10 11)\n
 * (12 13 14 15)\n
 * </tt>
 * \c
 */
typedef struct {
	/**
	 * The actual data in the matrix
	 */
	float m[16];
} ILBMatrix4x4;

/// @cond IGNORE_IN_DOCUMENTATION 
/**
 * Macro to define handle types.
 * Inspired by windows headers
 */
#define ILB_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name;
/// @endcond 

/**
 * \struct ILBManagerHandle
 * Handle for Beast managers\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBManagerHandle)
/// @endcond

/** 
 * \struct ILBMeshHandle
 * Handle for Beast meshes\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBMeshHandle);
/// @endcond

/** 
 * \struct ILBSceneHandle
 * Handle for Beast scenes\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBSceneHandle);
/// @endcond

/** 
 * \struct ILBInstanceHandle
 * Handle for Beast instance\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBInstanceHandle);
/// @endcond

/** 
 * \struct ILBCameraHandle
 * Handle for Beast cameras\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBCameraHandle);
/// @endcond

/** 
 * \struct ILBLightHandle
 * Handle for Beast light sources\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBLightHandle);
/// @endcond

/** 
 * \struct ILBMaterialHandle
 * Handle for Beast materials\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBMaterialHandle);
/// @endcond

/** 
 * \struct ILBTextureHandle
 * Handle for Beast textures\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBTextureHandle);
/// @endcond

/** 
 * \struct ILBJobHandle
 * Handle for Beast jobs\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBJobHandle);
/// @endcond


/** 
 * \struct ILBStringHandle
 * Handle for Beast strings\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBStringHandle);
/// @endcond

/** 
 * \struct ILBTargetHandle
 * Handle for Beast target specification\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBTargetHandle);
/// @endcond


/** 
 * \struct ILBFramebufferHandle
 * Handle for Beast framebuffers
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBFramebufferHandle);
/// @endcond


/** 
 * \struct ILBTargetEntity
 * Handle for Beast target entities
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBTargetEntityHandle);
/// @endcond

/** 
 * \struct ILBRenderPassHandle
 * Handle for Beast render pass\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBRenderPassHandle);
/// @endcond

/** 
 * \struct ILBLightPassEntryHandle
 * Handle for Light pass entry\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBLightPassEntryHandle);
/// @endcond

/** 
 * \struct ILBPointCloudHandle
 * Handle for Beast point clouds\n
 * Intentionally hidden implementation
 */
/// @cond IGNORE_IN_DOCUMENTATION 
ILB_DECLARE_HANDLE(ILBPointCloudHandle);
/// @endcond

/**
 * Enum defining if a light link is inclusive or exclusive
 */
typedef enum {
	/**
	 * Excludes the supplied lights/objects
	 */
	ILB_LL_EXCLUDING = 0,
	
	/**
	 * Includes the supplied lights/objects
	 */
	ILB_LL_INCLUDING,
} ILBLightLinkMode;

/**
 * Setup for getting functions exported or imported from the DLL
 * on windows
 */
#ifdef WIN32
#ifdef beastapi_EXPORTS
#define ILB_DLL_FUNCTION __declspec(dllexport)
#else
#define ILB_DLL_FUNCTION __declspec(dllimport)
#endif // beastapi_EXPORTS
#else // WIN32
#define ILB_DLL_FUNCTION 
#endif // WIN32


#endif //BEASTAPITYPES_H


