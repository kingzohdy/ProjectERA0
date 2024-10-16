
#pragma once
#include "TimeBar.h"

#define KFWN_POS_CHANGE			1
#define KFWN_ADD_KEYFRAME		2
#define KFWN_DELETE_KEYFRAME	3

class CKeyFrameObject
{
public:
	virtual void CopyKeyFrame(int from, int to) = 0;
	virtual int AddKeyFrame(unsigned int tick) = 0;
	virtual void DeleteKeyFrame(int index) = 0;
	virtual void ActiveKeyFrame(int index) = 0;
	virtual void SetKeyFramePos(int index, unsigned int tick) = 0;
	virtual int GetKeyFrameCount() = 0;
	virtual unsigned int GetKeyFrameTick(int index) = 0;
	virtual CString GetName() = 0;
};

class CKeyFrameWnd : public CBCGPDockingControlBar
{
	DECLARE_DYNAMIC(CKeyFrameWnd)

public:
	CKeyFrameWnd();
	virtual ~CKeyFrameWnd();


	CTimeAixes	m_wndTimeAixes;
	CKeyFrameObject* m_pKeyFrameObj;

	CTimeAixes* GetTimeAixesCtrl() { return &m_wndTimeAixes; };

	void SetKeyFrameObj(CKeyFrameObject* pObj);
	CKeyFrameObject* GetKeyFrameObj() { return m_pKeyFrameObj; };
	void UpdateKeyFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(unsigned int nType, int cx, int cy);
	afx_msg void OnPosChange(NMHDR* pHdr, LRESULT* pResult);
	afx_msg void OnLayerActive(NMHDR* pHdr, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


