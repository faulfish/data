#pragma once
//#include "stdafx.h"
//#include <afxwin.h>
//#include <afxtempl.h>

#include "NV_ItemHolder.h"

class CNVArray : public  CList <CNV_ItemHolder,CNV_ItemHolder&>
{

public:
	CNVArray();
	~CNVArray();

	INT_PTR Add( CNV_ItemHolder& newElement );			// add element at tail of list & return index of added element
	INT_PTR Append( /*const*/ CNVArray& src );			// appends another CNVArray to the CNVArray
	INT_PTR FindByNVItem( INT_PTR nV_ID );				// returns index of given NV Item specified by his ID or -1 if not found
	INT_PTR FindByNVItem( /*LPCWSTR*/ CString fileName );			// returns index of given NV Item specified by his name or -1 if not found
	CNV_ItemHolder& GetByNVItem( INT_PTR nV_ID );		// returns CNVArray with given NV Item ID
	CNV_ItemHolder& GetByNVItem( /*LPCWSTR*/ CString fileName );	// returns CNVArray with given NV Item ID
	BOOL IsInArray( INT_PTR nV_ID );					// check if NV Item specified by his ID is in CNVArray
	BOOL IsInArray( /*LPCWSTR*/CString fileName );					// check if NV Item specified by his ID is in CNVArray
	
	VOID SortByID();
	CArray<CNV_ItemHolder,CNV_ItemHolder&>& ExportToCArray( CArray<CNV_ItemHolder,CNV_ItemHolder&> &Arr );
	INT_PTR ImportFromCArray( CArray<CNV_ItemHolder,CNV_ItemHolder&> &Arr );
	
	CNV_ItemHolder& operator[]( INT_PTR nIndex );
	const CNV_ItemHolder& operator[]( INT_PTR nIndex ) const;
	
};
