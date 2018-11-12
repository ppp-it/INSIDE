// testDoc.h : interface of the CtestDoc class
//


#pragma once


class CtestDoc : public CDocument
{
protected: // create from serialization only
	CtestDoc();
	DECLARE_DYNCREATE(CtestDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CtestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


