#pragma once
#include "Structures.h"
#include "PersonsTable.h"
#include "PhoneNumbersTable.h"
#include "PersonWithPhoneNums.h"


/////////////////////////////////////////////////////////////////////////////
// CPersonsData

class CPersonsData
{

	// Constructor / Destructor
	//---------------------
public:
	CPersonsData();
	virtual ~CPersonsData();

	// Methods
	//---------------------
public:
	BOOL SelectAll(CPersonsArray& oPersonsArray);
	BOOL SelectWhereID(const int nID, PERSONS& recPerson);
	BOOL UpdateWhereID(const int nID, CPersonWithPhoneNums& recCPersonWithPhoneNums);
	BOOL UpdateWhereID(const int nID, PERSONS& recPERSONS);
	BOOL Insert(CPersonWithPhoneNums& recCPersonWithPhoneNums, int& nID);
	BOOL Insert(PERSONS& recPERSONS, int& nID);
	BOOL DeleteWhereID(const int nID);
	BOOL AddPhoneNumsToPerson(CPersonWithPhoneNums& oPersonWithPhoneNumbers);

private:
	BOOL OpenSession();
	BOOL BeginTransaction();
	BOOL CommitTransaction();

	// Members
	//---------------------
private:
	CPersonsTable m_oPersonsTable;
	CPhoneNumbersTable m_oPhoneNumbersTable;
	CSession m_oSession;
};



