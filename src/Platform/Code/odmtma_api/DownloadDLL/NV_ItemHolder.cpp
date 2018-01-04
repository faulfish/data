#include "StdAfx.h"
#include "NV_ItemHolder.h"
CNV_ItemHolder::CNV_ItemHolder()
{
	m_nSize = -1;
	m_nNVid = -1;
	m_pData = NULL;
	m_szName = '\0';
}

CNV_ItemHolder::CNV_ItemHolder(const CNV_ItemHolder& src)
{
	this->m_nSize = src.GetSize();
	this->m_nNVid = src.GetNV_ID();
	_CpyData(src.GetData());
	this->m_szName = src.m_szName;
}

CNV_ItemHolder::CNV_ItemHolder(const unsigned char* srcData,int srcSize,int srcNVId)
{
    this->m_nNVid = srcNVId;
	this->m_nSize = srcSize;
	_CpyData(srcData);
	this->m_szName = '\0';
}

CNV_ItemHolder::CNV_ItemHolder(const unsigned char* srcData,int srcSize,CString srcFileName)
{
	this->m_nNVid = -1;
	this->m_nSize = srcSize;
	_CpyData(srcData);
	this->m_szName = srcFileName;
}


CNV_ItemHolder::~CNV_ItemHolder()
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}

int CNV_ItemHolder::GetSize() const
{
	/*
	size_t check;
	check = _msize(_data);
	*/
	return m_nSize;	
}

int CNV_ItemHolder::GetNV_ID() const
{
	return m_nNVid;
}

CString CNV_ItemHolder::GetName() const
{
	return m_szName;
}
unsigned char* CNV_ItemHolder::GetData() const
{
	return m_pData;
}

void CNV_ItemHolder::SetSize(int nSize)
{
	m_nSize = nSize;
	m_pData = static_cast<unsigned char*>( realloc(m_pData, nSize));
}

void CNV_ItemHolder::SetNV_ID(int nNV_id)
{
	m_nNVid = nNV_id;
}

void CNV_ItemHolder::SetNV_ID(CString& sNV_id)
{
	//strtol();
	m_nNVid = atoi(sNV_id.GetBuffer());
	sNV_id.ReleaseBuffer();
}

void CNV_ItemHolder::SetName(const CString& srcFileName)
{
	m_szName = srcFileName;
}

void CNV_ItemHolder::SetData(const unsigned char* nData,int nSize)
{
	m_nSize = nSize;
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	_CpyData(nData);

}

void CNV_ItemHolder::_CpyData(/*int gSize,*/ const unsigned char *nData)
{
	//m_size = gSize;
	if(m_nSize >0)
	{
		m_pData = new unsigned char[m_nSize];

		for (int i = 0;i<m_nSize;i++)
			m_pData[i] = nData[i];
	}
	
    
}

