#include "pch.h"
#include "framework.h"
#include "PhoneTypesView.h"
#include "PhoneTypesDialog.h"

#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesView


// Macros
// ----------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PHONE_TYPE_COLUMN_NAME _T("Phone type")

IMPLEMENT_DYNCREATE(CPhoneTypesView, CListView)

BEGIN_MESSAGE_MAP(CPhoneTypesView, CListView)
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXTMENU_SELECT, &CPhoneTypesView::OnContextMenuSelect)
	ON_COMMAND(ID_CONTEXTMENU_INSERT, &CPhoneTypesView::OnContextMenuInsert)
	ON_COMMAND(ID_CONTEXTMENU_DELETE, &CPhoneTypesView::OnContextMenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_EDIT, &CPhoneTypesView::OnContextMenuEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPhoneTypesView::AssertValid() const
{
	CListView::AssertValid();
}

void CPhoneTypesView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPhoneTypesDocument* CPhoneTypesView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPhoneTypesDocument)));
	return (CPhoneTypesDocument*)m_pDocument;
}
#endif


// Constructor / Destructor
// ----------------

CPhoneTypesView::CPhoneTypesView() noexcept
{
}

CPhoneTypesView::~CPhoneTypesView()
{
}


// Methods
// ----------------

BOOL CPhoneTypesView::GetSelectedRecordID(int& nID)
{
	CListCtrl& ListCtrl = GetListCtrl();
	POSITION pPosition = ListCtrl.GetFirstSelectedItemPosition();
	if (pPosition == NULL)
	{
		CString strErrorMessage = _T("You must select a phone type.");
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

	int nItem = ListCtrl.GetNextSelectedItem(pPosition);
	nID = ListCtrl.GetItemData(nItem);

	return TRUE;
}


// MFC Overrides
// ----------------

void CPhoneTypesView::OnDraw(CDC* /*pDC*/)
{
	CPhoneTypesDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL CPhoneTypesView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}

void CPhoneTypesView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();

	ListCtrl.SetView(LVS_REPORT);
	ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	ListCtrl.InsertColumn(ColumnPhoneType, PHONE_TYPE_COLUMN_NAME);
	ListCtrl.SetColumnWidth(ColumnPhoneType, LVSCW_AUTOSIZE_USEHEADER);

	CPhoneTypesArray& const pAllPhoneTypesArray = GetDocument()->GetAllTableRecordsArray();
	for (INT_PTR i = 0; i < pAllPhoneTypesArray.GetCount(); i++)
	{
		int nIndex = ListCtrl.InsertItem(ColumnPhoneType, pAllPhoneTypesArray.GetAt(i)->szPhoneType);
		ListCtrl.SetItemData(nIndex, pAllPhoneTypesArray.GetAt(i)->nID);
	}
}

void CPhoneTypesView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

	CObjectWrapper<PHONE_TYPES>* pObjectWrapper = dynamic_cast<CObjectWrapper<PHONE_TYPES>*>(pHint);
	PHONE_TYPES& PhoneType = pObjectWrapper->GetObject();

	int nIndex{};
	if (lHint == OPERATION_EDIT)
	{
		nIndex = ListCtrl.GetSelectionMark();
		ListCtrl.SetItemText(nIndex, ColumnPhoneType, PhoneType.szPhoneType);
	}
	else if (lHint == OPERATION_INSERT)
	{
		nIndex = ListCtrl.InsertItem(ColumnPhoneType, PhoneType.szPhoneType);
	}

	ListCtrl.SetItemData(nIndex, PhoneType.nID);

}

BOOL CPhoneTypesView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CPhoneTypesView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPhoneTypesView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// MFC Message Handlers
// ----------------

void CPhoneTypesView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
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

void CPhoneTypesView::OnContextMenuSelect()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	PHONE_TYPES* pPhoneType;
	bResult = GetDocument()->FindRecInDoc(nID, pPhoneType);
	if (!bResult)
	{
		return;
	}

	CPhoneTypesDialog oPhoneTypesDialog(CPhoneTypesDialog::Operations::OperationSelect, *pPhoneType);
	oPhoneTypesDialog.DoModal();
}

void CPhoneTypesView::OnContextMenuEdit()
{

	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	PHONE_TYPES* pPhoneType;
	bResult = GetDocument()->FindRecInDoc(nID, pPhoneType);
	if (!bResult)
	{
		return;
	}
	PHONE_TYPES* pPhoneTypeCopy = new PHONE_TYPES;
	*pPhoneTypeCopy = *pPhoneType;
	CPhoneTypesDialog oPhoneTypesDialog(CPhoneTypesDialog::Operations::OperationEdit, *pPhoneTypeCopy);

	const int iResult = oPhoneTypesDialog.DoModal();
	if (iResult != IDOK)
	{
		return;
	}

	bResult = GetDocument()->UpdateWhereID(nID, *pPhoneTypeCopy);
	if (!bResult)
	{
		return;
	}
}

void CPhoneTypesView::OnContextMenuInsert()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	PHONE_TYPES* pPhoneType(new PHONE_TYPES);
	pPhoneType->nUpdateCounter = 0;
	CPhoneTypesDialog oPhoneTypesDialog(CPhoneTypesDialog::Operations::OperationInsert, *pPhoneType);

	const int iResult = oPhoneTypesDialog.DoModal();
	if (iResult != IDOK)
	{
		return;
	}

	bResult = GetDocument()->Insert(*pPhoneType);
	if (!bResult)
	{
		return;
	}
}

void CPhoneTypesView::OnContextMenuDelete()
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

void CPhoneTypesView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnContextMenuSelect();
}