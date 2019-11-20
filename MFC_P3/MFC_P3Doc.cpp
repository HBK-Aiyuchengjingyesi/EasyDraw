
// MFC_P3Doc.cpp : CMFC_P3Doc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFC_P3.h"
#endif

#include "MFC_P3Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFC_P3Doc

IMPLEMENT_DYNCREATE(CMFC_P3Doc, CDocument)

BEGIN_MESSAGE_MAP(CMFC_P3Doc, CDocument)
END_MESSAGE_MAP()


// CMFC_P3Doc ����/����

CMFC_P3Doc::CMFC_P3Doc()
{
	// TODO: �ڴ����һ���Թ������

}

CMFC_P3Doc::~CMFC_P3Doc()
{
}

BOOL CMFC_P3Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	m_pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	return TRUE;
}




// CMFC_P3Doc ���л�

void CMFC_P3Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
	m_entity.Serialize(ar);
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CMFC_P3Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CMFC_P3Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CMFC_P3Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFC_P3Doc ���

#ifdef _DEBUG
void CMFC_P3Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFC_P3Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CMFC_P3Doc ����
BOOL CMFC_P3Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if(!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}

	return TRUE;
}

void CMFC_P3Doc::DeleteContents()
{
	while(!m_entity.IsEmpty())
	{
		delete m_entity.RemoveHead();
	}
	CDocument::DeleteContents();
}

CMFC_P3Entity* CMFC_P3Doc::NewEntity()
{
	CMFC_P3Entity *pEntityItem = new CMFC_P3Entity();
	m_entity.AddTail(pEntityItem);
	SetModifiedFlag();
	return pEntityItem;
}

//CMFC_P3EntityAtom
IMPLEMENT_SERIAL(CMFC_P3EntityAtom, CObject, 1)
CMFC_P3EntityAtom::CMFC_P3EntityAtom()
{
	;
}

void CMFC_P3EntityAtom::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		ar << (WORD)m_nType;
		m_pointArray.Serialize(ar);
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		WORD w;
		ar >> w;
		m_nType = w;
		m_pointArray.Serialize(ar);
	}
}

//CMFC_P3EntityAtom::CMFC_P3EntityAtom(CPoint startPoint, CPoint endPoint, int nType)
void CMFC_P3EntityAtom::SetData(CPoint startPoint, CPoint endPoint, int nType)
{
	m_pointArray.Add(startPoint);
	m_pointArray.Add(endPoint);
	m_nType = nType;
}

int CMFC_P3EntityAtom::GetType()
{
	return m_nType;
}

CPoint CMFC_P3EntityAtom::GetStartPoint()
{
	return m_pointArray[0];
}

CPoint CMFC_P3EntityAtom::GetEndPoint()
{
	return m_pointArray[1];
}

//CMFC_P3Entity
IMPLEMENT_SERIAL(CMFC_P3Entity, CObject, 1)
CMFC_P3Entity::CMFC_P3Entity()
{
	;
}

void CMFC_P3Entity::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
		m_entityArray.Serialize(ar);
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		m_entityArray.Serialize(ar);
	}
}

