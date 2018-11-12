// testView.h : interface of the CtestView class
//


#pragma once


class CtestView : public CListView
{
protected: // create from serialization only
	CtestView();
	DECLARE_DYNCREATE(CtestView)

// Attributes
public:
	CtestDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnColumnClick(UINT id, NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR *pNMHDR, LRESULT *pResult);

	// вызываетс€ в первый раз после конструктора
	virtual void OnInitialUpdate(void);
	// заполн€ет грид содержимым
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);

	void PrepareCache(int iFrom, int iTo);
	//void GetDispInfo(LVITEM* pItem);
	int FindItem(int iStart, LVFINDINFO* plvfi);
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	CFont m_FontBold;

// Implementation
public:
	virtual ~CtestView();
	//ќбновление только строки с номером nItem
	DWORD UpdateRows(int nRows = -1);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void UpdateRecord(void);
protected:
	//	Ќомер текущей строки.
	int	m_nRowsSel;
};

#ifndef _DEBUG  // debug version in testView.cpp
inline CtestDoc* CtestView::GetDocument() const
   { return reinterpret_cast<CtestDoc*>(m_pDocument); }
#endif

