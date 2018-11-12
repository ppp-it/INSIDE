// testView.cpp : implementation of the CtestView class
//

#include "stdafx.h"
#include "test.h"

#include "testDoc.h"
#include "testView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CDDS_SUBITEMPREPAINT (CDDS_ITEM | CDDS_SUBITEM | CDDS_PREPAINT)


// CtestView

IMPLEMENT_DYNCREATE(CtestView, CListView)

BEGIN_MESSAGE_MAP(CtestView, CListView)
	ON_WM_RBUTTONUP()
	//ON_COMMAND(ID_CLIENT_ENABLE, &CFileExchangeCore::OnClientEnable)
	ON_NOTIFY_RANGE(LVN_COLUMNCLICK,0,0xffff, &CtestView::OnColumnClick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CtestView::OnCustomDraw)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CtestView construction/destruction

CtestView::CtestView()
{
	// TODO: add construction code here

}

CtestView::~CtestView()
{
}

BOOL CtestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));
    lf.lfHeight = 13;
    lf.lfWeight = FW_BOLD;
	#pragma warning (push)
	#pragma warning(disable : 4996)
    _tcsncpy(lf.lfFaceName, _T("Tahoma"), 7);
	#pragma warning (pop)
    VERIFY(m_FontBold.CreateFontIndirect(&lf)); 

	cs.style |= ( LVS_REPORT | LVS_OWNERDATA);
	//cs.dwExStyle |= ( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	return CListView::PreCreateWindow(cs);
}

// CtestView drawing

void CtestView::OnDraw(CDC* /*pDC*/)
{
	CtestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CtestView printing

BOOL CtestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CtestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CtestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CtestView diagnostics

#ifdef _DEBUG
void CtestView::AssertValid() const
{
	CListView::AssertValid();
}

void CtestView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CtestDoc* CtestView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestDoc)));
	return (CtestDoc*)m_pDocument;
}
#endif //_DEBUG


// вызывается в первый раз после конструктора

void CtestView::OnInitialUpdate(void)
{
	CListView::OnInitialUpdate();
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	LV_COLUMN lvc;

	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVIF_IMAGE/* | LVS_EX_AUTOSIZECOLUMNS*/;

	lvc.iSubItem = 0;
	lvc.pszText = _T("");
	lvc.cx = 22;
	GetListCtrl().InsertColumn(lvc.iSubItem,&lvc);
	lvc.iSubItem = 1;
	lvc.pszText = _T("N");
	lvc.cx = 22;
	GetListCtrl().InsertColumn(lvc.iSubItem,&lvc);
	lvc.iSubItem = 2;
	lvc.pszText = _T("колонка 1");
	lvc.cx = 70;
	GetListCtrl().InsertColumn(lvc.iSubItem,&lvc);
	lvc.iSubItem = 3;
	lvc.pszText = _T("колонка 2");
	lvc.cx = 135;
	GetListCtrl().InsertColumn(lvc.iSubItem,&lvc);
	lvc.iSubItem = 4;
	lvc.pszText = _T("колонка 3");
	lvc.cx = 375;
	GetListCtrl().InsertColumn(lvc.iSubItem,&lvc);

	UpdateData(TRUE);
	UpdateRecord();

	ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT /*| LVS_EX_SUBITEMIMAGES*/);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
}

void CtestView::UpdateRecord(void)
{
	// set the item count to the new record count
	int nRecordCount = 3;
	GetListCtrl().SetItemCountEx( nRecordCount );

	// Invalidate the listview so it will redraw itself
	Invalidate();
}

//Обновление только строки с номером nRows
DWORD CtestView::UpdateRows(int nRows)
{
	BOOL fOk = FALSE;
	DWORD dwRetSend = 0;

	fOk = ListView_Update(m_hWnd, nRows);
	//fOk = ::SendMessageTimeout(m_hWnd, LVM_UPDATE, (WPARAM)(nRows), 0L, SMTO_NORMAL, 1, &dwRetSend);
	//fOk = ::PostMessage((m_hWnd), LVM_UPDATE, (WPARAM)(nRows), 0L);

  return fOk;
}

