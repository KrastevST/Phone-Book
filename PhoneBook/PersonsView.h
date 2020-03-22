#pragma once

#include "PersonsDocument.h"
#include "PersonsDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsView

class CPersonsView : public CListView
{

// Macros
// ----------------
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_DYNCREATE(CPersonsView)
	DECLARE_MESSAGE_MAP()
#ifndef _DEBUG 
	inline CPersonsDocument* CPersonsView::GetDocument() const
	{
		return reinterpret_cast<CPersonsDocument*>(m_pDocument);
	}
#endif

// Constants
// ----------------
private:
	enum Columns { ColumnFirstName, ColumnMiddleName, ColumnLastName, ColumnCity, ColumnEGN, ColumnAddress};

// Constructor / Destructor
// ----------------
public:
	CPersonsView() noexcept;
	virtual ~CPersonsView();

// Methods
// ----------------
public:
	afx_msg void OnContextMenuSelect();
	afx_msg void OnContextMenuEdit();
	afx_msg void OnContextMenuInsert();
	afx_msg void OnContextMenuDelete();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
private:
	BOOL GetSelectedRecordID(int& nID);

// Overrides
// ----------------
public:
	virtual void OnDraw(CDC* pDC)override;
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs)override;	
protected:
	virtual void OnInitialUpdate() override;
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

// Message Handlers
// ----------------
	CPersonsDocument* GetDocument() const;
	void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);

// Members
//--------------------
private:
	CMenu m_oContextMenu;
	CCitiesData m_oCitiesData;
};

