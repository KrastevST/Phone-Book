#include "pch.h"
#include "PhoneTypesData.h"

//////////////////////////////////////////////////////////////////////////////////
// CPhoneTypesData

// Constructor / Destructor
//---------------------
CPhoneTypesData::CPhoneTypesData()
{
}
CPhoneTypesData::~CPhoneTypesData()
{
}


// Methods
//---------------------
BOOL CPhoneTypesData::SelectAll(CPhoneTypesArray& oPhoneTypesArray)
{
	return m_oPhoneTypesTable.SelectAll(oPhoneTypesArray);
}

BOOL CPhoneTypesData::SelectWhereID(const int nID, PHONE_TYPES& recPhoneTypes)
{
	return m_oPhoneTypesTable.SelectWhereID(nID, recPhoneTypes);
}

BOOL CPhoneTypesData::UpdateWhereID(const int nID, PHONE_TYPES& recPhoneTypes)
{
	return m_oPhoneTypesTable.UpdateWhereID(nID, recPhoneTypes);
}

BOOL CPhoneTypesData::Insert(PHONE_TYPES& recPhoneTypes, int& nID)
{
	return m_oPhoneTypesTable.Insert(recPhoneTypes, nID);
}

BOOL CPhoneTypesData::DeleteWhereID(const int nID)
{
	return m_oPhoneTypesTable.DeleteWhereID(nID);
}