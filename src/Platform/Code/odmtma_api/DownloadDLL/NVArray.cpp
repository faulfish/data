#include "stdafx.h"

#include "NVArray.h"


CNVArray::CNVArray()
{

}

CNVArray::~CNVArray()
{
	if( this->GetCount() > 0 )
		this->RemoveAll();
}

INT_PTR CNVArray::Add(CNV_ItemHolder& newElement)
{
	if (IsEmpty())
		this->AddHead(newElement);
	else
		this->AddTail(newElement);
	
	return this->GetCount()-1;
}


INT_PTR CNVArray::Append( /*const*/ CNVArray& src )
{
	int index = static_cast<int>(this->GetCount());
	
	for (int i = 0; i < src.GetCount(); i++)
		this->Add(src[i]);
	
	return index;
}

INT_PTR CNVArray::FindByNVItem(INT_PTR nV_ID)
{
	for (INT_PTR i = 0; i < this->GetCount(); i++)
		if ( (*this)[i].GetNV_ID() == nV_ID )
			return i;

	return -1;
}

INT_PTR CNVArray::FindByNVItem( CString fileName )
{
	for (INT_PTR i = 0; i < this->GetCount(); i++)
		if ( (*this)[i].GetName() == fileName )
			return i;

	return -1;
}

CNV_ItemHolder& CNVArray::GetByNVItem( INT_PTR nV_ID )
{
	return (*this)[this->FindByNVItem(nV_ID)];
}

CNV_ItemHolder& CNVArray::GetByNVItem( CString fileName )
{
	return (*this)[this->FindByNVItem(fileName)];
}

BOOL CNVArray::IsInArray( INT_PTR nV_ID )
{
	int index = static_cast<int>(this->FindByNVItem(nV_ID));
	
	if (index == -1)
		return false;
	else
		return true;
}

BOOL CNVArray::IsInArray( CString fileName )
{
	int index = static_cast<int>(this->FindByNVItem( fileName ));
	
	if (index == -1)
		return false;
	else
		return true;
}

VOID CNVArray::SortByID()
{
	POSITION pos;
	int i,j,aSize = static_cast<int>(this->GetCount());

	for (i = 0; i < aSize; i++)
	{
		pos = this->FindIndex(i);
		for (j = i+1; j < aSize; j++ )
		{
			if ( this->GetAt(pos).GetNV_ID() < (*this)[j].GetNV_ID() )
				pos = this->FindIndex(j);
		}
		this->AddHead(this->GetAt(pos));
		this->RemoveAt(pos);
	}
}

CArray<CNV_ItemHolder,CNV_ItemHolder&>& CNVArray::ExportToCArray( CArray<CNV_ItemHolder,CNV_ItemHolder&> &tArray )
{
	if ( !(tArray.IsEmpty()) )
		tArray.RemoveAll();
	
	tArray.SetSize( this->GetCount() );
	
	for (int i = 0; i <  this->GetCount(); i++)
		tArray[i] = (*this)[i];
	
	return tArray;
}

INT_PTR CNVArray::ImportFromCArray( CArray<CNV_ItemHolder,CNV_ItemHolder&> &tArray )
{
	for (int i = 0; i <  tArray.GetCount(); i++)
		this->Add(tArray[i]);
	
	return this->GetCount()-1;
}


CNV_ItemHolder& CNVArray::operator[]( INT_PTR nIndex )
{
	POSITION pos;
	pos = this->FindIndex(nIndex);

	return const_cast<CNV_ItemHolder&> ( this->GetAt(pos) );
}

const CNV_ItemHolder& CNVArray::operator[]( INT_PTR nIndex ) const
{
	POSITION pos;
	pos = this->FindIndex(nIndex);

	return  this->GetAt(pos) ;
}