#pragma once

#include "PhoneBook.h"
#include "afxdialogex.h"
#include "Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDialog

class CPhoneTypesDialog : public CDialog
{

// Macros
// ----------------
	DECLARE_DYNAMIC(CPhoneTypesDialog)
	DECLARE_MESSAGE_MAP();

// Constants
// ----------------
public:
	enum Operations { OperationSelect, OperationEdit, OperationInsert };
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHONE_TYPES_DIALOG };
#endif

// Constructor / Destructor
// ----------------
public:
	CPhoneTypesDialog(Operations eOperation, PHONE_TYPES& pPhoneType, CWnd* pParent = NULL);
	virtual ~CPhoneTypesDialog();

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
	CEdit m_edbPhoneType;
	Operations m_eOperation;
	PHONE_TYPES& m_PhoneType;
};
