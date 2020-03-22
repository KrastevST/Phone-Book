#pragma once
#include "Structures.h"
#include "PersonsData.h"
#include "TemplateDocument.h"
#include "CitiesData.h"
#include "PhoneTypesData.h"

/////////////////////////////////////////////////////////////////////////////
// CPersonsDocument

class CPersonsDocument : public CTemplateDocument<CPersonsData, PERSONS>
{

// Macros
// ----------------
protected:
	DECLARE_DYNCREATE(CPersonsDocument)
	DECLARE_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
public:
	CPersonsDocument() noexcept;
	virtual ~CPersonsDocument();

// Methods
// ----------------
public:
	BOOL SelectWhereID(const int nID, PERSONS& recPerson);
	BOOL UpdateWhereID(const int nID, CPersonWithPhoneNums& recPersonWithPhoneNums);
	BOOL Insert(CPersonWithPhoneNums& recPersonWithPhoneNums);
	BOOL AddPhoneNumsToPerson(CPersonWithPhoneNums& oPersonWithPhoneNumbers);
	CPersonsArray& GetPersonsArray();
	CCitiesMap& GetCitiesMap();
	CPhoneTypesMap& GetPhoneTypesMap();

// Overrides
// ----------------
public:
	BOOL SelectAll() override;
	BOOL DeleteWhereID(const int nID) override;

// Members
// ----------------
private:
	CPersonsData m_oPersonsData;
	CCitiesData m_oCitiesData;
	CPhoneTypesData m_oPhoneTypesData;
	CCitiesArray m_oCitiesArray;
	CPhoneTypesArray m_oPhoneTypesArray;
	CCitiesMap m_oCitiesMap;
	CPhoneTypesMap m_oPhoneTypesMap;
};