CNV_ItemHolder& CNV_ItemHolder::operator = (const CNV_ItemHolder& src)
{
	
	m_nSize = src.GetSize();
	m_nNVid = src.GetNV_ID();
	m_szName = src.GetName();

	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
	_CpyData(src.GetData());
	
	return (*this);
}
/*
void CNV_ItemHolder::ModifyData( int offset, int length, enDataTypes type, CString value )
{
	if (_data)
	{
		switch(type)
		{
			case enUINT8:
				_ModifyUInt8( offset, length, value );
				break;
			case enUINT16:
				_ModifyUInt16( offset, length, value );
				break;
			case enINT8:
				_ModifyInt8( offset, length, value );
				break;
			case enINT16:
				_ModifyInt16( offset, length, value );
				break;
			case enUINT32:
				_ModifyUInt32( offset, length, value );
				break;
			case enINT32:  
				_ModifyInt32( offset, length, value );
				break;
			case enASCIIZ:
				_ModifyASCIIZ( offset, length, value );
				break;
			case enASCIIFF:
				_ModifyASCIIFF( offset, length, value );
				break;
			case enUTF16:
				_ModifyUTF16( offset, length, value );
				break;
			case enUTF8:
				_ModifyUTF8( offset, length, value );
				break;
			case enBOOL:
				_ModifyBool( offset, length, value );
			default:
				;
		}
	}
}/*
CString CNV_ItemHolder::ExtractData( int offset, int length, enDataTypes type )
{
	if (_data)
	{
		switch(type)
		{
			case enUINT8:
				return _ExtractUInt8( offset, length );
				break;
			case enUINT16:
				return _ExtractUInt16( offset, length );
				break;
			case enINT8:
				return _ExtractInt8( offset, length );
				break;
			case enINT16:
				return _ExtractInt16( offset, length );
				break;
			case enUINT32:
				return _ExtractUInt32( offset, length );
				break;
			case enINT32:  
				return _ExtractInt32( offset, length );
				break;
			case enASCIIZ:
				return _ExtractASCIIZ( offset, length );
				break;
			case enASCIIFF:
				return _ExtractASCIIFF( offset, length );
				break;
			case enUTF16:
				return _ExtractUTF16( offset, length );
				break;
			case enUTF8:
				return _ExtractUTF8( offset, length );
				break;
			case enBOOL:
				return _ExtractBool( offset, length );
			default:
				return '\0';
		}
	}
return '\0';//test
}
*/
/* private */
/*
void CNV_ItemHolder::_ModifyUInt8( int offset, int length, CString value )
{
	unsigned char* bufCon = new unsigned char;
	*bufCon = static_cast<unsigned char>(atoi(value.GetBuffer(128)));
	value.ReleaseBuffer();
	//unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	_data[offset] = *bufCon;

	delete bufCon;

}

void CNV_ItemHolder::_ModifyUInt16( int offset, int length, CString value )
{
	unsigned short* bufCon = new unsigned short;
	*bufCon = static_cast<unsigned short>(atoi(value.GetBuffer(128)));
	value.ReleaseBuffer();
	unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	for (int i = 0; i < 2; i++ )
		_data[offset+i] = pData[i];
	
	delete bufCon;	
}

void CNV_ItemHolder::_ModifyInt8( int offset, int length, CString value )
{
	char* bufCon = new char;
	*bufCon = static_cast<char>(atoi(value.GetBuffer(128)));
	value.ReleaseBuffer();
	unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	_data[offset] = *pData;

	delete bufCon;

}

void CNV_ItemHolder::_ModifyInt16( int offset, int length, CString value )
{
	//short - length = 2
	short* bufCon = new short;
	*bufCon = static_cast<short>(atoi(value.GetBuffer(128)));
	value.ReleaseBuffer();
	unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	for (int i = 0; i <  2; i++ )
		_data[offset+i] = pData[i];
	
	delete bufCon;	
}

void CNV_ItemHolder::_ModifyUInt32( int offset, int length, CString value )
{
	//short - length = 4
	unsigned long* bufCon = new unsigned long;
	*bufCon = static_cast<unsigned long>(atoi(value.GetBuffer(128)));
	value.ReleaseBuffer();
	unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	for (int i = 0; i < 4; i++ )
		_data[offset+i] = pData[i];
	
	delete bufCon;	
}

void CNV_ItemHolder::_ModifyInt32( int offset, int length, CString value )
{
	//short - length = 2
	long* bufCon = new long;
	*bufCon = static_cast<long>(atoi(value.GetBuffer(128)));
	value.ReleaseBuffer();
	unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	for (int i = 0; i <  4; i++ )
		_data[offset+i] = pData[i];
	
	delete bufCon;	
}

void CNV_ItemHolder::_ModifyASCIIZ( int offset, int length, CString value )
{
	unsigned char* buf = reinterpret_cast<unsigned char*>(value.GetBuffer(length+1));
	
	/*
	for ( int i = 0; i < length; i++ )
		_data[offset+i] = 0x00;
	*//*
	int i = 0;
	for (i = 0; i < value.GetLength(); i++ )
		_data[offset+i] = buf[i];
	
	for (;i < length-1; i++ )
		_data[offset+i] = 0x00;
	
	//0 included in length value
	_data[offset+length-1] = 0x00;

	value.ReleaseBuffer();
}

void CNV_ItemHolder::_ModifyASCIIFF( int offset, int length, CString value )
{
	_ModifyASCIIZ(offset,length,value);
	_data[offset+length-1] = 0xFF;
}

void CNV_ItemHolder::_ModifyUTF16( int offset, int length, CString value )
{
	WCHAR* wszText;
	wszText = new WCHAR[length];
	int valSize = value.GetLength();

	MultiByteToWideChar(CP_ACP, 0, value.GetBuffer(valSize+1), value.GetLength(), wszText, valSize );
	value.ReleaseBuffer();
	unsigned char *pBuf = reinterpret_cast<unsigned char*>(wszText);

	//for( int i = 0; i < length-valSize*2; i++ )
	int i = 0;
	for(i = 0; i < valSize*2; i++ )
		_data[offset+i] = pBuf[i];
	for ( i = valSize*2; i < length; i++)
		_data[offset+i] = 0;
	
	delete[] wszText;
}
void CNV_ItemHolder::_ModifyUTF8( int offset, int length, CString value )
{

	WCHAR* wszText;
	wszText = new WCHAR[length];
	int valSize = value.GetLength();

	int retVal = MultiByteToWideChar(CP_ACP, 0, value.GetBuffer(valSize+1), value.GetLength(), wszText, valSize );	//utf16
	value.ReleaseBuffer();
	char* uText;
	uText = new char[valSize*3];
	retVal = WideCharToMultiByte(CP_UTF8,0,	wszText, value.GetLength(),uText,valSize*3, NULL,NULL);
	unsigned char *pBuf = reinterpret_cast<unsigned char*>(uText);
	int i = 0;
	for(i = 0; i < retVal; i++ )
		_data[offset+i] = pBuf[i];
	for (; i < length; i++)
		_data[offset+i] = 0;
	
	delete[] wszText;
	delete[] uText;
}

void CNV_ItemHolder::_ModifyBool( int offset, int length, CString value )
{
	//bool bufCon;
	unsigned char bufCon; 
	if (atoi(value.GetBuffer(64)) == 1)
		bufCon = 1;
		//bufCon = true;
	else
		bufCon = 0;
		//bufCon = false;
	
	//unsigned char* pData = reinterpret_cast<unsigned char*>(bufCon);
	_data[offset] = bufCon/**pData*//*;
}


CString CNV_ItemHolder::_ExtractUInt8( int offset, int length )	
{
	//wchar_t *wText = new wchar_t[8];
	wchar_t wText[64];
    _ltow(static_cast<unsigned long>(_data[offset]),wText,10);
	return wText;
}

CString CNV_ItemHolder::_ExtractUInt16( int offset, int length ) 
{
	wchar_t wText[64];
	unsigned short bufCon;
	unsigned char* pData = reinterpret_cast<unsigned char*>(&bufCon);
	pData[0] = _data[offset];
	pData[1] = _data[offset+1];

    _ltow(static_cast<unsigned long>(bufCon),wText,10);
	return wText;

}

CString CNV_ItemHolder::_ExtractUInt32( int offset, int length ) 
{
	wchar_t wText[64];
	unsigned long bufCon;
	unsigned char* pData = reinterpret_cast<unsigned char*>(&bufCon);
	pData[0] = _data[offset];
	pData[1] = _data[offset+1];
	pData[2] = _data[offset+2];
	pData[3] = _data[offset+3];

    _ltow(static_cast<unsigned long>(bufCon),wText,10);
	return wText;

}

CString CNV_ItemHolder::_ExtractInt8( int offset, int length ) 
{
	wchar_t wText[64] = {0};
    _ltow(static_cast<long>(_data[offset]),wText,10);
	return wText;
}

CString CNV_ItemHolder::_ExtractInt16( int offset, int length ) 
{
	wchar_t wText[64] = {0};
	short bufCon;
	unsigned char* pData = reinterpret_cast<unsigned char*>(&bufCon);
	pData[0] = _data[offset];
	pData[1] = _data[offset+1];

    _ltow(static_cast<long>(bufCon),wText,10);
	return wText;
}

CString CNV_ItemHolder::_ExtractInt32( int offset, int length ) 
{
	wchar_t wText[64] = {0};
	long bufCon;
	unsigned char* pData = reinterpret_cast<unsigned char*>(&bufCon);
	pData[0] = _data[offset];
	pData[1] = _data[offset+1];
	pData[2] = _data[offset+2];
	pData[3] = _data[offset+3];

    _ltow(static_cast<long>(bufCon),wText,10);
	
	return wText;
}

CString CNV_ItemHolder::_ExtractASCIIZ( int offset, int length ) 
{
	/*
	wchar_t wText[128] = {0};
	
	for ( int i = 0; i < length-1; i++ )
		wText[i] = _data[offset+i];
	//wText[length-1] = '\0';
	
	return wText;
	*//*
	char *cText = new char[length+1];
	for ( int i = 0; i < length-1; i++ )
		cText[i] = _data[offset+i];
	cText[length] = '\0';
	
	return cText;
}

CString CNV_ItemHolder::_ExtractASCIIFF( int offset, int length )
{
	/*
	wchar_t wText[128] = {0};
	for ( int i = 0; i < length-1; i++ )
		wText[i] = _data[offset+i];
	//wText[length-1] = '\0';
	return wText;
	*//*
	char *cText = new char[length+1];
	for ( int i = 0; i < length-1; i++ )
		cText[i] = _data[offset+i];
	cText[length] = 255;

	return cText;
 }

CString CNV_ItemHolder::_ExtractUTF16( int offset, int length )
{
	WCHAR* bufText = new WCHAR[static_cast<int>(length / 2)];
	unsigned char* pBuf = reinterpret_cast<unsigned char*>(bufText);

	for (int i = 0; i < length; i++ )
		pBuf[i] = _data[offset+i];
	
	//maybe conversion to other coding format /UTF8?
	CString retVal(bufText);
	delete[] bufText;
	
	return retVal;
}

CString CNV_ItemHolder::_ExtractUTF8( int offset, int length )
{
	char* bufText = new char[length];
	unsigned char* pBuf = reinterpret_cast<unsigned char*>(bufText);

	for (int i = 0; i < length; i++ )
		pBuf[i] = _data[offset+i];
	
	//maybe conversion to other coding format /UTF8?
	CString retVal(bufText);
	delete[] bufText;
	
	return retVal;
}

CString CNV_ItemHolder::_ExtractBool( int offset, int length ) 
{
	wchar_t wText[64] = {0};
	
	_ltow(static_cast<unsigned long>(_data[offset]),wText,10);
		
	return wText;
}*/