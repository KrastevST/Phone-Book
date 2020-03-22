#pragma once

#include "PhoneBook.h"
#include "afxdialogex.h"
#include "Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNumbersDialog

class CPhoneNumbersDialog : public CDialog
{

// Macros
// ----------------
	DECLARE_DYNAMIC(CPhoneNumbersDialog)
	DECLARE_MESSAGE_MAP();

// Constants
// ----------------
public:
	enum Operations { OperationSelect, OperationEdit, OperationInsert };
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_NUMBERS_DIALOG };
#endif

// Constructor / Destructor
// ----------------
public:
	CPhoneNumbersDialog(Operations eOperation, PHONE_NUMBERS& pPhoneNumber, CPhoneTypesMap& oPhoneTypesMap, CWnd* pParent = NULL);
	virtual ~CPhoneNumbersDialog();

// MFC Overrides
// ----------------
private:
	void DoDataExchange(CDataExchange* pDX) override;

// Overrides
// ----------------
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();

// Methods
// ----------------
public:
	void UpdateState();
	BOOL SaveChanges();

// Members
// ----------------
private:
	CEdit m_edbPhoneNmber;
	Operations m_eOperation;
	PHONE_NUMBERS& m_recPhoneNumber;
	CPhoneTypesMap& m_PhoneTypesMap;
	CEdit m_edbPhoneNumber;
	CComboBox m_cmbPhoneType;
};


