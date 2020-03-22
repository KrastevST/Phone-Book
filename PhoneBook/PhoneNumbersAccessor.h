#pragma once
#include <atldbcli.h>
#include <iostream>
#include "Structures.h"

#define NUMBER_OF_ACCESSORS 2
#define READONLY_ACCESSOR 0
#define READ_AND_WRITE_ACCESSOR 1
#define ID_COL 1
#define UPDATE_COUNTER_COL 2
#define PERSON_ID_COL 3
#define PHONE_TYPE_ID_COL 4
#define PHONE_NUMBER_COL 5

class CPhoneNumbersAccessor
{
protected:
	PHONE_NUMBERS m_recPhoneNumbers;

	BEGIN_ACCESSOR_MAP(CPhoneNumbersAccessor, NUMBER_OF_ACCESSORS)
		BEGIN_ACCESSOR(READONLY_ACCESSOR, true)
		COLUMN_ENTRY(ID_COL, m_recPhoneNumbers.nID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(READ_AND_WRITE_ACCESSOR, true)
		COLUMN_ENTRY(UPDATE_COUNTER_COL, m_recPhoneNumbers.nUpdateCounter)
		COLUMN_ENTRY(PERSON_ID_COL, m_recPhoneNumbers.nPersonID)
		COLUMN_ENTRY(PHONE_TYPE_ID_COL, m_recPhoneNumbers.nPhoneTypeID)
		COLUMN_ENTRY(PHONE_NUMBER_COL, m_recPhoneNumbers.szPhoneNumber)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};