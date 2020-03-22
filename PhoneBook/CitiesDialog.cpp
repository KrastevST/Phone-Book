#include "pch.h"
#include "CitiesDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDialog


// Macros
// ----------------
#define EMPTY _T("")

IMPLEMENT_DYNAMIC(CCitiesDialog, CDialog)

BEGIN_MESSAGE_MAP(CCitiesDialog, CDialog)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------

CCitiesDialog::CCitiesDialog(Operations eOperation, CITIES* pCity, CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CITIES_DIALOG, pParent)
{
	m_eOperation = eOperation;
	m_pCity = pCity;
}

CCitiesDialog::~CCitiesDialog()
{
}


// MFC Overrides
// ----------------

void CCitiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_CITIES_NAME, m_edbName);
	DDX_Control(pDX, IDC_STT_CITIES_NAME, m_sttName);
	DDX_Control(pDX, IDC_EDB_CITIES_REGION, m_edbRegion);
	DDX_Control(pDX, IDC_STT_CITIES_REGION, m_sttRegion);
}


// MFC Message Handlers
// ----------------

BOOL CCitiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_edbName.SetLimitText(CITY_NAME_SIZE);
	m_edbRegion.SetLimitText(CITY_REGION_SIZE);

	m_edbName.EnableWindow(TRUE);
	m_edbRegion.EnableWindow(TRUE);
	m_edbName.SetWindowText(m_pCity->szName);
	m_edbRegion.SetWindowText(m_pCity->szRegion);

	UpdateState();

	return TRUE;
}


void CCitiesDialog::OnOK()
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

void CCitiesDialog::UpdateState()
{
	if (m_eOperation == Operations::OperationSelect)
	{
		m_edbName.EnableWindow(FALSE);
		m_edbRegion.EnableWindow(FALSE);
	}
	else if (m_eOperation == Operations::OperationInsert)
	{
		m_edbName.SetWindowText(EMPTY);
		m_edbRegion.SetWindowText(EMPTY);
	}
}

BOOL CCitiesDialog::SaveChanges()
{
	const int iResult = MessageBox(_T("Are you sure you want to save changes?"), NULL, MB_YESNO);
	if (iResult != IDYES)
	{
		return FALSE;
	}

	CString strWindowText;
	m_edbName.GetWindowText(strWindowText);
	lstrcpy(m_pCity->szName, strWindowText);
	m_edbRegion.GetWindowText(strWindowText);
	lstrcpy(m_pCity->szRegion, strWindowText);

	return TRUE;
}