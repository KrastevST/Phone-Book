#include "pch.h"
#include "CitiesData.h"

//////////////////////////////////////////////////////////////////////////////////
// CCitiesData

// Constructor / Destructor
//---------------------
CCitiesData::CCitiesData()
{
}
CCitiesData::~CCitiesData()
{
}


// Methods
//---------------------
BOOL CCitiesData::SelectAll(CCitiesArray& oCitiesArray)
{
	return m_oCitiesTable.SelectAll(oCitiesArray);
}

BOOL CCitiesData::SelectWhereID(const int nID, CITIES& recCities)
{
	return m_oCitiesTable.SelectWhereID(nID, recCities);
}

BOOL CCitiesData::UpdateWhereID(const int nID, CITIES& recCities)
{
	return m_oCitiesTable.UpdateWhereID(nID, recCities);
}

BOOL CCitiesData::Insert(CITIES& recCities, int& nID)
{
	return m_oCitiesTable.Insert(recCities, nID);
}

BOOL CCitiesData::DeleteWhereID(const int nID)
{
	return m_oCitiesTable.DeleteWhereID(nID);
}