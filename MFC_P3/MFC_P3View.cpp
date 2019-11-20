
// MFC_P3View.cpp : CMFC_P3View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFC_P3.h"
#endif

#include "MFC_P3Doc.h"
#include "MFC_P3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFC_P3View

IMPLEMENT_DYNCREATE(CMFC_P3View, CView)

BEGIN_MESSAGE_MAP(CMFC_P3View, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFC_P3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CMFC_P3View ����/����

CMFC_P3View::CMFC_P3View()
{
	// TODO: �ڴ˴���ӹ������
	//m_memDC.CreateCompatibleDC(NULL); 
}

CMFC_P3View::~CMFC_P3View()
{
}

BOOL CMFC_P3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMFC_P3View ����

void CMFC_P3View::OnDraw(CDC* pDC)
{
	CMFC_P3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//CRect rect;
	//GetClientRect(&rect);
	//m_nWidth = rect.Width();
	//m_nHeight = rect.Height();
	//m_memDC.FillSolidRect(0,0,m_nWidth,m_nHeight,RGB(255,255,255)); 

	CTypedPtrList<CObList, CMFC_P3Entity*>& entityList = pDoc->m_entity;
	POSITION pos = entityList.GetHeadPosition();
	while(pos != NULL)
	{
		CMFC_P3Entity *pEntity = entityList.GetNext(pos);
		pEntity->DrawPointShape(pDC);
	}
}


// CMFC_P3View ��ӡ


void CMFC_P3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFC_P3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFC_P3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFC_P3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMFC_P3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFC_P3View::OnLButtonUp(UINT /* nFlags */, CPoint point)
{
	if(GetCapture() != this)
	{
		return;
	}

	CPoint use[3] = {};
	m_endPoint = point;

	CMFC_P3Doc *pDoc = GetDocument();

	CClientDC dc(this);
	CPen *pOldPen = dc.SelectObject(pDoc->GetCurrentPen());
	switch(m_nType)
	{
	case RECTANGLE:
		dc.Rectangle(CRect(m_startPoint, m_endPoint));
		break;
	case CIRCLE:
		dc.Ellipse(CRect(m_startPoint, m_endPoint));
		break;
	case TRIANGLE:
		use[0].SetPoint(m_startPoint.x, m_endPoint.y);
		use[1] = m_endPoint;
		use[2].SetPoint((m_endPoint.x + m_startPoint.x) / 2, 
						  m_startPoint.y);
		dc.Polygon(use, 3);
		break;
	default:
		break;
	}

	/*m_memBitmap.CreateCompatibleBitmap(GetDC(),m_nWidth,m_nHeight); 
	CBitmap *pOldBit=m_memDC.SelectObject(&m_memBitmap); 

	switch(m_nType)
	{
	case RECTANGLE:
		m_memDC.Rectangle(CRect(m_startPoint, m_endPoint));
		break;
	case CIRCLE:
		m_memDC.Ellipse(CRect(m_startPoint, m_endPoint));
		break;
	case TRIANGLE:
		use[0].SetPoint(m_startPoint.x, m_endPoint.y);
		use[1] = m_endPoint;
		use[2].SetPoint((m_endPoint.x + m_startPoint.x) / 2, 
						  m_startPoint.y);
		m_memDC.Polygon(use, 3);
		break;
	default:
		break;
	}
	GetDC()->BitBlt(0,0,m_nWidth,m_nHeight,&m_memDC,0,0,SRCCOPY);
	m_memBitmap.DeleteObject(); */

	CMFC_P3EntityAtom *tmp = m_pEntity->NewEntityAtom();
	tmp->SetData(m_startPoint, m_endPoint, m_nType);
	m_pEntity->m_entityArray.AddTail(tmp);

	ReleaseCapture();
	return;
}

void CMFC_P3View::OnLButtonDown(UINT /* nFlags */, CPoint point)
{
	if(m_nType != RECTANGLE && m_nType != CIRCLE && m_nType != TRIANGLE)
	{
		return;
	}

	CMFC_P3Doc *pDoc = GetDocument();
	m_pEntity = pDoc->NewEntity();
	
	SetCapture();
	m_startPoint = point;
	return;
}

void CMFC_P3View::OnMouseMove(UINT /* nFlags */, CPoint point)
{
	if(GetCapture() != this)
	{
		return;
	}

	//Invalidate();

	/*CPoint use[3] = {};
	m_endPoint = point;

	CMFC_P3Doc *pDoc = GetDocument();

	CClientDC dc(this);
	CPen *pOldPen = dc.SelectObject(pDoc->GetCurrentPen());
	switch(m_nType)
	{
	case RECTANGLE:
		dc.Rectangle(CRect(m_startPoint, m_endPoint));
		break;
	case CIRCLE:
		dc.Ellipse(CRect(m_startPoint, m_endPoint));
		break;
	case TRIANGLE:
		use[0].SetPoint(m_startPoint.x, m_endPoint.y);
		use[1] = m_endPoint;
		use[2].SetPoint((m_endPoint.x + m_startPoint.x) / 2, 
						  m_startPoint.y);
		dc.Polygon(use, 3);
		break;
	default:
		break;
	}*/

	/*m_memBitmap.CreateCompatibleBitmap(GetDC(),m_nWidth,m_nHeight); 
	CBitmap *pOldBit=m_memDC.SelectObject(&m_memBitmap); 

	switch(m_nType)
	{
	case RECTANGLE:
		m_memDC.Rectangle(CRect(m_startPoint, m_endPoint));
		break;
	case CIRCLE:
		m_memDC.Ellipse(CRect(m_startPoint, m_endPoint));
		break;
	case TRIANGLE:
		use[0].SetPoint(m_startPoint.x, m_endPoint.y);
		use[1] = m_endPoint;
		use[2].SetPoint((m_endPoint.x + m_startPoint.x) / 2, 
						  m_startPoint.y);
		m_memDC.Polygon(use, 3);
		break;
	default:
		break;
	}
	GetDC()->BitBlt(0,0,m_nWidth,m_nHeight,&m_memDC,0,0,SRCCOPY);
	m_memBitmap.DeleteObject(); */

	return;
}


void CMFC_P3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFC_P3View ���

#ifdef _DEBUG
void CMFC_P3View::AssertValid() const
{
	CView::AssertValid();
}

void CMFC_P3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFC_P3Doc* CMFC_P3View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFC_P3Doc)));
	return (CMFC_P3Doc*)m_pDocument;
}
#endif //_DEBUG


// CMFC_P3View ��Ϣ�������