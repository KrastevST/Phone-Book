#pragma once

#include "PhoneBook.h"
#include "afxdialogex.h"
#include "Structures.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDialog


class CCitiesDialog : public CDialog
{
// Macros
// ----------------
	DECLARE_DYNAMIC(CCitiesDialog)
	DECLARE_MESSAGE_MAP();

// Constants
// ----------------
public:
	enum Operations{ OperationSelect, OperationEdit, OperationInsert};
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CITIES_DIALOG };
#endif

// Constructor / Destructor
// ----------------
public:
	CCitiesDialog(Operations eOperation, CITIES* pCity, CWnd* pParent = NULL);
	virtual ~CCitiesDialog();

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
	CEdit m_edbName;
	CStatic m_sttName;
	CEdit m_edbRegion;
	CStatic m_sttRegion;
	Operations m_eOperation;
	CITIES* m_pCity;
};
