#ifndef __CURVE__
#define __CURVE__

#include "OgreMath.h"
#include <algorithm>

namespace Ogre
{
	// 关键点的模式
	enum InterpCurveMode
	{
		CIM_Linear,
		CIM_Curve,
		CIM_Constant
	};

	// 关键点
	template <class T>
	class InterpCurvePoint
	{
	public:
		float			fInVal;
		T				TOutVal;
		T				TArriveTangent;
		T				TLeaveTangent;
		InterpCurveMode	InterpMode;

		static bool CompareKeyPoint (InterpCurvePoint* pFirst, InterpCurvePoint* pSecond)
		{
			return pFirst->fInVal < pSecond->fInVal;
		}
	};


	// 曲线
	template <class T>
	class InterpCurve
	{
	public:
		InterpCurve ()
		{
		}

		// 不要在这里加入delete m_vecPoints[i]的处理，由外部系统维护
		~InterpCurve ()
		{
			//for (int i=0; i<(int)m_vecPoints.size(); i++)
			//{
			//	delete m_vecPoints[i];
			//}

			m_vecPoints.clear();
		}

		void InsertPoint (InterpCurvePoint<T> *pPoint)
		{
			m_vecPoints.push_back(pPoint);
			std::sort(m_vecPoints.begin(),m_vecPoints.end(),InterpCurvePoint<T>::CompareKeyPoint);
		}

		void RemovePoint (InterpCurvePoint<T> *pPoint)
		{
			vector<InterpCurvePoint<T>*>::iterator it = m_vecPoints.begin();
			vector<InterpCurvePoint<T>*>::iterator it_end = m_vecPoints.end();

			for (it; it!=it_end; it++)
			{
				if ((*it) == pPoint)
				{
					m_vecPoints.erase(it);
					return;
				}
			}

			std::sort(m_vecPoints.begin(),m_vecPoints.end(),InterpCurvePoint<T>::CompareKeyPoint);
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

			return m_vecPoints.back()->fInVal;
		}

		std::vector<InterpCurvePoint<T>*>& GetPoints ()
		{
			return m_vecPoints;
		}

		void Update ()
		{
			std::sort(m_vecPoints.begin(),m_vecPoints.end(),InterpCurvePoint<T>::CompareKeyPoint);
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
		std::vector<InterpCurvePoint<T>*> m_vecPoints;
	};
}

#endif