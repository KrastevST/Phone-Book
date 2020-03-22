#pragma once
#include "pch.h"
#include "PersonsDocument.h"
#include "Operations.h"


/////////////////////////////////////////////////////////////////////////////
// CPersonsDocument

// Macros
// ----------------

IMPLEMENT_DYNCREATE(CPersonsDocument, CDocument)

BEGIN_MESSAGE_MAP(CPersonsDocument, CDocument)
END_MESSAGE_MAP()


// Constructor / Destructor
// ----------------

CPersonsDocument::CPersonsDocument() noexcept
{
}
CPersonsDocument::~CPersonsDocument()
{
}


// Methods
// ----------------

BOOL CPersonsDocument::SelectWhereID(const int nID, PERSONS& recPerson)
{
	return __super::SelectWhereID(nID, recPerson);
}

BOOL CPersonsDocument::UpdateWhereID(const int nPersonID, CPersonWithPhoneNums& recPersonWithPhoneNums)
{
	BOOL bResult = m_oPersonsData.UpdateWhereID(nPersonID, recPersonWithPhoneNums);
	if (!bResult)
	{
		return FALSE;
	}

	bResult = FALSE;
	PERSONS& recPerson = recPersonWithPhoneNums.GetPerson();
	for (INT_PTR i = 0; i < GetPersonsArray().GetCount(); i++)
	{
		int nCurrentID = GetPersonsArray().GetAt(i)->nID;
		if (nCurrentID = nPersonID)
		{
			*GetPersonsArray().GetAt(i) = recPerson;
			bResult = TRUE;
		}
	}

	if (!bResult)
	{
		CString strMessage;
		strMessage.Format(_T("Unable to find record in local store."));
		AfxMessageBox(strMessage);
		return FALSE;
	}

	CObjectWrapper<PERSONS>* pObjectWrapper = new CObjectWrapper<PERSONS>(recPerson);
	UpdateAllViews(NULL, OPERATION_EDIT, pObjectWrapper);

	return TRUE;
}

BOOL CPersonsDocument::Insert(CPersonWithPhoneNums& recPersonWithPhoneNums)
{
	int nID;
	BOOL bResult = m_oPersonsData.Insert(recPersonWithPhoneNums, nID);
	if (!bResult)
	{
		return FALSE;
	}

	PERSONS& Person = recPersonWithPhoneNums.GetPerson();
	Person.nID = nID;
	GetPersonsArray().Add(new PERSONS(Person));

	CObjectWrapper<PERSONS>* pObjectWrapper = new CObjectWrapper<PERSONS>(Person);
	UpdateAllViews(NULL, OPERATION_INSERT, pObjectWrapper);

	return TRUE;
}

BOOL CPersonsDocument::AddPhoneNumsToPerson(CPersonWithPhoneNums& oPersonWithPhoneNumbers)
{
	return m_oPersonsData.AddPhoneNumsToPerson(oPersonWithPhoneNumbers);
}

CPersonsArray& CPersonsDocument::GetPersonsArray()
{
	return m_oAllTableRecordsArray;
}

CCitiesMap& CPersonsDocument::GetCitiesMap()
{
	return m_oCitiesMap;
}

CPhoneTypesMap& CPersonsDocument::GetPhoneTypesMap()
{
	return m_oPhoneTypesMap;
}


// Overrides
// ----------------

BOOL CPersonsDocument::SelectAll()
{
	BOOL bResult = __super::SelectAll();
	if (!bResult)
	{
		return FALSE;
	}

	bResult = m_oCitiesData.SelectAll(m_oCitiesArray);
	if (!bResult)
	{
		return FALSE;
	}

	for (INT_PTR i = 0; i < m_oCitiesArray.GetCount(); i++)
	{
		m_oCitiesMap.SetAt(m_oCitiesArray.GetAt(i)->nID, m_oCitiesArray.GetAt(i));
	}

	bResult = m_oPhoneTypesData.SelectAll(m_oPhoneTypesArray);
	if (!bResult)
	{
		return FALSE;
	}

	for (INT_PTR i = 0; i < m_oPhoneTypesArray.GetCount(); i++)
	{
		m_oPhoneTypesMap.SetAt(m_oPhoneTypesArray.GetAt(i)->nID, m_oPhoneTypesArray.GetAt(i));
	}

	return TRUE;
}

BOOL CPersonsDocument::DeleteWhereID(const int nID)
{
	BOOL bResult = m_oPersonsData.DeleteWhereID(nID);
	if (!bResult)
	{
		return FALSE;
	}

	bResult = FALSE;
	for (INT_PTR i = 0; i < GetPersonsArray().GetCount(); i++)
	{
		if (GetPersonsArray().GetAt(i)->nID = nID)
		{
			GetPersonsArray().RemoveAt(i);
			bResult = TRUE;;
		}
	}

	if (!bResult)
	{
		CString strMessage;
		strMessage.Format(_T("Unable to find record in local store."));
		AfxMessageBox(strMessage);
	}

	UpdateAllViews(NULL, OPERATION_DELETE);
	
	return TRUE;
}