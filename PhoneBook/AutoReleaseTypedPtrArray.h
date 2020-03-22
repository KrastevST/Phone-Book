#pragma once

template<typename T, typename U>
class CAutoReleaseTypedPtrArray : public CTypedPtrArray<T, U>
{
public:
	~CAutoReleaseTypedPtrArray() override
	{
		for (INT_PTR i = 0; i < this->GetSize(); i++)
		{
			delete this->GetAt(i);
		}
	}
};