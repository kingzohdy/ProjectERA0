
#ifndef __FrameTick_H__
#define __FrameTick_H__

class FrameTick
{
public:
	FrameTick();

	//return dtick
	unsigned int DoFrame();

	float GetFPS()
	{
		return m_CurFPS;
	}

	bool CheckFPSUpdated()
	{
		if(m_bFPSUpdated)
		{
			m_bFPSUpdated = false;
			return true;
		}
		else return false;
	}

	void setFrameLimit(int n);//限制桢数在n以下, n==0表示不限桢

private:

	bool m_bInit;
	bool m_bFPSUpdated;
	unsigned int m_LastTick;
	unsigned int m_FPSAccumTick;
	unsigned int m_FPSAccumFrame;
	float m_CurFPS;

	float m_FPSLimit;
};

#endif