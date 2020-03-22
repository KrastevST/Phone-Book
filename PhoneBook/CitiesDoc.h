#pragma once
#include "Structures.h"
#include "CitiesData.h"
#include "TemplateDocument.h"

/////////////////////////////////////////////////////////////////////////////
// CCitiesDoc

class CCitiesDoc : public CTemplateDocument<CCitiesData, CITIES>
{

// Macros
// ----------------
protected:
	DECLARE_DYNCREATE(CCitiesDoc)
	DECLARE_MESSAGE_MAP()

// Constructor / Destructor
// ----------------
public:
	CCitiesDoc() noexcept;
	virtual ~CCitiesDoc();
};
