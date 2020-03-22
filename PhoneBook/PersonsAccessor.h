#pragma once
#include <atldbcli.h>
#include <iostream>
#include "Structures.h"

#define NUMBER_OF_ACCESSORS 2
#define READONLY_ACCESSOR 0
#define READ_AND_WRITE_ACCESSOR 1
#define ID_COL 1
#define UPDATE_COUNTER_COL 2
#define FIRST_NAME_COL 3
#define MIDDLE_NAME_COL 4
#define LAST_NAME_COL 5
#define EGN_COL 6
#define CITY_ID_COL 7
#define ADDRESS_COL 8

class CPersonsAccessor
{
protected:
	PERSONS m_recPersons;

	BEGIN_ACCESSOR_MAP(CPersonsAccessor, NUMBER_OF_ACCESSORS)
		BEGIN_ACCESSOR(READONLY_ACCESSOR, true)
		COLUMN_ENTRY(ID_COL, m_recPersons.nID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(READ_AND_WRITE_ACCESSOR, true)
		COLUMN_ENTRY(UPDATE_COUNTER_COL, m_recPersons.nUpdateCounter)
		COLUMN_ENTRY(FIRST_NAME_COL, m_recPersons.szFirstName)
		COLUMN_ENTRY(MIDDLE_NAME_COL, m_recPersons.szMiddleName)
		COLUMN_ENTRY(LAST_NAME_COL, m_recPersons.szLastName)
		COLUMN_ENTRY(EGN_COL, m_recPersons.szEGN)
		COLUMN_ENTRY(CITY_ID_COL, m_recPersons.nCityID)
		COLUMN_ENTRY(ADDRESS_COL, m_recPersons.szAddress)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};