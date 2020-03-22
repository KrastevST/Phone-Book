#include "pch.h"
#include "PersonsDialog.h"
#include "PhoneNumbersDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsDialog


// Macros
// ----------------
#define EMPTY _T("")
#define PHONE_NUMBER _T("Number")
#define PHONE_TYPE _T("Type")

IMPLEMENT_DYNAMIC(CPersonsDialog, CDialog)

BEGIN_MESSAGE_MAP(CPersonsDialog, CDialog)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXTMENU_SELECT, &CPersonsDialog::OnContextMenuSelect)
	ON_COMMAND(ID_CONTEXTMENU_INSERT, &CPersonsDialog::OnContextMenuInsert)
	ON_COMMAND(ID_CONTEXTMENU_DELETE, &CPersonsDialog::OnContextMenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_EDIT, &CPersonsDialog::OnContextMenuEdit)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------

CPersonsDialog::CPersonsDialog(Operations eOperation, CPersonWithPhoneNums& oPersonWithPhoneNums, CCitiesMap& oCitiesMap, CPhoneTypesMap& oPhoneTypesMap, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PERSONS_DIALOG, pParent),
	m_PersonWithPhoneNums(oPersonWithPhoneNums),
	m_CitiesMap(oCitiesMap),
	m_PhoneTypesMap(oPhoneTypesMap)
{
	m_eOperation = eOperation;
}

CPersonsDialog::~CPersonsDialog()
{
}


// MFC Overrides
// ----------------

void CPersonsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_PERSONS_FIRST_NAME, m_edbFirstName);
	DDX_Control(pDX, IDC_EDB_PERSONS_MIDDLE_NAME, m_edbMiddleName);
	DDX_Control(pDX, IDC_EDB_PERSONS_LAST_NAME, m_edbLastName);
	DDX_Control(pDX, IDC_EDB_PERSONS_EGN, m_edbEGN);
	DDX_Control(pDX, IDC_EDB_PERSONS_ADDRESS, m_edbAddress);
	DDX_Control(pDX, IDC_LSC_PERSONS_PHONE_NUMBERS, m_lscPhoneNumbers);
	DDX_Control(pDX, IDC_CMB_PERSONS_CITY_ID, m_cmbCity);
}


// MFC Message Handlers
// ----------------

BOOL CPersonsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbFirstName.SetLimitText(PERSON_FIRST_NAME_SIZE);
	m_edbMiddleName.SetLimitText(PERSON_MIDDLE_NAME_SIZE);
	m_edbLastName.SetLimitText(PERSON_LAST_NAME_SIZE);
	m_edbEGN.SetLimitText(PERSON_EGN_SIZE);
	m_edbAddress.SetLimitText(PERSON_ADDRESS_SIZE);

	m_edbFirstName.EnableWindow(TRUE);
	m_edbMiddleName.EnableWindow(TRUE);
	m_edbLastName.EnableWindow(TRUE);
	m_edbEGN.EnableWindow(TRUE);
	m_edbAddress.EnableWindow(TRUE);
	m_cmbCity.EnableWindow(TRUE);

	m_PersonWithPhoneNums.GetPerson();
	m_edbFirstName.SetWindowText(m_PersonWithPhoneNums.GetPerson().szFirstName);
	m_edbMiddleName.SetWindowText(m_PersonWithPhoneNums.GetPerson().szMiddleName);
	m_edbLastName.SetWindowText(m_PersonWithPhoneNums.GetPerson().szLastName);
	m_edbEGN.SetWindowText(m_PersonWithPhoneNums.GetPerson().szEGN);
	m_edbAddress.SetWindowText(m_PersonWithPhoneNums.GetPerson().szAddress);

	CITIES* p_recCity{};
	BOOL bResult = m_CitiesMap.Lookup(m_PersonWithPhoneNums.GetPerson().nCityID, p_recCity);
	if (bResult)
	{
		m_cmbCity.SetWindowText(p_recCity->szName);
	}
	else if (m_eOperation != Operations::OperationInsert)
	{
		CString strMessage;
		strMessage.Format(_T("Error city not found in local storage."));
		AfxMessageBox(strMessage);
		return FALSE;
	}

	CCitiesMap::CPair* pCurrentPair{};
	pCurrentPair = m_CitiesMap.PGetFirstAssoc();
	while (pCurrentPair != NULL)
	{
		int nIndex = m_cmbCity.AddString(pCurrentPair->value->szName);
		m_cmbCity.SetItemData(nIndex, pCurrentPair->value->nID);
		pCurrentPair = m_CitiesMap.PGetNextAssoc(pCurrentPair);
	}

	m_lscPhoneNumbers.SetView(LVS_REPORT);
	m_lscPhoneNumbers.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lscPhoneNumbers.InsertColumn(ColumnPhoneNumber, PHONE_NUMBER);
	m_lscPhoneNumbers.InsertColumn(ColumnPhoneType, PHONE_TYPE);
	m_lscPhoneNumbers.SetColumnWidth(ColumnPhoneNumber, LVSCW_AUTOSIZE_USEHEADER);
	m_lscPhoneNumbers.SetColumnWidth(ColumnPhoneType, LVSCW_AUTOSIZE_USEHEADER);
	
	for (INT_PTR i = 0; i < m_PersonWithPhoneNums.GetPhoneNumbers().GetCount(); i++)
	{
		const PHONE_NUMBERS& PhoneNumber = *m_PersonWithPhoneNums.GetPhoneNumbers().GetAt(i);
		int nIndex = m_lscPhoneNumbers.InsertItem(ColumnPhoneNumber, PhoneNumber.szPhoneNumber);
		int  nPhoneTypeId = PhoneNumber.nPhoneTypeID;
		PHONE_TYPES* p_recPhoneType{};
		m_PhoneTypesMap.Lookup(nPhoneTypeId, p_recPhoneType);
		m_lscPhoneNumbers.SetItemText(nIndex, ColumnPhoneType, p_recPhoneType->szPhoneType);
		m_lscPhoneNumbers.SetItemData(nIndex, PhoneNumber.nID);
	}

	UpdateState();

	return TRUE;
}

