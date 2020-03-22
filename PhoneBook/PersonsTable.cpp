#include "pch.h"
#include "PersonsTable.h"


// Constructor / Destructor
//---------------------
CPersonsTable::CPersonsTable()
	: CTemplateTable(_T("PERSONS"), offsetof(PERSONS, nUpdateCounter))
{
}

CPersonsTable::~CPersonsTable()
{
}

// Overrides
//---------------------
PERSONS& CPersonsTable::GetRecord()
{
	return m_recPersons;
}