BOOL CMFC_P3Entity::DrawPointShape(CDC *pDC)
{
	CPoint ld;
	CPoint rt;
	CPoint mid;
	CPen penCircle;
	int nType;
	if(!penCircle.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)))
	{
		return FALSE;
	}
	CPen* pOldPen = pDC->SelectObject(&penCircle);

	POSITION pos = m_entityArray.GetHeadPosition();
	while(pos != NULL)
	{
		CMFC_P3EntityAtom *pTmp = m_entityArray.GetNext(pos);
		nType = pTmp->GetType();
		switch(nType)
		{
		case RECTANGLE:
			ld = pTmp->GetStartPoint();
			rt = pTmp->GetEndPoint();
			pDC->Rectangle(CRect(ld, rt));
			break;
		case CIRCLE:
			ld = pTmp->GetStartPoint();
			rt = pTmp->GetEndPoint();
			pDC->Ellipse(CRect(ld, rt));
			break;
		case TRIANGLE:
			ld.SetPoint(pTmp->GetStartPoint().x, pTmp->GetEndPoint().y); 
			rt = pTmp->GetEndPoint();
			mid.SetPoint((pTmp->GetEndPoint().x + pTmp->GetStartPoint().x) / 2, 
						  pTmp->GetStartPoint().y); 
			CPoint use[3] = {ld, mid, rt};
			pDC->Polygon(use, 3);
			break;
		}
	}

	//for(int i = 0; i < m_entityArray.GetCount(); ++i)
	//{
	//	nType = m_entityArray[i].GetType();
	//	switch(nType)
	//	{
	//	case RECTANGLE:
	//		ld = m_entityArray[i].GetStartPoint();
	//		rt = m_entityArray[i].GetEndPoint();
	//		pDC->Rectangle(CRect(ld, rt));
	//		break;
	//	case CIRCLE:
	//		ld = m_entityArray[i].GetStartPoint();
	//		rt = m_entityArray[i].GetEndPoint();
	//		pDC->Ellipse(CRect(ld, rt));
	//		break;
	//	case TRIANGLE:
	//		ld.SetPoint(m_entityArray[i].GetStartPoint().x, m_entityArray[i].GetEndPoint().y); 
	//		rt = m_entityArray[i].GetEndPoint();
	//		mid.SetPoint((m_entityArray[i].GetEndPoint().x + m_entityArray[i].GetStartPoint().x) / 2, 
	//					  m_entityArray[i].GetStartPoint().y); 
	//		CPoint use[3] = {ld, mid, rt};
	//		pDC->Polygon(use, 3);
	//		break;
	//	}
	//}
	pDC->SelectObject(pOldPen);
	return TRUE;
}

CMFC_P3EntityAtom* CMFC_P3Entity::NewEntityAtom()
{
	CMFC_P3EntityAtom *pEntityAtomItem = new CMFC_P3EntityAtom();
	m_entityArray.AddTail(pEntityAtomItem);
	return pEntityAtomItem;
}

void CMFC_P3Entity::AddEntityAtom(CMFC_P3EntityAtom &atom)
{
	;
}

//BOOL CMFC_P3Entity::DrawPointCircle(CDC *pDC)
//{
//	CPoint ld;
//	CPoint rt;
//	CPen penCircle;
//	if(!penCircle.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)))
//	{
//		return FALSE;
//	}
//	CPen* pOldPen = pDC->SelectObject(&penCircle);
//	for(int i = 0; i < m_entityArray.GetSize(); ++i)
//	{
//		ld = m_entityArray[i].GetStartPoint();
//		rt = m_entityArray[i].GetEndPoint();
//		pDC->Ellipse(CRect(ld, rt));
//	}
//	pDC->SelectObject(pOldPen);
//	return TRUE;
//}

//BOOL CMFC_P3Entity::DrawPointTriangle(CDC *pDC)
//{
//	CPoint ld;
//	CPoint rt;
//	CPoint mid;
//	CPen penCircle;
//	if(!penCircle.CreatePen(PS_SOLID, 2, RGB(0, 0, 0)))
//	{
//		return FALSE;
//	}
//	CPen* pOldPen = pDC->SelectObject(&penCircle);
//	for(int i = 0; i < m_entityArray.GetSize(); ++i)
//	{
//		ld.SetPoint(m_entityArray[i].GetStartPoint().x, m_entityArray[i].GetEndPoint().y); 
//		rt = m_entityArray[i].GetEndPoint();
//		mid.SetPoint((m_entityArray[i].GetEndPoint().x + m_entityArray[i].GetStartPoint().x) / 2, 
//					  m_entityArray[i].GetStartPoint().y); 
//		CPoint use[3] = {ld, mid, rt};
//		pDC->Polygon(use, 3);
//	}
//	pDC->SelectObject(pOldPen);
//	return TRUE;
//}