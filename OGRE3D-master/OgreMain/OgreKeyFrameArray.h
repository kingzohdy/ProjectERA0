
#ifndef __KeyFrameArray_H__
#define __KeyFrameArray_H__

#include "OgrePrerequisites.h"
#include "OgreQuaternion.h"
#include "OgreVector3.h"
#include "OgreArchive.h"
#include "OgreBaseObject.h"
#include <vector>

namespace Ogre
{
	enum InterpolType
	{
		INTERPOL_NONE = 0,
		INTERPOL_LINEAR,
		INTERPOL_HERMITE,
		INTERPOL_RANDOM
	};

	template<class T>
	inline void KEYFRAME_LERP(T &dest, float t, const T &src1, const T &src2)
	{
		dest = Lerp(src1, src2, t);
	}

	inline void KEYFRAME_LERP( Quaternion &dest, float t, const Quaternion &src1, const Quaternion &src2)
	{
		dest.slerp( src1, src2, t );
	}

	template<class T>
	inline void KEYFRAME_HERMITE(T &dest, const float r, const T &v1, const T &v2, const T &in, const T &out)
	{
		// basis functions
		float h1 = 2.0f*r*r*r - 3.0f*r*r + 1.0f;
		float h2 = -2.0f*r*r*r + 3.0f*r*r;
		float h3 = r*r*r - 2.0f*r*r + r;
		float h4 = r*r*r - r*r;

		// interpolation
		dest = v1*h1 + v2*h2 + in*h3 + out*h4;
	}

	class BaseKeyFrameArray : public BaseObject
	{
	public:
		struct AnimRange
		{
			int begin;
			int end;  //[begin, end]
		};

	public:
		virtual ~BaseKeyFrameArray()
		{
		}

		bool hasSeq(int seq)
		{
			//for old version data
			if(m_Ranges.size() == 0)
			{
				return true;
			}
			//end

			assert(seq>=0 && seq<int(m_Ranges.size()));
			if(m_Ranges[seq].begin > m_Ranges[seq].end) return false;
			return true;
		}

		virtual void _serialize(Archive &ar, int version)
		{
			ar.serializeRawArray(m_Ranges);
		}

		virtual size_t getNumKey() = 0;
		virtual void getValue(int seq, uint tick, void *pdata) = 0;

	public:
		std::vector<AnimRange>m_Ranges;
	};

	template<typename POSTYPE>
	class KeyFrameArray : public BaseKeyFrameArray
	{
	public:
		KeyFrameArray() : m_Type(INTERPOL_LINEAR){}
		KeyFrameArray( size_t n ) : m_Type(INTERPOL_LINEAR), m_Array(n){}

		virtual size_t getNumKey()
		{
			return m_Array.size();
		}

		virtual void getValue(int seq, uint tick, void *pdata)
		{
			return getValue(seq, tick, *(POSTYPE *)pdata);
		}

		void getValue(int seq, uint tick, POSTYPE &data, bool loop=true)
		{
			int size = (int)m_Array.size();

			//assert(size > 0);

			if (size == 0)
				return;
			
			data = m_Array[0].data;

			int start, end;
			int ntotalkeys = (int)m_Array.size();
			int nseq = (int)m_Ranges.size();

			if(nseq == 0)
			{
				start = 0;
				end = ntotalkeys-1;
			}
			else
			{
				assert(seq>=0 && seq<nseq);
				start = m_Ranges[seq].begin;
				end = m_Ranges[seq].end;
				assert(start <= end);
			}
			

			if(m_Type == INTERPOL_RANDOM)
			{
				int i = start;
				if(end > start) i += rand()%(end-start);

				data = m_Array[i].data;
				return;
			}
			if(start >= size)
			{
				start = size -1;
			}
			if(end >= size)
			{
				end = size -1;
			}
			tick -= m_Array[start].tick;
			uint dtick = (m_Array[end].tick - m_Array[start].tick + 1);
			if(dtick == 0) tick = m_Array[start].tick;
			else
			{	
				if(loop) tick = m_Array[start].tick + (tick%dtick);
				else if(dtick < tick) tick = m_Array[end].tick;
				else tick = m_Array[start].tick + tick;
			}

			if ( m_nPrindex >= start && m_nPrindex  < end )  
			{
				if ( tick > m_Array[m_nPrindex + 1].tick && m_nPrindex + 2 <= end)
				{
					m_nPrindex++;
				}
				if ( tick >= m_Array[m_nPrindex].tick && tick <= m_Array[m_nPrindex + 1].tick )
				{
					uint tick1 = m_Array[m_nPrindex].tick;
					uint tick2 = m_Array[m_nPrindex+1].tick;
					if( tick1 == tick2 )
					{
						return;
					}
					assert( tick1 < tick2);

					float t = float( int(tick-tick1) / double(tick2-tick1) );
					if(m_Type == INTERPOL_LINEAR) KEYFRAME_LERP( data, t, m_Array[m_nPrindex].data, m_Array[m_nPrindex + 1].data);
					else KEYFRAME_HERMITE(data, t, m_Array[m_nPrindex].data, m_Array[m_nPrindex + 1].data, m_CtrlPt[m_nPrindex].out, m_CtrlPt[m_nPrindex + 1].in);
					return;
				}
				//while ( m_nPrindex < end)
				//{
				//	if ( tick >= m_Array[m_nPrindex].tick && tick <= m_Array[m_nPrindex + 1].tick )
				//	{
				//		uint tick1 = m_Array[m_nPrindex].tick;
				//		uint tick2 = m_Array[m_nPrindex+1].tick;
				//		if( tick1 == tick2 )
				//		{
				//			return;
				//		}
				//		assert( tick1 < tick2);

				//		float t = float( int(tick-tick1) / double(tick2-tick1) );
				//		if(m_Type == INTERPOL_LINEAR) KEYFRAME_LERP( data, t, m_Array[m_nPrindex].data, m_Array[m_nPrindex + 1].data);
				//		else KEYFRAME_HERMITE(data, t, m_Array[m_nPrindex].data, m_Array[m_nPrindex + 1].data, m_CtrlPt[m_nPrindex].out, m_CtrlPt[m_nPrindex + 1].in);
				//		return;
				//	}
				//	m_nPrindex++;
				//}
			}
			//char po[100];
			//sprintf(po,"start %d end %d\n",start,end);
			//OutputDebugString(po);

			for(;;)
			{
				if( start >= end-1 ) break;

				int middle = (start+end)/2;
				if( tick < m_Array[middle].tick ) end = middle;
				else start = middle;
			}

			if( start == end ) data = m_Array[start].data;
			else
			{
				uint tick1 = m_Array[start].tick;
				uint tick2 = m_Array[end].tick;
				if( tick1 == tick2 )
				{
					return;
				}
				assert( tick1 < tick2);

				float t = float( int(tick-tick1) / double(tick2-tick1) );
				if(m_Type == INTERPOL_LINEAR) KEYFRAME_LERP( data, t, m_Array[start].data, m_Array[end].data);
				else KEYFRAME_HERMITE(data, t, m_Array[start].data, m_Array[end].data, m_CtrlPt[start].out, m_CtrlPt[end].in);
				m_nPrindex = start;
			}
		}

