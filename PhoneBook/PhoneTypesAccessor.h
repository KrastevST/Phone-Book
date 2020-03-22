#pragma once
#include <atldbcli.h>
#include <iostream>
#include "Structures.h"

#define NUMBER_OF_ACCESSORS 2
#define READONLY_ACCESSOR 0
#define READ_AND_WRITE_ACCESSOR 1
#define ID_COL 1
#define UPDATE_COUNTER_COL 2
#define PHONE_TYPE_COL 3


class CPhoneTypesAccessor
{
protected:
	PHONE_TYPES m_recPhoneTypes;

	BEGIN_ACCESSOR_MAP(CPhoneTypesAccessor, NUMBER_OF_ACCESSORS)
		BEGIN_ACCESSOR(READONLY_ACCESSOR, true)
		COLUMN_ENTRY(ID_COL, m_recPhoneTypes.nID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(READ_AND_WRITE_ACCESSOR, true)
		COLUMN_ENTRY(UPDATE_COUNTER_COL, m_recPhoneTypes.nUpdateCounter)
		COLUMN_ENTRY(PHONE_TYPE_COL, m_recPhoneTypes.szPhoneType)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};