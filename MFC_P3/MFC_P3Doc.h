
// MFC_P3Doc.h : CMFC_P3Doc ��Ľӿ�
//


#pragma once

#define RECTANGLE 1
#define CIRCLE 2
#define TRIANGLE 3

// �Զ�����ļ��洢��
class CMFC_P3EntityAtom : public CObject
{
public:
	CMFC_P3EntityAtom();
protected:
	DECLARE_SERIAL(CMFC_P3EntityAtom);
public:
	CArray<CPoint, CPoint> m_pointArray;
	int m_nType;
	virtual void Serialize(CArchive &ar);
	void SetData(CPoint startPoint, CPoint endPoint, int nType);
	CPoint GetStartPoint();
	CPoint GetEndPoint();
	int GetType();
};

class CMFC_P3Entity : public CObject
{
public:
	CMFC_P3Entity();
protected:
	DECLARE_SERIAL(CMFC_P3Entity);
public:
	//CArray<CMFC_P3EntityAtom, CMFC_P3EntityAtom> m_entityArray;
	CTypedPtrList<CObList, CMFC_P3EntityAtom*> m_entityArray;
	BOOL DrawPointShape(CDC *pDC);
	void AddEntityAtom(CMFC_P3EntityAtom &atom);
	CMFC_P3EntityAtom* NewEntityAtom();
	//BOOL DrawPointCircle(CDC *pDC);
	//BOOL DrawPointTriangle(CDC *pDC);
	virtual void Serialize(CArchive &ar);
};

//MFC Auto Doc
class CMFC_P3Doc : public CDocument
{
protected: // �������л�����
	CMFC_P3Doc();
	DECLARE_DYNCREATE(CMFC_P3Doc)

// ����
protected:
	CPen m_pen;
public:
	CTypedPtrList<CObList, CMFC_P3Entity*> m_entity;
	CPen* GetCurrentPen() {return &m_pen;}
	int nType;

// ����
public:
	CMFC_P3Entity* NewEntity();

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// ʵ��
public:
	virtual ~CMFC_P3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
