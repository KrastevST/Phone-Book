#pragma once
#include "Structures.h"

//////////////////////////////////////////////////////////////////////////////////
// CPersonWithPhoneNums

class CPersonWithPhoneNums
{

// Constructor / Destructor
//---------------------
public:
	CPersonWithPhoneNums(PERSONS& recPerson);
	virtual ~CPersonWithPhoneNums();

// Methods
//---------------------
public:
	void AddPhoneNumber(PHONE_NUMBERS& recPhoneNumbers);
	PERSONS& GetPerson();
	CPhoneNumbersArray& GetPhoneNumbers();

// Members
//---------------------
private:
	PERSONS& m_Person;
	CPhoneNumbersArray m_oPhoneNumbersArray{};
};




