#pragma once
#include "Structures.h"
#include "PersonsAccessor.h"
#include "TemplateTable.h"
#include "PersonWithPhoneNums.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsTable

class CPersonsTable : public CTemplateTable<PERSONS, CPersonsAccessor>
{

// Constructor / Destructor
//---------------------
public:
	CPersonsTable();
	virtual ~CPersonsTable();

// Overrides
//---------------------
private:
	PERSONS& GetRecord() override;
};