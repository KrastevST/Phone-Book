#pragma once
#include "Structures.h"
#include "CitiesAccessor.h"
#include "TemplateTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesTable

class CCitiesTable : public CTemplateTable<CITIES, CCitiesAccessor>
{

// Constructor / Destructor
//---------------------
public:
	CCitiesTable();
	virtual ~CCitiesTable();

// Overrrides
//---------------------
private:
	CITIES& GetRecord() override;
};