// заполняет грид содержимым
BOOL CtestView::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult)
{
#ifdef _DEBUG
	static int nCountUpdate=0;
#endif
	if(message == WM_NOTIFY)
	{
		NMHDR* phdr = (NMHDR*)lParam;


		// these 3 notifications are only sent by virtual listviews
		switch(phdr->code)
		{
		case LVN_GETDISPINFO:
			{
				NMLVDISPINFO* pLvdi;

				pLvdi = (NMLVDISPINFO*)lParam;
					if(pLvdi->item.mask & LVIF_TEXT)
					{
						LPLVITEM pItem = &(pLvdi->item);
						// заполняет грид по содержимым
						if ((long)pLvdi->item.iItem == 1){
							pItem->iImage = 1;

							switch(pItem->iSubItem)
							{
							case 2:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр1 к 2"), 8);
								break;
							case 3:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр1 к 3"), 8);
								break;
							case 4:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр1 к 4"), 8);
								break;
							case 1:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр1 к 1"), 8);
								break;
							}
						}else if ((long)pLvdi->item.iItem == 1){
							pItem->iImage = 1;

							switch(pItem->iSubItem)
							{
							case 2:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр2 к 2"), 8);
								break;
							case 3:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр2 к 3"), 8);
								break;
							case 4:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр2 к 4"), 8);
								break;
							case 1:
								Checked::tcsncpy_s(pItem->pszText, pItem->cchTextMax, _T("стр2 к 1"), 8);
								break;
							}
						}
					}
				//}
			}
			if(pLResult != NULL)
			{
				*pLResult = 0;
			}
			break;
		case LVN_ODCACHEHINT:
			{
				//TRACE1("OnChildNotify::LVN_ODCACHEHINT. rows %d\n", m_nRowsSel);
				m_nRowsSel = -1;
				NMLVCACHEHINT* pHint = (NMLVCACHEHINT*)lParam;

				PrepareCache(pHint->iFrom, pHint->iTo);
			}
			if(pLResult != NULL)
			{
				*pLResult = 0;
			}
			break;
		case LVN_ODFINDITEM:
			{
				//TRACE1("OnChildNotify::LVN_ODFINDITEM. rows %d\n", m_nRowsSel);
				NMLVFINDITEM* pFindItem = (NMLVFINDITEM*)lParam;
				int i = FindItem(pFindItem->iStart, &pFindItem->lvfi);
				if(pLResult != NULL)
				{
					*pLResult = i;
				}
			}
			break;
		default:
			{
				//TRACE1("OnChildNotify::default. rows %d\n", m_nRowsSel);
			return CListView::OnChildNotify(message, wParam, lParam, pLResult);
			}
		}
	}
	else
		return CListView::OnChildNotify(message, wParam, lParam, pLResult);

	return TRUE;
}
void CtestView::PrepareCache(int /*iFrom*/, int /*iTo*/)
{
}

int CtestView::FindItem(int /*iStart*/, LVFINDINFO* /*plvfi*/)
{
	return -1;
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void CtestView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	//AfxMessageBox(_T("CtestView::OnRButtonUp\n"));
	CMainFrame * pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	CtestView * pView = static_cast<CtestView*>(pFrame->GetActiveView());
}

void CtestView::OnColumnClick(UINT /*id*/, NMHDR* pNotifyStruct, LRESULT* /*pResult*/)
{
	// when a column is clicked on, sort by that column

	switch(reinterpret_cast<NMLISTVIEW*>(pNotifyStruct)->iSubItem)
	{
	case 0:
		//phmSetting->SetSort(_T("[Name]"));
		break;
	//case 1:
	//	phmSetting->SetSort(_T("[Name]"));
	//	break;
	//case 2:
	//	phmSetting->SetSort(_T("[Inp]"));
	//	break;
	//case 3:
	//	phmSetting->SetSort(_T("[Log]"));
	//	break;
	//case 4:
	//	phmSetting->SetSort(_T("[Scan]"));
	//	break;
	//case 5:
	//	phmSetting->SetSort(_T("[Repeet]"));
	//	break;
	//case 6:
	//	phmSetting->SetSort(_T("[Repeet]"));
	//	break;
	}
	Invalidate();
}

