// modelshowDoc.h : CmodelshowDoc ��Ľӿ�
//


#pragma once


class CmodelshowDoc : public CDocument
{
protected: // �������л�����
	CmodelshowDoc();
	DECLARE_DYNCREATE(CmodelshowDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CmodelshowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