void CPersonsDialog::OnOK()
{
	if (m_eOperation != OperationSelect)
	{
		BOOL bResult = SaveChanges();
		if (!bResult)
		{
			return;
		}
	}

	CDialog::OnOK();
}

void CPersonsDialog::OnContextMenuSelect()
{
	int nID{};
	BOOL bResult = GetSelectedRecordID(nID);
	if (!bResult)
	{
		return;
	}

	CPhoneNumbersArray& PhoneNumbersArray = m_PersonWithPhoneNums.GetPhoneNumbers();

	int nIndex{};
	bResult = FALSE;
	for (INT_PTR i = 0; i < PhoneNumbersArray.GetCount(); i++)
	{
		if (PhoneNumbersArray.GetAt(i)->nID == nID)
		{
			nIndex = i;
			bResult = TRUE;
			break;
		}
	}

	if (!bResult)
	{
		CString strMessage;
		strMessage.Format(_T("Error phone type not found."));
		AfxMessageBox(strMessage);
		return;
	}

	CPhoneNumbersDialog oPhoneNumbersDialog(CPhoneNumbersDialog::Operations::OperationSelect, *PhoneNumbersArray.GetAt(nIndex), m_PhoneTypesMap);
	oPhoneNumbersDialog.DoModal();
}

void CPersonsDialog::OnContextMenuEdit()
{

}

void CPersonsDialog::OnContextMenuInsert()
{

}

void CPersonsDialog::OnContextMenuDelete()
{

}

void CPersonsDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}

// Methods
//--------------------

void CPersonsDialog::UpdateState()
{
	if (m_eOperation == Operations::OperationSelect)
	{
		m_edbFirstName.EnableWindow(FALSE);
		m_edbMiddleName.EnableWindow(FALSE);
		m_edbLastName.EnableWindow(FALSE);
		m_edbEGN.EnableWindow(FALSE);
		m_edbAddress.EnableWindow(FALSE);
		m_cmbCity.EnableWindow(FALSE);

	}
	else if (m_eOperation == Operations::OperationInsert)
	{
		m_edbFirstName.SetWindowText(EMPTY);
		m_edbMiddleName.SetWindowText(EMPTY);
		m_edbLastName.SetWindowText(EMPTY);
		m_edbEGN.SetWindowText(EMPTY);
		m_edbAddress.SetWindowText(EMPTY);
		m_cmbCity.SetWindowText(EMPTY);

	}
}

BOOL CPersonsDialog::SaveChanges()
{
	const int iResult = MessageBox(_T("Are you sure you want to save changes?"), NULL, MB_YESNO);
	if (iResult != IDYES)
	{
		return FALSE;
	}

	CString strWindowText;
	m_edbFirstName.GetWindowText(strWindowText);
	lstrcpy(m_PersonWithPhoneNums.GetPerson().szFirstName, strWindowText);
	m_edbMiddleName.GetWindowText(strWindowText);
	lstrcpy(m_PersonWithPhoneNums.GetPerson().szMiddleName, strWindowText);
	m_edbLastName.GetWindowText(strWindowText);
	lstrcpy(m_PersonWithPhoneNums.GetPerson().szLastName, strWindowText);
	m_edbEGN.GetWindowText(strWindowText);
	lstrcpy(m_PersonWithPhoneNums.GetPerson().szEGN, strWindowText);
	m_edbAddress.GetWindowText(strWindowText);
	lstrcpy(m_PersonWithPhoneNums.GetPerson().szAddress, strWindowText);

	int nIndex = m_cmbCity.GetCurSel();
	int nCityID = m_cmbCity.GetItemData(nIndex);

	m_PersonWithPhoneNums.GetPerson().nCityID = nCityID;

	return TRUE;
}

BOOL CPersonsDialog::GetSelectedRecordID(int nID)
{
	POSITION pPosition = m_lscPhoneNumbers.GetFirstSelectedItemPosition();
	if (pPosition == NULL)
	{
		CString strErrorMessage = _T("You must select a phone number.");
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

	int nItem = m_lscPhoneNumbers.GetNextSelectedItem(pPosition);
	nID = m_lscPhoneNumbers.GetItemData(nItem);

	return TRUE;
}