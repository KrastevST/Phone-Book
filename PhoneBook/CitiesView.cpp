#include "pch.h"
#include "framework.h"
#include "CitiesView.h"
#include "Operations.h"

#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CCitiesView


// Macros
// ----------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CITY_COLUMN_NAME _T("City")
#define REGION_COLUMN_NAME _T("Region")

IMPLEMENT_DYNCREATE(CCitiesView, CListView)

BEGIN_MESSAGE_MAP(CCitiesView, CListView)
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXTMENU_SELECT, &CCitiesView::OnContextMenuSelect)
	ON_COMMAND(ID_CONTEXTMENU_INSERT, &CCitiesView::OnContextMenuInsert)
	ON_COMMAND(ID_CONTEXTMENU_DELETE, &CCitiesView::OnContextMenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_EDIT, &CCitiesView::OnContextMenuEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CCitiesView::AssertValid() const
{
	CListView::AssertValid();
}

void CCitiesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CCitiesDoc* CCitiesView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCitiesDoc)));
	return (CCitiesDoc*)m_pDocument;
}
#endif


// Constructor / Destructor
// ----------------

CCitiesView::CCitiesView() noexcept
{
}

CCitiesView::~CCitiesView()
{
}


// Methods
// ----------------

BOOL CCitiesView::GetSelectedRecordID(int& nID)
{
	CListCtrl& ListCtrl = GetListCtrl();
	POSITION pPosition = ListCtrl.GetFirstSelectedItemPosition();
	if (pPosition == NULL)
	{
		CString strErrorMessage = _T("You must select a city.");
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

	int nItem = ListCtrl.GetNextSelectedItem(pPosition);
	nID = ListCtrl.GetItemData(nItem);

	return TRUE;
}


// MFC Overrides
// ----------------

void CCitiesView::OnDraw(CDC* /*pDC*/)
{
	CCitiesDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL CCitiesView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}

void CCitiesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();

	ListCtrl.SetView(LVS_REPORT);
	ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	ListCtrl.InsertColumn(ColumnName, CITY_COLUMN_NAME);
	ListCtrl.InsertColumn(ColumnRegion, REGION_COLUMN_NAME);

	ListCtrl.SetColumnWidth(ColumnName, LVSCW_AUTOSIZE_USEHEADER);
	ListCtrl.SetColumnWidth(ColumnRegion, LVSCW_AUTOSIZE_USEHEADER);

	CCitiesArray& const AllCitiesArray = GetDocument()->GetAllTableRecordsArray();
	for (INT_PTR i = 0; i < AllCitiesArray.GetCount(); i++)
	{
		int nIndex = ListCtrl.InsertItem(ColumnName, AllCitiesArray.GetAt(i)->szName);
		ListCtrl.SetItemText(nIndex, ColumnRegion, AllCitiesArray.GetAt(i)->szRegion);
		ListCtrl.SetItemData(nIndex, AllCitiesArray.GetAt(i)->nID);
	}
}

void CCitiesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if (!lHint)
	{
		return;
	}

	CListCtrl& ListCtrl = GetListCtrl();

	if (lHint == OPERATION_DELETE)
	{
		int nIndex = ListCtrl.GetSelectionMark();
		ListCtrl.DeleteItem(nIndex);
		return;
	}

	CObjectWrapper<CITIES>* pObjectWrapper = dynamic_cast<CObjectWrapper<CITIES>*>(pHint);
	CITIES& City = pObjectWrapper->GetObject();

	int nIndex{};
	if (lHint == OPERATION_EDIT)
	{
		nIndex = ListCtrl.GetSelectionMark();
		ListCtrl.SetItemText(nIndex, ColumnName, City.szName);
		
	}
	else if (lHint == OPERATION_INSERT)
	{
		nIndex = ListCtrl.InsertItem(ColumnName, City.szName);
	}

	ListCtrl.SetItemText(nIndex, ColumnRegion, City.szRegion);
	ListCtrl.SetItemData(nIndex, City.nID);
}

BOOL CCitiesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CCitiesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CCitiesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// MFC Message Handlers
// ----------------

void CCitiesView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
{
	CPoint oHitPoint = ptMousePos;
	ScreenToClient(&oHitPoint);

	CMenu oSubmenu;
	oSubmenu.CreatePopupMenu();
	oSubmenu.AppendMenuW(MF_STRING, ID_CONTEXTMENU_SELECT, L"Select");
	oSubmenu.AppendMenuW(MF_SEPARATOR);
	oSubmenu.AppendMenuW(MF_STRING, ID_CONTEXTMENU_EDIT, L"Edit");
	oSubmenu.AppendMenuW(MF_STRING, ID_CONTEXTMENU_INSERT, L"Insert");
	oSubmenu.AppendMenuW(MF_STRING, ID_CONTEXTMENU_DELETE, L"Delete");

	oSubmenu.TrackPopupMenu(TPM_LEFTALIGN, ptMousePos.x, ptMousePos.y, this);
}

void CCitiesView::OnContextMenuSelect()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	CITIES* pCity;
	bResult = GetDocument()->FindRecInDoc(nID, pCity);
	if (!bResult)
	{
		return;
	}

	CCitiesDialog oCitiesDialog(CCitiesDialog::Operations::OperationSelect, pCity);
	oCitiesDialog.DoModal();
}

void CCitiesView::OnContextMenuEdit()
{

	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	CITIES* pCity;
	bResult = GetDocument()->FindRecInDoc(nID, pCity);
	if (!bResult)
	{
		return;
	}
	CITIES* pCityCopy = new CITIES;
	*pCityCopy = *pCity;
	CCitiesDialog oCitiesDialog(CCitiesDialog::Operations::OperationEdit, pCityCopy);

	const int iResult = oCitiesDialog.DoModal();
	if (iResult != IDOK)
	{
		return;
	}

	bResult = GetDocument()->UpdateWhereID(nID, *pCityCopy);
	if (!bResult)
	{
		return;
	}
}

void CCitiesView::OnContextMenuInsert()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	CITIES* pCity = new CITIES();
	pCity->nUpdateCounter = 0;
	CCitiesDialog oCitiesDialog(CCitiesDialog::Operations::OperationInsert, pCity);

	const int iResult = oCitiesDialog.DoModal();
	if (iResult != IDOK)
	{
		return;
	}

	bResult = GetDocument()->Insert(*pCity);
	if (!bResult)
	{
		return;
	}
}

void CCitiesView::OnContextMenuDelete()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	const int iResult = MessageBox(_T("Are you sure you want to delete the selected record"), NULL, MB_YESNO);
	if (iResult != IDYES)
	{
		return;
	}

	bResult = GetDocument()->DeleteWhereID(nID);
	if (!bResult)
	{
		return;
	}
}

void CCitiesView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnContextMenuSelect();
}