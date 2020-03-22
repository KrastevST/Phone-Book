#pragma once
#include <atldbcli.h>
#include <atlstr.h>
#include "AutoReleaseTypedPtrArray.h"
#include "DBConnector.h"

//////////////////////////////////////////////////////////////////////////////////
// CTemplateTable


template <typename TABLE_RECORD, typename CTableAccessor>
class CTemplateTable : protected CCommand<CAccessor<CTableAccessor>>
{

// Constructor / Destructor
//---------------------
public:
	CTemplateTable(CString strTableName, size_t Offset);
	virtual ~CTemplateTable();

// Macros
//---------------------
#define SELECT 0
#define UPDATE DBPROPVAL_UP_CHANGE
#define INSERT DBPROPVAL_UP_INSERT
#define DELETE DBPROPVAL_UP_DELETE

	typedef CAutoReleaseTypedPtrArray<CPtrArray, TABLE_RECORD*> CTableRecordArray;

// Methods
//---------------------
public:
	BOOL SelectAll(CTableRecordArray& oCTableRecordArray);
	BOOL SelectWhereID(const int nID, TABLE_RECORD& recTableRecord);
	BOOL UpdateWhereID(const int nID, TABLE_RECORD& recTableRecord);
	BOOL Insert(TABLE_RECORD& recTableRecord, int& nID);
	BOOL DeleteWhereID(const int nID);
	void SetExternalSession(BOOL bUseExternalSession, CSession* pSession);

protected:
	void TerminateOperation(CString& strMessage, CString& strQuery);
	void TerminateOperation(CString& strMessage);
	BOOL OpenDBRecord(long lOperationType, int nID);
	BOOL OpenDBRecord(long lOperationType, CString strQuery);
	BOOL OpenSession();
	void CloseSession();

private:
	void SetCDBPropSetRowSet(CDBPropSet& oDBPropSet, long lOperation);
	virtual TABLE_RECORD& GetRecord() = 0;
	int* GetUpdateCounter(TABLE_RECORD& recTableRecord);

// Members
//---------------------
protected:
	HRESULT m_hResult{ S_OK };

private:
	CSession* m_pSession;
	CString m_strTableName;
	BOOL m_bUseExternalSession = FALSE;
	size_t m_UpdateCounterOffset;
};

template <typename TABLE_RECORD, typename TableAccessor>
CTemplateTable<TABLE_RECORD, TableAccessor>::CTemplateTable(CString strTableName, size_t Offset)
{
	m_strTableName = strTableName;
	m_UpdateCounterOffset = Offset;
}

template <typename TABLE_RECORD, typename TableAccessor>
CTemplateTable<TABLE_RECORD, TableAccessor>::~CTemplateTable()
{
}

