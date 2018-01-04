//
#pragma once

#include <afxtempl.h>
//#include <malloc.h>
//#include <stdlib.h>

enum enDataTypes
{
	enUNDEFINED=0x00, // for error cases, 
    enUINT8=0x01, 
    enUINT16=0x02, 
    enINT8=0x03,
    enINT16=0x04, 
    enUINT32=0x05,
    enINT32=0x06,  
    enUTF16=0x50,
	enUTF8=0x51,
    enASCIIZ=0x100,
    enASCIIFF=0x101, 
    enBOOL=0x150 
};


class CNV_ItemHolder
{
public:
	CNV_ItemHolder();
	CNV_ItemHolder(const CNV_ItemHolder&);
	CNV_ItemHolder(const unsigned char* srcData,int srcSize,int srcNVId);		//construct new object based on pointer to buffer, NV item size
	CNV_ItemHolder(const unsigned char* srcData,int srcSize, CString srcName);		//construct new object based on pointer to buffer, File item size
	
	~CNV_ItemHolder();

	int GetSize() const;
	int GetNV_ID() const;
	CString GetName() const;
	unsigned char* GetData() const;
	void SetSize(int nSize);
	void SetNV_ID(int nNV_id);
	void SetNV_ID(CString& sNV_id);
	void SetName(const CString& srcFileName);
	void SetData(const unsigned char* nData,int nSize);
	
	//void ModifyData( int offset, int length, enDataTypes type, CString value );
	//CString ExtractData( int offset, int length, enDataTypes type );
	
	CNV_ItemHolder& operator = (const CNV_ItemHolder& src);

	
private:
	/* data members */
	int m_nSize;
	int m_nNVid;
	CString m_szName;
	
	unsigned char* m_pData;

	/* function members */
	void _CpyData( /*int gSize,*/ const unsigned char *nData );
	
	/* .. conversion methods *//*
	void _ModifyUInt8( int offset, int length, CString value );
	void _ModifyUInt16( int offset, int length, CString value );
	void _ModifyUInt32( int offset, int length, CString value );
	void _ModifyInt8( int offset, int length, CString value );
	void _ModifyInt16( int offset, int length, CString value );
	void _ModifyInt32( int offset, int length, CString value );
	void _ModifyASCIIZ( int offset, int length, CString value );
	void _ModifyASCIIFF( int offset, int length, CString value );
	void _ModifyUTF16( int offset, int length, CString value );
	void _ModifyUTF8( int offset, int length, CString value );
	void _ModifyBool( int offset, int length, CString value );
	
	/* .. .. extraction methods *//*
	CString _ExtractUInt8( int offset, int length );
	CString _ExtractUInt16( int offset, int length );
	CString _ExtractUInt32( int offset, int length );
	CString _ExtractInt8( int offset, int length );
	CString _ExtractInt16( int offset, int length );
	CString _ExtractInt32( int offset, int length );
	CString _ExtractASCIIZ( int offset, int length );
	CString _ExtractASCIIFF( int offset, int length );
	CString _ExtractUTF16( int offset, int length );
	CString _ExtractUTF8( int offset, int length );
	CString _ExtractBool( int offset, int length );*/
};

// sizeof = 8 + GetSize()
// g - given
// n - new
