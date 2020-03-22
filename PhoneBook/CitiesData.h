#pragma once
#include "Structures.h"
#include "CitiesTable.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesData

class CCitiesData
{

// Constructor / Destructor
//---------------------
public:
	CCitiesData();
	virtual ~CCitiesData();

// Methods
//---------------------
public:
	BOOL SelectAll(CCitiesArray& oCitiesArray);
	BOOL SelectWhereID(const int nID, CITIES& recCities);
	BOOL UpdateWhereID(const int nID, CITIES& recCities);
	BOOL Insert(CITIES& recCities, int& nID);
	BOOL DeleteWhereID(const int nID);

// Members
//---------------------
private:
	CCitiesTable m_oCitiesTable;
};

