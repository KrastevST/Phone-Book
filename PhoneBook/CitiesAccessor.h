#pragma once
#include <atldbcli.h>
#include <iostream>
#include "Structures.h"

#define NUMBER_OF_ACCESSORS 2
#define READONLY_ACCESSOR 0
#define READ_AND_WRITE_ACCESSOR 1
#define ID_COL 1
#define UPDATE_COUNTER_COL 2
#define NAME_COL 3
#define REGION_COL 4

class CCitiesAccessor
{

protected:
	CITIES m_recCities;

	BEGIN_ACCESSOR_MAP(CCitiesAccessor, NUMBER_OF_ACCESSORS)
		BEGIN_ACCESSOR(READONLY_ACCESSOR, true)
		COLUMN_ENTRY(ID_COL, m_recCities.nID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(READ_AND_WRITE_ACCESSOR, true)
		COLUMN_ENTRY(UPDATE_COUNTER_COL, m_recCities.nUpdateCounter)
		COLUMN_ENTRY(NAME_COL, m_recCities.szName)
		COLUMN_ENTRY(REGION_COL, m_recCities.szRegion)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};