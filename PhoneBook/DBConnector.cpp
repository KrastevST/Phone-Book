#include "pch.h"
#include "DBConnector.h"

//////////////////////////////////////////////////////////////////////////////////
// CDBConnector


// Constructor / Destructor
//---------------------
CDBConnector::CDBConnector()
{
	m_pInstance = nullptr;
}
CDBConnector::~CDBConnector()
{
}

CDBConnector* CDBConnector::m_pInstance = nullptr;


// Methods
//---------------------
BOOL CDBConnector::Connect()
{
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);
	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("DESKTOP-5DTIFOB"));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("PhoneBook"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));
	HRESULT hResult = m_oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
	if (FAILED(hResult))
	{
		CString strMessage;
		strMessage.Format(_T("Unable to connect to SQL Server database. Error: %x"), hResult);
		AfxMessageBox(strMessage);

		return FALSE;
	}
}

void CDBConnector::Disconnect()
{
	m_oDataSource.Close();
}

CDBConnector* CDBConnector:: GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new CDBConnector();
	}

	return m_pInstance;
}

CDataSource& CDBConnector::GetDataSource()
{
	return m_oDataSource;
}