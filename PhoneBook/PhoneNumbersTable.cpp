#include "pch.h"
#include "PhoneNumbersTable.h"
#include "PersonWithPhoneNums.h"


// Constructor / Destructor
//---------------------
CPhoneNumbersTable::CPhoneNumbersTable()
	: CTemplateTable(_T("PHONE_NUMBERS"), offsetof(PHONE_NUMBERS, nUpdateCounter))
{
}

CPhoneNumbersTable::~CPhoneNumbersTable()
{
}


// Methods
//---------------------

BOOL CPhoneNumbersTable::SelectWherePersonID(int nPersonID, CPhoneNumbersArray& oPhoneNumbersArray)
{
	OpenSession();

	CString strQuery{};
	strQuery.Format(_T("SELECT * FROM PHONE_NUMBERS WHERE PERSON_ID = %i"), nPersonID);
	BOOL bResult = OpenDBRecord(SELECT, strQuery);
	if (!bResult)
	{
		return FALSE;
	}

	m_hResult = MoveNext();
	if (FAILED(m_hResult))
	{
		CString strErrorMessage("Error reading from database");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	oPhoneNumbersArray.RemoveAll();
	do
	{
		oPhoneNumbersArray.Add(new PHONE_NUMBERS(m_recPhoneNumbers));
	} while (MoveNext() != DB_S_ENDOFROWSET);

	CloseSession();

	return TRUE;
}

BOOL CPhoneNumbersTable::DeleteWherePersonID(int nPersonID)
{
	BOOL bResult = OpenSession();
	if (!bResult)
	{
		return FALSE;
	}

	CPhoneNumbersArray oPhoneNumbersArray;
	SelectWherePersonID(nPersonID, oPhoneNumbersArray);

	for (INT_PTR i = 0; i < oPhoneNumbersArray.GetCount(); i++)
	{
		int nPhoneNumberID = oPhoneNumbersArray.GetAt(i)->nID;
		BOOL bResult = DeleteWhereID(nPhoneNumberID);
		if (!bResult)
		{
			return FALSE;
		}
	}

	CloseSession();
	return TRUE;
}

BOOL CPhoneNumbersTable::Insert(PHONE_NUMBERS& recPhoneNumber)
{
	int nID;
	return __super::Insert(recPhoneNumber, nID);
}


// Overrides
//---------------------

PHONE_NUMBERS& CPhoneNumbersTable::GetRecord()
{
	return m_recPhoneNumbers;
}