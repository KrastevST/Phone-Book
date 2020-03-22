#include "pch.h"
#include "CitiesTable.h"

// Constructor / Destructor
//---------------------
CCitiesTable::CCitiesTable()
	: CTemplateTable(_T("CITIES"), offsetof(CITIES, nUpdateCounter))
{
}

CCitiesTable::~CCitiesTable()
{
}

// Overrides
//---------------------
CITIES& CCitiesTable::GetRecord()
{
	return m_recCities;
}