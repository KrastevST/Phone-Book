#pragma once

#include "CitiesDoc.h"
#include "CitiesDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesView

class CCitiesView : public CListView
{

// Macros
// ----------------
public:
	#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
	#endif

protected:
	DECLARE_DYNCREATE(CCitiesView)
	DECLARE_MESSAGE_MAP()
	#ifndef _DEBUG 
		inline CCitiesDoc* CCitiesView::GetDocument() const
		{
			return reinterpret_cast<CCitiesDoc*>(m_pDocument);
		}
	#endif

// Constants
// ----------------
private:
	enum Columns { ColumnName, ColumnRegion };

// Constructor / Destructor
// ----------------
public:
	CCitiesView() noexcept;
	virtual ~CCitiesView();

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
	CCitiesDoc* GetDocument() const;
	void OnContextMenu(CWnd* pWnd, CPoint ptMousePos);

// Members
//--------------------
private:
	CMenu m_oContextMenu;
};