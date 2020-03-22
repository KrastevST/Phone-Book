#pragma once
#include "Structures.h"
#include "PhoneTypesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesData

class CPhoneTypesData
{

// Constructor / Destructor
//---------------------
public:
	CPhoneTypesData();
	virtual ~CPhoneTypesData();

// Methods
//---------------------
public:
	BOOL SelectAll(CPhoneTypesArray& oPhoneTypesArray);
	BOOL SelectWhereID(const int nID, PHONE_TYPES& recPhoneTypes);
	BOOL UpdateWhereID(const int nID, PHONE_TYPES& recPhoneTypes);
	BOOL Insert(PHONE_TYPES& recPhoneTypes, int& nID);
	BOOL DeleteWhereID(const int nID);

// Members
//---------------------
private:
	CPhoneTypesTable m_oPhoneTypesTable;
};

