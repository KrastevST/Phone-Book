#include "pch.h"
#include "framework.h"
#include "PersonsView.h"
#include "Operations.h"

#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPersonsView


// Macros
// ----------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FIRST_NAME_COLUMN_NAME _T("First name")
#define MIDDLE_NAME_COLUMN_NAME _T("Middle name")
#define LAST_NAME_COLUMN_NAME _T("Last name")
#define CITY_COLUMN_NAME _T("City")
#define EGN_COLUMN_NAME _T("EGN")
#define ADDRESS_COLUMN_NAME _T("Address")

#define SELECT CPersonsDialog::Operations::OperationSelect
#define EDIT CPersonsDialog::Operations::OperationEdit
#define INSERT CPersonsDialog::Operations::OperationInsert

IMPLEMENT_DYNCREATE(CPersonsView, CListView)

BEGIN_MESSAGE_MAP(CPersonsView, CListView)
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXTMENU_SELECT, &CPersonsView::OnContextMenuSelect)
	ON_COMMAND(ID_CONTEXTMENU_INSERT, &CPersonsView::OnContextMenuInsert)
	ON_COMMAND(ID_CONTEXTMENU_DELETE, &CPersonsView::OnContextMenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_EDIT, &CPersonsView::OnContextMenuEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPersonsView::AssertValid() const
{
	CListView::AssertValid();
}

void CPersonsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CPersonsDocument* CPersonsView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPersonsDocument)));
	return (CPersonsDocument*)m_pDocument;
}
#endif


// Constructor / Destructor
// ----------------

CPersonsView::CPersonsView() noexcept
{
}

CPersonsView::~CPersonsView()
{
}


// Methods
// ----------------

BOOL CPersonsView::GetSelectedRecordID(int& nID)
{
	CListCtrl& ListCtrl = GetListCtrl();
	POSITION pPosition = ListCtrl.GetFirstSelectedItemPosition();
	if (pPosition == NULL)
	{
		CString strErrorMessage = _T("You must select a person.");
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

	int nItem = ListCtrl.GetNextSelectedItem(pPosition);
	nID = ListCtrl.GetItemData(nItem);

	return TRUE;
}


// MFC Overrides
// ----------------

void CPersonsView::OnDraw(CDC* /*pDC*/)
{
	CPersonsDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

BOOL CPersonsView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CListView::PreCreateWindow(cs);
}

void CPersonsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();

	ListCtrl.SetView(LVS_REPORT);
	ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	ListCtrl.InsertColumn(ColumnFirstName, FIRST_NAME_COLUMN_NAME);
	ListCtrl.InsertColumn(ColumnMiddleName, MIDDLE_NAME_COLUMN_NAME);
	ListCtrl.InsertColumn(ColumnLastName, LAST_NAME_COLUMN_NAME);
	ListCtrl.InsertColumn(ColumnCity, CITY_COLUMN_NAME);
	ListCtrl.InsertColumn(ColumnEGN, EGN_COLUMN_NAME);
	ListCtrl.InsertColumn(ColumnAddress, ADDRESS_COLUMN_NAME);

	ListCtrl.SetColumnWidth(ColumnFirstName, LVSCW_AUTOSIZE_USEHEADER);
	ListCtrl.SetColumnWidth(ColumnMiddleName, LVSCW_AUTOSIZE_USEHEADER);
	ListCtrl.SetColumnWidth(ColumnLastName, LVSCW_AUTOSIZE_USEHEADER);
	ListCtrl.SetColumnWidth(ColumnCity, LVSCW_AUTOSIZE_USEHEADER);
	ListCtrl.SetColumnWidth(ColumnEGN, LVSCW_AUTOSIZE_USEHEADER);
	ListCtrl.SetColumnWidth(ColumnAddress, LVSCW_AUTOSIZE_USEHEADER);

	CPersonsArray& const PersonsArray = GetDocument()->GetPersonsArray();
	for (INT_PTR i = 0; i < PersonsArray.GetCount(); i++)
	{
		PERSONS* pPerson = PersonsArray.GetAt(i);
		int nIndex = ListCtrl.InsertItem(ColumnFirstName, PersonsArray.GetAt(i)->szFirstName);
		ListCtrl.SetItemText(nIndex, ColumnMiddleName, PersonsArray.GetAt(i)->szMiddleName);
		ListCtrl.SetItemText(nIndex, ColumnLastName, PersonsArray.GetAt(i)->szLastName);
		ListCtrl.SetItemText(nIndex, ColumnEGN, PersonsArray.GetAt(i)->szEGN);
		ListCtrl.SetItemText(nIndex, ColumnAddress, PersonsArray.GetAt(i)->szAddress);

		int nCityID = PersonsArray.GetAt(i)->nCityID;
		CITIES* p_recCity{};
		GetDocument()->GetCitiesMap().Lookup(nCityID, p_recCity);
		ListCtrl.SetItemText(nIndex, ColumnCity, p_recCity->szName);
		ListCtrl.SetItemData(nIndex, PersonsArray.GetAt(i)->nID);
	}
}

void CPersonsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
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

	CObjectWrapper<PERSONS>* pObjectWrapper = dynamic_cast<CObjectWrapper<PERSONS>*>(pHint);
	PERSONS& Person = pObjectWrapper->GetObject();

	int nIndex{};
	if (lHint == OPERATION_EDIT)
	{
		nIndex = ListCtrl.GetSelectionMark();
		ListCtrl.SetItemText(nIndex, ColumnFirstName, Person.szFirstName);

	}
	else if (lHint == OPERATION_INSERT)
	{
		nIndex = ListCtrl.InsertItem(ColumnFirstName, Person.szFirstName);
	}

	ListCtrl.SetItemText(nIndex, ColumnMiddleName, Person.szMiddleName);
	ListCtrl.SetItemText(nIndex, ColumnLastName, Person.szLastName);
	ListCtrl.SetItemText(nIndex, ColumnEGN, Person.szEGN);
	ListCtrl.SetItemText(nIndex, ColumnAddress, Person.szAddress);

	int nCityID = Person.nCityID;
	CITIES* p_recCity{};
	GetDocument()->GetCitiesMap().Lookup(nCityID, p_recCity);
	ListCtrl.SetItemText(nIndex, ColumnCity, p_recCity->szName);
	ListCtrl.SetItemData(nIndex, Person.nID);
}

BOOL CPersonsView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CPersonsView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CPersonsView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}


// MFC Message Handlers
// ----------------

void CPersonsView::OnContextMenu(CWnd* pWnd, CPoint ptMousePos)
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

void CPersonsView::OnContextMenuSelect()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	PERSONS* pPerson;
	bResult = GetDocument()->FindRecInDoc(nID, pPerson);
	if (!bResult)
	{
		return;
	}

	CPersonWithPhoneNums oPersonWithNums(*pPerson);
	bResult = GetDocument()->AddPhoneNumsToPerson(oPersonWithNums);
	if (!bResult)
	{
		return;
	}

	CCitiesMap& CitiesMap = GetDocument()->GetCitiesMap();
	CPhoneTypesMap& PhoneTypesMap = GetDocument()->GetPhoneTypesMap();
	CPersonsDialog oPersonsDialog(SELECT, oPersonWithNums, CitiesMap, PhoneTypesMap);
	oPersonsDialog.DoModal();
}

void CPersonsView::OnContextMenuEdit()
{

	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	PERSONS* pPerson;
	bResult = GetDocument()->FindRecInDoc(nID, pPerson);
	if (!bResult)
	{
		return;
	}
	
	CPersonWithPhoneNums oPersonWithNums(*pPerson);
	bResult = GetDocument()->AddPhoneNumsToPerson(oPersonWithNums);
	if (!bResult)
	{
		return;
	}

	CPersonsDialog oPersonsDialog(EDIT, oPersonWithNums, GetDocument()->GetCitiesMap(), GetDocument()->GetPhoneTypesMap());
	const int iResult = oPersonsDialog.DoModal();
	if (iResult != IDOK)
	{
		return;
	}

	//bResult = GetDocument()->UpdateWhereID(nID, oPersonWithNums);
	if (!bResult)
	{
		return;
	}
}

void CPersonsView::OnContextMenuInsert()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	CPersonWithPhoneNums oPersonWithNums(*(new PERSONS()));
	oPersonWithNums.GetPerson().nUpdateCounter = 0;

	CCitiesMap& CitiesMap = GetDocument()->GetCitiesMap();
	CPhoneTypesMap& PhoneTypesMap = GetDocument()->GetPhoneTypesMap();
	CPersonsDialog oPersonsDialog(INSERT, oPersonWithNums, CitiesMap, PhoneTypesMap);
	int iResult = oPersonsDialog.DoModal();
	if (iResult != IDOK)
	{
		return;
	}

	bResult = GetDocument()->Insert(oPersonWithNums);
	if (!bResult)
	{
		return;
	}
}

void CPersonsView::OnContextMenuDelete()
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

void CPersonsView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnContextMenuSelect();
}