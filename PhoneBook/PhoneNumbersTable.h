#pragma once
#include "Structures.h"
#include "PhoneNumbersAccessor.h"
#include "TemplateTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneNmbersTable

class CPhoneNumbersTable : public CTemplateTable<PHONE_NUMBERS, CPhoneNumbersAccessor>
{

// Constructor / Destructor
//---------------------
public:
	CPhoneNumbersTable();
	virtual ~CPhoneNumbersTable();

// Methods
//---------------------
public:
	BOOL SelectWherePersonID(int nID, CPhoneNumbersArray& oPhoneNumbersArray);
	BOOL DeleteWherePersonID(int nPersonID);
	BOOL Insert(PHONE_NUMBERS& recPhoneNumber);

// Overrides
//---------------------
private:
	PHONE_NUMBERS& GetRecord() override;
};