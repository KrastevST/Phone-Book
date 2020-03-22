#include "pch.h"
#include "PhoneTypesDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDialog


// Macros
// ----------------
#define EMPTY _T("")

IMPLEMENT_DYNAMIC(CPhoneTypesDialog, CDialog)

BEGIN_MESSAGE_MAP(CPhoneTypesDialog, CDialog)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------

CPhoneTypesDialog::CPhoneTypesDialog(Operations eOperation, PHONE_TYPES& recPhoneType, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PHONE_TYPES_DIALOG, pParent),
	m_PhoneType(recPhoneType)
{
	m_eOperation = eOperation;
}

CPhoneTypesDialog::~CPhoneTypesDialog()
{
}


// MFC Overrides
// ----------------

void CPhoneTypesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_PHONE_TYPES_PHONE_TYPE, m_edbPhoneType);
}


// MFC Message Handlers
// ----------------

BOOL CPhoneTypesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_edbPhoneType.SetLimitText(PHONE_TYPES_PHONE_TYPE_SIZE);
	m_edbPhoneType.EnableWindow(TRUE);
	m_edbPhoneType.SetWindowText(m_PhoneType.szPhoneType);

	UpdateState();

	return TRUE;
}


void CPhoneTypesDialog::OnOK()
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

void CPhoneTypesDialog::UpdateState()
{
	if (m_eOperation == Operations::OperationSelect)
	{
		m_edbPhoneType.EnableWindow(FALSE);
	}
	else if (m_eOperation == Operations::OperationInsert)
	{
		m_edbPhoneType.SetWindowText(EMPTY);
	}
}

BOOL CPhoneTypesDialog::SaveChanges()
{
	const int iResult = MessageBox(_T("Are you sure you want to save changes?"), NULL, MB_YESNO);
	if (iResult != IDYES)
	{
		return FALSE;
	}

	CString strWindowText;
	m_edbPhoneType.GetWindowText(strWindowText);
	lstrcpy(m_PhoneType.szPhoneType, strWindowText);

	return TRUE;
}