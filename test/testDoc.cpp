// testDoc.cpp : implementation of the CtestDoc class
//

#include "stdafx.h"
#include "test.h"

#include "testDoc.h"
#include "testView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDoc

IMPLEMENT_DYNCREATE(CtestDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestDoc, CDocument)
END_MESSAGE_MAP()


// CtestDoc construction/destruction

CtestDoc::CtestDoc()
{
	// TODO: add one-time construction code here

}

CtestDoc::~CtestDoc()
{
}

BOOL CtestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CtestDoc serialization

void CtestDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CtestDoc diagnostics

#ifdef _DEBUG
void CtestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestDoc commands
