#pragma once
#include "pch.h"

/////////////////////////////////////////////////////////////////////////////
// CObjectWrapper

template<typename T>
class CObjectWrapper : public CObject
{

// Constructor / Destructor
// ----------------
public:
	CObjectWrapper(T& Object);
	virtual~CObjectWrapper();

// Methods
// ----------------
public:
	T& GetObject();

// Members
// ----------------
private:
	T& m_Object;
};


// Constructor / Destructor
// ----------------

template<typename T>
CObjectWrapper<T>::CObjectWrapper(T& Object)
	: m_Object(Object)
{
}

template<typename T>
CObjectWrapper<T>::~CObjectWrapper()
{
}

// Methods
// ----------------

template<typename T>
T& CObjectWrapper<T>::GetObject()
{
	return m_Object;
}