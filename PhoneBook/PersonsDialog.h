#pragma once

#include "PhoneBook.h"
#include "afxdialogex.h"
#include "Structures.h"
#include "PersonWithPhoneNums.h"
#include "CitiesData.h"
#include "PhoneTypesData.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsDialog

class CPersonsDialog : public CDialog
{
// Macros
// ----------------
	DECLARE_DYNAMIC(CPersonsDialog)
	DECLARE_MESSAGE_MAP();

// Constants
// ----------------
public:
	enum Operations { OperationSelect, OperationEdit, OperationInsert };
	enum Columns { ColumnPhoneNumber, ColumnPhoneType };
	enum { IDD = IDD_PERSONS_DIALOG };

// Constructor / Destructor
// ----------------
public:
	CPersonsDialog(Operations eOperation, CPersonWithPhoneNums& oPersonWithPhoneNums, CCitiesMap& oCitiesMap, CPhoneTypesMap& oPhoneTypesMap, CWnd* pParent = NULL);
	virtual ~CPersonsDialog();

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
	afx_msg void OnContextMenuSelect();
	afx_msg void OnContextMenuEdit();
	afx_msg void OnContextMenuInsert();
	afx_msg void OnContextMenuDelete();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	void UpdateState();
	BOOL SaveChanges();
	BOOL GetSelectedRecordID(int nID);

// Members
// ----------------
private:
	Operations m_eOperation;
	CPersonWithPhoneNums& m_PersonWithPhoneNums;
	CCitiesMap& m_CitiesMap;
	CPhoneTypesMap& m_PhoneTypesMap;
	CEdit m_edbFirstName;
	CEdit m_edbMiddleName;
	CEdit m_edbLastName;
	CEdit m_edbEGN;
	CEdit m_edbAddress;
	CListCtrl m_lscPhoneNumbers;
	CComboBox m_cmbCity;
};
