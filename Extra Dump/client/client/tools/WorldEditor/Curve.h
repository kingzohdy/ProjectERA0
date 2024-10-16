// Curve.h

#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <algorithm>
#include "OgreVector3.h"
using namespace Ogre;
using namespace std;

// 线性差值
template <class T, class U>
T Lerp (const T& A, const T& B, const U& Alpha)
{
	return (T)(A + Alpha*(B-A));
}

// 三次差值
template <class T, class U> 
T CubicInterp (const T& P0, const T& T0,
				const T& P1, const T& T1,
				const U& A)
{
	const float A2 = A*A;
	const float A3 = A2*A;

	return (T)
		(((2*A3)-(3*A2)+1)*P0) + ((A3-(2*A2)+A)*T0) + ((A3-A2)*T1) + (((-2*A3)+(3*A2))*P1);	
}

// 关键点的模式
enum InterpCurveMode
{
	CIM_Linear,
	CIM_Curve,
	CIM_Constant
};

// 关键点
template <class T>
class FInterpCurvePoint
{
public:
	float			fInVal;
	T				TOutVal;
	T				TArriveTangent;
	T				TLeaveTangent;
	InterpCurveMode	InterpMode;

	static bool CompareKeyPoint (FInterpCurvePoint* pFirst, FInterpCurvePoint* pSecond)
	{
		return pFirst->fInVal < pSecond->fInVal;
	}
};

// 曲线
template <class T>
class FInterpCurve
{
public:
	FInterpCurve ()
	{
	}

	// 不要在这里加入delete m_vecPoints[i];的处理，由外部系统维护
	~FInterpCurve ()
	{
		//for (int i=0; i<(int)m_vecPoints.size(); i++)
		//{
		//	delete m_vecPoints[i];
		//}

		m_vecPoints.clear();
	}

	void InsertPoint (FInterpCurvePoint<T> *pPoint)
	{
		m_vecPoints.push_back(pPoint);
		std::sort(m_vecPoints.begin(),m_vecPoints.end(),FInterpCurvePoint<T>::CompareKeyPoint);
	}

	void RemovePoint (FInterpCurvePoint<T> *pPoint)
	{
		vector<FInterpCurvePoint<T>*>::iterator it = m_vecPoints.begin();
		vector<FInterpCurvePoint<T>*>::iterator it_end = m_vecPoints.end();

		for (it; it!=it_end; it++)
		{
			if ((*it) == pPoint)
			{
				m_vecPoints.erase(it);
				return;
			}
		}

		std::sort(m_vecPoints.begin(),m_vecPoints.end(),FInterpCurvePoint<T>::CompareKeyPoint);
	}

	int GetNumPoints ()
	{
		return (int)m_vecPoints.size();
	}

	float GetMinInValue ()
	{
		Update();

		if (m_vecPoints.size()==0)
			return 0.0f;

		return m_vecPoints[0]->fInVal;
	}

	float GetMaxInValue ()
	{
		Update();

		if (m_vecPoints.size()==0)
			return 0.0f;

		return m_vecPoints[(int)m_vecPoints.size()-1]->fInVal;
	}

	std::vector<FInterpCurvePoint<T>*>& GetPoints ()
	{
		return m_vecPoints;
	}

	void Update ()
	{
		std::sort(m_vecPoints.begin(),m_vecPoints.end(),FInterpCurvePoint<T>::CompareKeyPoint);
	}

	void Reset ()
	{
		m_vecPoints.clear();
	}

	T Evaluate (const float fInVal, const T& TDefault, int *pIndex) const
	{
		const int iNumPoints = (int)m_vecPoints.size();

		// 如果没有点，返回我们传入的缺省值
		if (iNumPoints == 0)
		{
			if (pIndex)
			{
				*pIndex = -1;
			}
			
			return TDefault;
		}

		// 如果只有一个点，返回第一个点的值
		if (iNumPoints < 2 || (fInVal <= m_vecPoints[0]->fInVal))
		{
			if (pIndex)
			{
				*pIndex = 0;
			}

			return m_vecPoints[0]->TOutVal;
		}

		// 如果在曲线最后一个点之后，返回最后一个点的值
		float fEndInVal = (*(m_vecPoints.end()-1))->fInVal;
		if (fInVal >= fEndInVal)
		{
			if (pIndex)
			{
				*pIndex = iNumPoints - 1;
			}

			return (*(m_vecPoints.end()-1))->TOutVal;
		}

		for (int i=0; i<iNumPoints; i++)
		{
			if (fInVal < m_vecPoints[i]->fInVal)
			{
				const float fDiff = m_vecPoints[i]->fInVal - m_vecPoints[i-1]->fInVal;

				if (fDiff>0.f && m_vecPoints[i-1]->InterpMode != CIM_Constant)
				{
					const float fAlpha = (fInVal - m_vecPoints[i-1]->fInVal) / fDiff;

					if (pIndex)
					{
						*pIndex = i-1;
					}

					if (m_vecPoints[i-1]->InterpMode == CIM_Linear)
					{
						return Lerp(m_vecPoints[i-1]->TOutVal, m_vecPoints[i]->TOutVal, fAlpha);
					}
					else
					{
						return CubicInterp(
							m_vecPoints[i-1]->TOutVal,
							m_vecPoints[i-1]->TLeaveTangent*fDiff,
							m_vecPoints[i]->TOutVal,
							m_vecPoints[i]->TArriveTangent*fDiff,
							fAlpha);
					}
				}
				else // CIM_Constant
				{
					if (pIndex)
					{
						*pIndex = i - 1;
					}

					return m_vecPoints[i-1]->TOutVal;
				}
			}
		}

		// 不会运行到这里！
		if (pIndex)
		{
			*pIndex = iNumPoints - 1;
		}

		return m_vecPoints[iNumPoints-1]->TOutVal;
	}

protected:
	std::vector<FInterpCurvePoint<T>*> m_vecPoints;
};

#endif