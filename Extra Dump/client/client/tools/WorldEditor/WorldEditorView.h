// WorldEditorView.h : CWorldEditorView 类的接口
//


#pragma once

class CWorldEditorView : public CWnd
{
public: // 仅从序列化创建
	CWorldEditorView();

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CWorldEditorView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

