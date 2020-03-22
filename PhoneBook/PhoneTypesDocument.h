#pragma once
#include "Structures.h"
#include "PhoneTypesData.h"
#include "TemplateDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CPhoneTypesDocument

class CPhoneTypesDocument : public CTemplateDocument<CPhoneTypesData, PHONE_TYPES>
{

// Macros
// ----------------
protected:
	DECLARE_DYNCREATE(CPhoneTypesDocument)
	DECLARE_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
public:
	CPhoneTypesDocument() noexcept;
	virtual ~CPhoneTypesDocument();
};

