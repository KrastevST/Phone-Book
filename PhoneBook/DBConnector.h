#pragma once
#include <atldbcli.h>

//////////////////////////////////////////////////////////////////////////////////
// CDBConnector

class CDBConnector
{

// Constructor / Destructor
//---------------------
private:
	CDBConnector();
public:
	~CDBConnector();


// Methods
//---------------------
public:
	static CDBConnector* GetInstance();
	BOOL Connect();
	void Disconnect();
	CDataSource& GetDataSource();

// Members
//---------------------
private:
	CDataSource m_oDataSource;
	static CDBConnector* m_pInstance;
};

