#include "pch.h"
#include "PersonsData.h"

//////////////////////////////////////////////////////////////////////////////////
// CPersonsData

// Constructor / Destructor
//---------------------
CPersonsData::CPersonsData()
{
}
CPersonsData::~CPersonsData()
{
}


// Methods
//---------------------
BOOL CPersonsData::SelectAll(CPersonsArray& oPersonsArray)
{
	return  m_oPersonsTable.SelectAll(oPersonsArray);
}

BOOL CPersonsData::SelectWhereID(const int nID, PERSONS& recPerson)
{
	return m_oPersonsTable.SelectWhereID(nID, recPerson);
}

BOOL CPersonsData::UpdateWhereID(const int nID, CPersonWithPhoneNums& recCPersonWithPhoneNums)
{
	OpenSession();
	BOOL bResult = BeginTransaction();
	if (!bResult)
	{
		return FALSE;
	}

	m_oPersonsTable.SetExternalSession(TRUE, &m_oSession);
	bResult = m_oPersonsTable.UpdateWhereID(nID, recCPersonWithPhoneNums.GetPerson());
	if (!bResult)
	{
		m_oSession.Abort();
		m_oSession.Close();
		return FALSE;
	}

	m_oPhoneNumbersTable.SetExternalSession(TRUE, &m_oSession);
	CPhoneNumbersArray oOldNumbersArray;
	CPhoneNumbersArray& NewNumbersArray = recCPersonWithPhoneNums.GetPhoneNumbers();

	m_oPhoneNumbersTable.SelectWherePersonID(nID, oOldNumbersArray);

	for (INT_PTR i = 0; i < NewNumbersArray.GetCount(); i++)
	{
		NewNumbersArray.GetAt(i)->nPersonID = nID;

		for (INT_PTR j = 0; j < oOldNumbersArray.GetCount(); j++)
		{
			int nOldId = oOldNumbersArray.GetAt(j)->nID;
			int nNewId = NewNumbersArray.GetAt(i)->nID;
			if (nNewId == 0)
			{
				bResult = m_oPhoneNumbersTable.Insert(*NewNumbersArray.GetAt(i));
				if (!bResult)
				{
					m_oSession.Abort();
					m_oSession.Close();
					return FALSE;
				}
			}
			else if (nNewId == nOldId)
			{
				bResult = m_oPhoneNumbersTable.UpdateWhereID(nNewId, *NewNumbersArray.GetAt(i));
				if (!bResult)
				{
					m_oSession.Abort();
					m_oSession.Close();
					return FALSE;
				}

				oOldNumbersArray.RemoveAt(j);
			}
		}
	}

	for (INT_PTR i = 0; i < oOldNumbersArray.GetCount(); i++)
	{
		int nPhoneNumberID = oOldNumbersArray.GetAt(i)->nID;
		bResult = m_oPhoneNumbersTable.DeleteWhereID(nPhoneNumberID);
		if (!bResult)
		{
			m_oSession.Abort();
			m_oSession.Close();
			return FALSE;
		}
	}

	bResult = CommitTransaction();
	if (!bResult)
	{
		return FALSE;
	}

	m_oSession.Close();
	return TRUE;
}

BOOL CPersonsData::UpdateWhereID(const int nID, PERSONS& recPERSONS)
{
	return m_oPersonsTable.UpdateWhereID(nID, recPERSONS);
}

BOOL CPersonsData::Insert(CPersonWithPhoneNums& recCPersonWithPhoneNums, int& nID)
{
	OpenSession();
	BOOL bResult = BeginTransaction();
	if (!bResult)
	{
		return FALSE;
	}

	m_oPersonsTable.SetExternalSession(TRUE, &m_oSession);
	bResult = m_oPersonsTable.Insert(recCPersonWithPhoneNums.GetPerson(), nID);
	if (!bResult)
	{
		m_oSession.Abort();
		m_oSession.Close();
		return FALSE;
	}

	m_oPhoneNumbersTable.SetExternalSession(TRUE, &m_oSession);
	for (INT_PTR i = 0; i < recCPersonWithPhoneNums.GetPhoneNumbers().GetCount(); i++)
	{
		PHONE_NUMBERS& PhoneNumber = *recCPersonWithPhoneNums.GetPhoneNumbers().GetAt(i);
		bResult = m_oPhoneNumbersTable.Insert(PhoneNumber);
		if (!bResult)
		{
			m_oSession.Abort();
			m_oSession.Close();
			return FALSE;
		}
	}

	bResult = CommitTransaction();
	if (!bResult)
	{
		return FALSE;
	}

	m_oSession.Close();
	return TRUE;
}

BOOL CPersonsData::Insert(PERSONS& recPERSONS, int& nID)
{
	return m_oPersonsTable.Insert(recPERSONS, nID);
}

BOOL CPersonsData::DeleteWhereID(const int nID)
{
	OpenSession();
	BOOL bResult = BeginTransaction();
	if (!bResult)
	{
		return FALSE;
	}

	m_oPhoneNumbersTable.SetExternalSession(TRUE, &m_oSession);
	bResult = m_oPhoneNumbersTable.DeleteWherePersonID(nID);
	if (!bResult)
	{
		m_oSession.Abort();
		m_oSession.Close();
		return FALSE;
	}

	m_oPersonsTable.SetExternalSession(TRUE, &m_oSession);
	bResult = m_oPersonsTable.DeleteWhereID(nID);
	if (!bResult)
	{
		m_oSession.Abort();
		m_oSession.Close();
		return FALSE;
	}

	bResult = CommitTransaction();
	if (!bResult)
	{
		return FALSE;
	}

	m_oSession.Close();
	return TRUE;
}

BOOL CPersonsData::AddPhoneNumsToPerson(CPersonWithPhoneNums& oPersonWithPhoneNumbers)
{
	int nID = oPersonWithPhoneNumbers.GetPerson().nID;
	return m_oPhoneNumbersTable.SelectWherePersonID(nID, oPersonWithPhoneNumbers.GetPhoneNumbers());
}

BOOL CPersonsData::OpenSession()
{
	CDataSource& DataSource = CDBConnector::GetInstance()->GetDataSource();
	HRESULT hResult = m_oSession.Open(DataSource);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session. Error: %x"), hResult);
		AfxMessageBox(strMessage);

		return FALSE;
	}

	return TRUE;
}

BOOL CPersonsData::BeginTransaction()
{
	HRESULT hResult = m_oSession.StartTransaction();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to start transaction. Error: %x"), hResult);
		AfxMessageBox(strMessage);
		return FALSE;
	}
}

BOOL CPersonsData::CommitTransaction()
{
	HRESULT hResult = m_oSession.Commit();
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to commit transaction. Error: %x"), hResult);
		AfxMessageBox(strMessage);
		m_oSession.Abort();
		m_oSession.Close();
		return FALSE;
	}
}