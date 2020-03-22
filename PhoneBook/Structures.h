#pragma once
#include <afxtempl.h>
#include "AutoReleaseTypedPtrArray.h"

#define PHONE_NUMBERS_PHONE_NUMBER_SIZE 16
#define PHONE_TYPES_PHONE_TYPE_SIZE 128
#define PERSON_FIRST_NAME_SIZE 128
#define PERSON_MIDDLE_NAME_SIZE 128
#define PERSON_LAST_NAME_SIZE 128
#define PERSON_EGN_SIZE 16
#define PERSON_ADDRESS_SIZE 256
#define CITY_NAME_SIZE 128
#define CITY_REGION_SIZE 128

struct PHONE_NUMBERS 
{
	int nID;
	int nUpdateCounter;
	int nPersonID;
	int nPhoneTypeID;
	TCHAR szPhoneNumber[PHONE_NUMBERS_PHONE_NUMBER_SIZE];

	PHONE_NUMBERS() 
	{
		SecureZeroMemory(this, sizeof(this));
	}
};

struct PHONE_TYPES
{
	int nID;
	int nUpdateCounter;
	TCHAR szPhoneType[PHONE_TYPES_PHONE_TYPE_SIZE];

	PHONE_TYPES() 
	{
		SecureZeroMemory(this, sizeof(this));
	}
};

struct PERSONS
{
	int nID;
	int nUpdateCounter;
	TCHAR szFirstName[PERSON_FIRST_NAME_SIZE];
	TCHAR szMiddleName[PERSON_MIDDLE_NAME_SIZE];
	TCHAR szLastName[PERSON_LAST_NAME_SIZE];
	int nCityID;
	TCHAR szEGN[PERSON_EGN_SIZE];
	TCHAR szAddress[PERSON_ADDRESS_SIZE];

	PERSONS() 
	{
		SecureZeroMemory(this, sizeof(this));
	}
};

struct CITIES
{
	int nID;
	int nUpdateCounter;
	TCHAR szName[CITY_NAME_SIZE];
	TCHAR szRegion[CITY_REGION_SIZE];

	CITIES()
	{
		SecureZeroMemory(this, sizeof(this));
	}
};

typedef CAutoReleaseTypedPtrArray<CPtrArray, CITIES*> CCitiesArray;
typedef CAutoReleaseTypedPtrArray<CPtrArray, PERSONS*> CPersonsArray;
typedef CAutoReleaseTypedPtrArray<CPtrArray, PHONE_TYPES*> CPhoneTypesArray;
typedef CAutoReleaseTypedPtrArray<CPtrArray, PHONE_NUMBERS*> CPhoneNumbersArray;
typedef CMap<int, int, CITIES*, CITIES*> CCitiesMap;
typedef CMap<int, int, PHONE_TYPES*, PHONE_TYPES*> CPhoneTypesMap;