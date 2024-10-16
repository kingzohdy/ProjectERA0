
#ifndef __Timer_H__
#define __Timer_H__

#include "OgrePrerequisites.h"

namespace Ogre
{
	class _OgreExport Timer
	{
	private:
		long mZeroClock;

		uint mStartTick;
		int64 mLastTime;
		int64 mStartTime;
		int64 mFrequency;

		uint mTimerMask;

	public:
		/** Timer constructor.  MUST be called on same thread that calls getMilliseconds() */
		Timer();
		~Timer();

		/** Method for setting a specific option of the Timer. These options are usually
		specific for a certain implementation of the Timer class, and may (and probably
		will) not exist across different implementations.  reset() must be called after
		all setOption() calls.
		@par
		Current options supported are:
		<ul><li>"QueryAffinityMask" (DWORD): Set the thread affinity mask to be used
		to check the timer. If 'reset' has been called already this mask should
		overlap with the process mask that was in force at that point, and should
		be a power of two (a single core).</li></ul>
		@param
		strKey The name of the option to set
		@param
		pValue A pointer to the value - the size should be calculated by the timer
		based on the key
		@return
		On success, true is returned.
		@par
		On failure, false is returned.
		*/
		bool setOption( const String& strKey, const void* pValue );

		/** Resets timer */
		void reset();

		/** Returns milliseconds since initialisation or last reset */
		uint getMilliseconds();

		/** Returns microseconds since initialisation or last reset */
		uint getMicroseconds();

		/** Returns milliseconds since initialisation or last reset, only CPU time measured */	
		uint getMillisecondsCPU();

		/** Returns microseconds since initialisation or last reset, only CPU time measured */	
		uint getMicrosecondsCPU();

		static uint getSystemTick();
	};
}

#endif