#pragma once
#include "pch.h"
#include "Structures.h"
#include "AutoReleaseTypedPtrArray.h"
#include "ObjectWrapper.h"
#include "Operations.h"

#include "framework.h"
#ifndef SHARED_HANDLERS
#include "PhoneBook.h"
#endif

#include <propkey.h>

/////////////////////////////////////////////////////////////////////////////
// CTemplateDocument

template<typename CTemplateDataClass, typename TABLE_RECORD>
class CTemplateDocument : public CDocument
{

// Macros
// ----------------

public:
	typedef CAutoReleaseTypedPtrArray<CPtrArray, TABLE_RECORD*> CTableRecordArray;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif
protected:

#ifdef SHARED_HANDLERS
	void SetSearchContent(const CString& value);
#endif

// Constructor / Destructor
// ----------------
public:
	CTemplateDocument() noexcept;
	virtual ~CTemplateDocument();

// Methods
// ----------------
public:
	virtual BOOL SelectAll();
	virtual BOOL SelectWhereID(const int nID, TABLE_RECORD& recTableRecord);
	virtual BOOL UpdateWhereID(const int nID, TABLE_RECORD& recTableRecord);
	virtual BOOL Insert(TABLE_RECORD& recTableRecord);
	virtual BOOL DeleteWhereID(const int nID);
	BOOL FindRecInDoc(int nID, TABLE_RECORD*& pTableRecord);
	CTableRecordArray& GetAllTableRecordsArray();

// Overrides
// ----------------
public:
	virtual BOOL OnNewDocument() override;
	virtual void Serialize(CArchive& ar) override;

// Members
//----------------------
protected:
	CTableRecordArray m_oAllTableRecordsArray;
	CTemplateDataClass m_oTemplateDataClass;
};


// Macros
//----------------------

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CTemplateDocument::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CTemplateDocument::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CTemplateDocument::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

#ifdef _DEBUG
template<typename CTemplateDataClass, typename TABLE_RECORD>
void CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::AssertValid() const
{
	CDocument::AssertValid();
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
void CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::Dump(CDumpContext& dc) const
{
	Dump(dc);
}
#endif //_DEBUG


// Constructor / Destructor
// ----------------

template<typename CTemplateDataClass, typename TABLE_RECORD>
CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::CTemplateDocument() noexcept
{
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::~CTemplateDocument()
{
}


// Methods
// ----------------

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::SelectAll()
{
	return m_oTemplateDataClass.SelectAll(m_oAllTableRecordsArray);
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::SelectWhereID(const int nID, TABLE_RECORD& recTableRecord)
{
	return m_oTemplateDataClass.SelectWhereID(nID, recTableRecord);
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::UpdateWhereID(const int nID, TABLE_RECORD& recTableRecord)
{
	BOOL bResult = m_oTemplateDataClass.UpdateWhereID(nID, recTableRecord);
	if (!bResult)
	{
		return FALSE;
	}

	TABLE_RECORD* pTableRecInDoc;
	bResult = FindRecInDoc(nID, pTableRecInDoc);
	if (!bResult)
	{
		return FALSE;
	}

	bResult = m_oTemplateDataClass.SelectWhereID(nID, *pTableRecInDoc);
	if (!bResult)
	{
		return FALSE;
	}

	CObjectWrapper<TABLE_RECORD>* pObjectWrapper = new CObjectWrapper<TABLE_RECORD>(*pTableRecInDoc);
	UpdateAllViews(NULL, OPERATION_EDIT, pObjectWrapper);
	return TRUE;
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::Insert(TABLE_RECORD& recTableRecord)
{
	int nID{};
	BOOL bResult = m_oTemplateDataClass.Insert(recTableRecord, nID);
	if (!bResult)
	{
		return FALSE;
	}

	m_oTemplateDataClass.SelectWhereID(nID, recTableRecord);
	m_oAllTableRecordsArray.Add(new TABLE_RECORD(recTableRecord));

	CObjectWrapper<TABLE_RECORD>* pObjectWrapper = new CObjectWrapper<TABLE_RECORD>(recTableRecord);
	UpdateAllViews(NULL, OPERATION_INSERT, pObjectWrapper);

	return TRUE;
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::DeleteWhereID(const int nID)
{
	BOOL bResult = m_oTemplateDataClass.DeleteWhereID(nID);
	if (!bResult)
	{
		return FALSE;
	}

	for (INT_PTR i = 0; i < m_oAllTableRecordsArray.GetCount(); i++)
	{
		if (m_oAllTableRecordsArray.GetAt(i)->nID == nID)
		{
			m_oAllTableRecordsArray.RemoveAt(i);
			bResult = TRUE;
			break;
		}
		bResult = FALSE;
	}

	if (!bResult)
	{
		CString strErrorMessage = _T("Error removing record from local storage");
		AfxMessageBox(strErrorMessage);
		return FALSE;
	}

	UpdateAllViews(NULL, OPERATION_DELETE);
	return TRUE;
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::FindRecInDoc(const int nID, TABLE_RECORD*& pTableRecord)
{
	for (INT_PTR i = 0; i < m_oAllTableRecordsArray.GetCount(); i++)
	{
		if (m_oAllTableRecordsArray.GetAt(i)->nID == nID)
		{
			pTableRecord = m_oAllTableRecordsArray.GetAt(i);
			return TRUE;
		}
	}

	CString strErrorMessage = _T("Error finding the selected record in local storage");
	AfxMessageBox(strErrorMessage);
	return FALSE;
}

template<typename CTemplateDataClass, typename TABLE_RECORD>
CAutoReleaseTypedPtrArray<CPtrArray, TABLE_RECORD*>& CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::GetAllTableRecordsArray()
{
	return m_oAllTableRecordsArray;
}


// MFC Overrides
// ----------------

template<typename CTemplateDataClass, typename TABLE_RECORD>
void CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// MFC Message Handlers
// ----------------

template<typename CTemplateDataClass, typename TABLE_RECORD>
BOOL CTemplateDocument<CTemplateDataClass, TABLE_RECORD>::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	SelectAll();

	return TRUE;
}

