// WorldEditorView.h : CWorldEditorView ��Ľӿ�
//


#pragma once

class CWorldEditorView : public CWnd
{
public: // �������л�����
	CWorldEditorView();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CWorldEditorView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

