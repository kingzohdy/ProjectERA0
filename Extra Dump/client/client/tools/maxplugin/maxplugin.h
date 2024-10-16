
#ifndef __MAXPLUGIN__H
#define __MAXPLUGIN__H

#include "exp_types.h"

namespace DKEXP
{
	// Document SummaryInfo properties
#define PID_MANAGER				0x0000000E
#define PID_COMPANY				0x0000000F
#define PID_CATEGORY			0x00000002
#define PID_HEADINGPAIR			0x0000000C
#define PID_DOCPARTS			0x0000000D

	extern ClassDesc* GetMaxPluginDesc();

	class MaxPlugin : public UtilityObj {
	public:
		IUtil *m_iu;
		Interface *m_ip;
		HWND hPanel;

		MaxPlugin();
		~MaxPlugin();

		void BeginEditParams(Interface *ip,IUtil *iu);
		void EndEditParams(Interface *ip,IUtil *iu);
		void DeleteThis() {}

		void Init(HWND hWnd);
		void Destroy(HWND hWnd);

		TimeValue m_animStart;
		TimeValue m_animEnd;
	};
}

#endif // __MaxPlugin__H
