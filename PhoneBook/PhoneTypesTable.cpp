#include "pch.h"
#include "PhoneTypesTable.h"


// Constructor / Destructor
//---------------------
CPhoneTypesTable::CPhoneTypesTable()
	: CTemplateTable(_T("PHONE_TYPES"), offsetof(PHONE_TYPES, nUpdateCounter))
{
}

CPhoneTypesTable::~CPhoneTypesTable()
{
}


// Overrides
//---------------------

PHONE_TYPES& CPhoneTypesTable::GetRecord()
{
	return m_recPhoneTypes;
}
