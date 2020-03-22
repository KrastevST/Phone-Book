#include "pch.h"
#include "PersonWithPhoneNums.h"

//////////////////////////////////////////////////////////////////////////////////
// CPersonWithPhoneNums


// Constructor / Destructor
//---------------------

CPersonWithPhoneNums::CPersonWithPhoneNums(PERSONS& recPerson)
	: m_Person(recPerson)
{
}
CPersonWithPhoneNums::~CPersonWithPhoneNums()
{
}


// Methods
//---------------------

void CPersonWithPhoneNums::AddPhoneNumber(PHONE_NUMBERS& recPhoneNumbers)
{
	m_oPhoneNumbersArray.Add(new PHONE_NUMBERS(recPhoneNumbers));
}

PERSONS& CPersonWithPhoneNums::GetPerson()
{
	return m_Person;
}

CPhoneNumbersArray& CPersonWithPhoneNums::GetPhoneNumbers()
{
	return m_oPhoneNumbersArray;
}