void CtestView::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	//3.)
	NMLVCUSTOMDRAW * pNMCD = ( NMLVCUSTOMDRAW* ) pNMHDR;
	*pResult = CDRF_DODEFAULT;

	switch ( pNMCD->nmcd.dwDrawStage )
	{

	case CDDS_PREPAINT:
		// при начале прорисовки листа указываем, что нам нужны нотификации на прорисовку каждой строки
		*pResult |= CDRF_NOTIFYITEMDRAW ;
		break;

	case CDDS_ITEMPREPAINT:
		// если нужно красить строки целиком, то можно остановиться здесь
		// установить цвета текста и фона и выставить *pResult |= CDRF_NEWFONT;, НО, нам
		// item-ов маловато, хотим прорисовывать разным цветом subitem-ы, поэтому
		
		if( pNMCD->nmcd.uItemState & CDIS_SELECTED )	{				
			m_nRowsSel = pNMCD->nmcd.dwItemSpec;
			//TRACE1("select rows %d\n", m_nRowsSel);
		}

		*pResult |= CDRF_NOTIFYSUBITEMDRAW;
		break;

	case CDDS_SUBITEMPREPAINT:
		// а вот и они

		bool bActivate = 0;

		switch ( pNMCD->iSubItem ) // каждую колоночку отдельно
		{

		case 1:
			pNMCD->clrTextBk = GetSysColor( COLOR_WINDOW ); 
			pNMCD->clrText = RGB(255, 0, 0);
			if( pNMCD->nmcd.uItemState & CDIS_SELECTED )	{
				pNMCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
				SelectObject(pNMCD->nmcd.hdc, m_FontBold);
				pNMCD->clrTextBk = RGB(225, 255, 221);
			}
			break;

		case 2:
			pNMCD->clrTextBk = GetSysColor( COLOR_WINDOW );
			if ( bActivate )
				pNMCD->clrText = RGB(26, 22, 146);
			else
				pNMCD->clrText = RGB(175, 175, 175);

			if( pNMCD->nmcd.uItemState & CDIS_SELECTED )	{				
				pNMCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
				SelectObject(pNMCD->nmcd.hdc, m_FontBold);
				pNMCD->clrTextBk = RGB(225, 255, 221);
				if ( bActivate )
					pNMCD->clrText = RGB(26, 22, 146);
				else
					pNMCD->clrText = RGB(175, 175, 175);
			}
			break;

		case 3:
			pNMCD->clrTextBk = GetSysColor( COLOR_WINDOW );			
			if ( bActivate )
				pNMCD->clrText = RGB(26, 22, 146);
			else
				pNMCD->clrText = RGB(175, 175, 175);
			
			if( pNMCD->nmcd.uItemState & CDIS_SELECTED )	{				
				pNMCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
				SelectObject(pNMCD->nmcd.hdc, m_FontBold);
				pNMCD->clrTextBk = RGB(225, 255, 221);
				if ( bActivate )
					pNMCD->clrText = RGB(26, 22, 146);
				else
					pNMCD->clrText = RGB(175, 175, 175);
			}
			break;

		default:
			pNMCD->clrTextBk = GetSysColor( COLOR_WINDOW );
			pNMCD->clrText = GetSysColor( COLOR_WINDOWTEXT );
			
			if( pNMCD->nmcd.uItemState & CDIS_SELECTED )	{				
				pNMCD->nmcd.uItemState &= ~CDIS_SELECTED; // kill sys sel
				SelectObject(pNMCD->nmcd.hdc, m_FontBold);
				pNMCD->clrTextBk = RGB(225, 255, 221);
				pNMCD->clrText = RGB(26, 22, 146);
			}
			break;
		}

		*pResult |= CDRF_NEWFONT;
		break;
	}
}