// Functions
//---------------------
template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::SelectAll(CTableRecordArray& oTableRecordArray)
{
	BOOL bResult = OpenSession();
	if (!bResult)
	{
		return FALSE;
	}

	CString strQuery{};
	strQuery.Format(_T("SELECT * FROM %s"), m_strTableName);
	bResult = OpenDBRecord(SELECT, strQuery);
	if (!bResult)
	{
		return FALSE;
	}

	m_hResult = MoveNext();
	if (FAILED(m_hResult))
	{
		CString strErrorMessage("Error reading from database");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	oTableRecordArray.RemoveAll();
	do
	{
		oTableRecordArray.Add(new TABLE_RECORD(GetRecord()));
	} while (MoveNext() != DB_S_ENDOFROWSET);

	CloseSession();
	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::SelectWhereID(const int nID, TABLE_RECORD& recTableRecord)
{
	BOOL bResult = OpenSession();
	if (!bResult)
	{
		return FALSE;
	}

	bResult = OpenDBRecord(SELECT, nID);
	if (!bResult)
	{
		return FALSE;
	}

	recTableRecord = GetRecord();

	CloseSession();
	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::UpdateWhereID(const int nID, TABLE_RECORD& recTableRecord)
{
	BOOL bResult = OpenSession();
	if (!bResult)
	{
		return FALSE;
	}

	bResult = OpenDBRecord(UPDATE, nID);
	if (!bResult)
	{
		return FALSE;
	}

	GetRecord() = recTableRecord;

	if (*GetUpdateCounter(recTableRecord) != *GetUpdateCounter(GetRecord()))
	{
		CString strErrorMessage = _T("Error, record was modified");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	++(*GetUpdateCounter(GetRecord()));

	m_hResult = SetData(READ_AND_WRITE_ACCESSOR);
	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error updating record");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	CloseSession();
	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::Insert(TABLE_RECORD& recTableRecord, int& nID)
{
	BOOL bResult = OpenSession();
	if (!bResult)
	{
		return FALSE;
	}

	CString strQuery;
	strQuery.Format(_T("SELECT TOP (0) * FROM %s"), m_strTableName);
	bResult = OpenDBRecord(INSERT, strQuery);
	if (!bResult)
	{
		return FALSE;
	}

	m_hResult = MoveNext();
	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error opening record");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	GetRecord() = recTableRecord;
	m_hResult = __super::Insert(READ_AND_WRITE_ACCESSOR);
	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error inserting record");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	m_hResult = MoveFirst();
	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error opening record");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	nID = GetRecord().nID;

	CloseSession();
	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::DeleteWhereID(const int nID)
{
	BOOL bResult = OpenSession();
	if (!bResult)
	{
		return FALSE;
	}

	bResult = OpenDBRecord(DELETE, nID);
	if (!bResult)
	{
		return FALSE;
	}

	m_hResult = Delete();
	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error deleting record");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	CloseSession();
	return TRUE;
}

// Methods
//---------------------
template <typename TABLE_RECORD, typename TableAccessor>
void CTemplateTable<TABLE_RECORD, TableAccessor>::TerminateOperation(CString& strMessage, CString& strQuery)
{
	CString strFormattedMessage;
	strFormattedMessage.Format(_T("%s. Error: %x. Query: %s"), strMessage.GetString(), m_hResult, strQuery.GetString());
	AfxMessageBox(strFormattedMessage);

	CloseSession();
}

template <typename TABLE_RECORD, typename TableAccessor>
void CTemplateTable<TABLE_RECORD, TableAccessor>::TerminateOperation(CString& strMessage)
{
	CString strFormattedMessage;
	strFormattedMessage.Format(_T("%s. Error: %x."), strMessage.GetString(), m_hResult);
	AfxMessageBox(strFormattedMessage);

	CloseSession();
}

template <typename TABLE_RECORD, typename TableAccessor>
void CTemplateTable<TABLE_RECORD, TableAccessor>::SetCDBPropSetRowSet(CDBPropSet& oDBPropSet, long lOperation)
{
	oDBPropSet.AddProperty(DBPROP_CANFETCHBACKWARDS, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oDBPropSet.AddProperty(DBPROP_IRowsetChange, true);
	oDBPropSet.AddProperty(DBPROP_UPDATABILITY, lOperation);
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::OpenDBRecord(long lOperationType, int nID)
{
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM %s WHERE ID = %i"), m_strTableName, nID);
	BOOL bResult = OpenDBRecord(lOperationType, strQuery);
	if (!bResult)
	{
		return FALSE;
	}

	m_hResult = MoveFirst();
	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error opening record");
		TerminateOperation(strErrorMessage);
		return FALSE;
	}

	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::OpenDBRecord(long lOperationType, CString strQuery)
{
	if (lOperationType != SELECT)
	{
		CDBPropSet oDBPropSet(DBPROPSET_ROWSET);
		SetCDBPropSetRowSet(oDBPropSet, lOperationType);
		m_hResult = Open(*m_pSession, strQuery, &oDBPropSet);
	}
	else
	{
		m_hResult = Open(*m_pSession, strQuery);
	}

	if (FAILED(m_hResult))
	{
		CString strErrorMessage = _T("Error executing query");
		TerminateOperation(strErrorMessage, strQuery);
		return FALSE;
	}

	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
BOOL CTemplateTable<TABLE_RECORD, TableAccessor>::OpenSession()
{
	if (m_bUseExternalSession)
	{
		return TRUE;
	}

	m_pSession = new CSession;

	CDataSource& DataSource = CDBConnector::GetInstance()->GetDataSource();
	m_hResult = m_pSession->Open(DataSource);
	if (FAILED(m_hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to open session. Error: %x"), m_hResult);
		AfxMessageBox(strMessage);

		return FALSE;
	}

	return TRUE;
}

template <typename TABLE_RECORD, typename TableAccessor>
void  CTemplateTable<TABLE_RECORD, TableAccessor>::CloseSession()
{
	Close();

	if (m_bUseExternalSession)
	{
		return;
	}

	m_pSession->Close();
}

template <typename TABLE_RECORD, typename TableAccessor>
void CTemplateTable<TABLE_RECORD, TableAccessor>::SetExternalSession(BOOL bUseExternalSession, CSession* pSession)
{
	m_bUseExternalSession = bUseExternalSession;
	m_pSession = pSession;
}

template <typename TABLE_RECORD, typename TableAccessor>
int* CTemplateTable<TABLE_RECORD, TableAccessor>::GetUpdateCounter(TABLE_RECORD& recTableRecord)
{
	byte* pUpdateCounterAddress = (byte*)&recTableRecord;
	pUpdateCounterAddress += m_UpdateCounterOffset;
	int* pUpdateCounter = (int*)pUpdateCounterAddress;

	return pUpdateCounter;
}