		virtual void _serialize(Archive &ar, int version)
		{
			ar.serializeRawType(m_Type);
			ar.serializeRawArray(m_Array);
			ar.serializeRawArray(m_CtrlPt);

			if(!m_Array.empty() && int(m_Array[0].tick)<0)
			{
				m_Array[0].tick = 0;
			}

			BaseKeyFrameArray::_serialize(ar, version);
			m_nPrindex	 = -1;
		}

		const KeyFrameArray<POSTYPE>& operator=(const KeyFrameArray<POSTYPE>&rhs)
		{
			m_Type = rhs.m_Type;
			m_Array = rhs.m_Array;
			m_Ranges = rhs.m_Ranges;
			m_CtrlPt = rhs.m_CtrlPt;

			return *this;
		}

		void setNumKey(size_t num)
		{
			m_Array.resize(num);
		}

		void setKeyFrame(size_t i, uint tick, POSTYPE data)
		{
			assert(i < m_Array.size());
			m_Array[i].tick = tick;
			m_Array[i].data = data;
		}

		void getKeyFrame(size_t i, uint &tick, POSTYPE &data)
		{
			assert(i < m_Array.size());
			tick = m_Array[i].tick;
			data = m_Array[i].data;
		}

	public:
#pragma pack(push,1)
		struct KEYFRAME_T
		{
			uint tick;
			POSTYPE data;
		};
		struct CONTROL_POINT_T
		{
			POSTYPE in;
			POSTYPE out;
		};
#pragma pack(pop)

		InterpolType m_Type;
		std::vector<KEYFRAME_T>m_Array;
		std::vector<CONTROL_POINT_T>m_CtrlPt;
		int		m_nPrindex;
	};

	template<typename POSTYPE>
	Archive &operator<<(Archive &ar, KeyFrameArray<POSTYPE>&frames)
	{
		frames._serialize(ar, 100);
		return ar;
	}

	template<class T>
	void SegmentReduction(const T *pos, const uint *pkey, size_t begin, size_t end, char *mk, float tolerance)
	{
		T val;
		float max_dist = 0;
		size_t isel;

		for(size_t i=begin+1; i<end; i++)
		{
			float t = float(int(pkey[i]-pkey[begin])/double(pkey[end]-pkey[begin]));
			KEYFRAME_LERP(val, t, pos[begin], pos[end]);

			float dist = DistanceSqr(pos[i], val);
			if(max_dist < dist)
			{
				isel = i;
				max_dist = dist;
			}
		}

		if(max_dist > tolerance*tolerance)
		{
			mk[isel] = 1;
			SegmentReduction(pos, pkey, begin, isel, mk, tolerance);
			SegmentReduction(pos, pkey, isel, end, mk, tolerance);
		}
	}

	/*
	Vector:  tolerance = min_dist
	Quaternion: tolerance = sin(theta/2)
	*/
	template<class T>
	size_t SampleReduction(size_t nsample, const T *pos, const uint *pkey, T *newpos, uint *newkeys, float tolerance)
	{
		char *mk = new char[nsample];
		memset(mk, 0, nsample);

		mk[0] = mk[nsample-1] = 1;
		SegmentReduction(pos, pkey, 0, nsample-1, mk, tolerance);

		size_t num = 0;
		for(size_t i=0; i<nsample; i++)
		{
			if(mk[i])
			{
				newpos[num] = pos[i];
				newkeys[num] = pkey[i];
				num++;
			}
		}

		delete[] mk;
		return num;
	}
}

#endif
