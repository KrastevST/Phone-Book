#pragma once
#include "Structures.h"
#include "PhoneTypesAccessor.h"
#include "TemplateTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

class CPhoneTypesTable : public CTemplateTable<PHONE_TYPES, CPhoneTypesAccessor>
{

// Constructor / Destructor
//---------------------
public:
	CPhoneTypesTable();
	virtual ~CPhoneTypesTable();

// Overrides
//---------------------
private:
	PHONE_TYPES& GetRecord() override;
};