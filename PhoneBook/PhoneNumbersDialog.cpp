#include "pch.h"
#include "PhoneNumbersDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDialog


// Macros
// ----------------
#define EMPTY _T("")

IMPLEMENT_DYNAMIC(CPhoneNumbersDialog, CDialog)

BEGIN_MESSAGE_MAP(CPhoneNumbersDialog, CDialog)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------

CPhoneNumbersDialog::CPhoneNumbersDialog(Operations eOperation, PHONE_NUMBERS& recPhoneNumber, CPhoneTypesMap& oPhoneTypesMap, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PHONE_NUMBERS_DIALOG, pParent),
	m_recPhoneNumber(recPhoneNumber),
	m_PhoneTypesMap(oPhoneTypesMap)
{
	m_eOperation = eOperation;
}

CPhoneNumbersDialog::~CPhoneNumbersDialog()
{
}


// MFC Overrides
// ----------------

void CPhoneNumbersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_PHONE_NUMBERS_PHONE_NUMBER, m_edbPhoneNumber);
	DDX_Control(pDX, IDC_CMB_PHONE_TYPE, m_cmbPhoneType);
}


// MFC Message Handlers
// ----------------

BOOL CPhoneNumbersDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbPhoneNumber.SetLimitText(PHONE_NUMBERS_PHONE_NUMBER_SIZE);
	m_edbPhoneNumber.EnableWindow(TRUE);
	m_cmbPhoneType.EnableWindow(TRUE);

	m_edbPhoneNumber.SetWindowText(m_recPhoneNumber.szPhoneNumber);

	PHONE_TYPES* pPhoneType{};
	BOOL bResult = m_PhoneTypesMap.Lookup(m_recPhoneNumber.nPhoneTypeID, pPhoneType);
	if (bResult)
	{
		m_cmbPhoneType.SetWindowText(pPhoneType->szPhoneType);
	}
	else if (m_eOperation != Operations::OperationInsert)
	{
		CString strMessage;
		strMessage.Format(_T("Error phone type not found in local storage."));
		AfxMessageBox(strMessage);
		return FALSE;
	}

	CPhoneTypesMap::CPair* pCurrentPair{};
	pCurrentPair = m_PhoneTypesMap.PGetFirstAssoc();
	while (pCurrentPair != NULL)
	{
		int nIndex = m_cmbPhoneType.AddString(pCurrentPair->value->szPhoneType);
		m_cmbPhoneType.SetItemData(nIndex, pCurrentPair->value->nID);
		pCurrentPair = m_PhoneTypesMap.PGetNextAssoc(pCurrentPair);
	}

	UpdateState();

	return TRUE;
}


void CPhoneNumbersDialog::OnOK()
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


// Methods
//--------------------

void CPhoneNumbersDialog::UpdateState()
{
	if (m_eOperation == Operations::OperationSelect)
	{
		m_edbPhoneNumber.EnableWindow(FALSE);
		m_cmbPhoneType.EnableWindow(FALSE);
	}
	else if (m_eOperation == Operations::OperationInsert)
	{
		m_edbPhoneNumber.SetWindowText(EMPTY);
		m_cmbPhoneType.SetWindowText(EMPTY);
	}
}

BOOL CPhoneNumbersDialog::SaveChanges()
{
	const int iResult = MessageBox(_T("Are you sure you want to save changes?"), NULL, MB_YESNO);
	if (iResult != IDYES)
	{
		return FALSE;
	}

	CString strWindowText;
	m_edbPhoneNmber.GetWindowText(strWindowText);
	lstrcpy(m_recPhoneNumber.szPhoneNumber, strWindowText);

	int nIndex = m_cmbPhoneType.GetCurSel();
	int nID = m_cmbPhoneType.GetItemData(nIndex);
	m_recPhoneNumber.nID = nID;

	return TRUE;
}
