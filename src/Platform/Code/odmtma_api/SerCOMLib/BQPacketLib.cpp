#include "stdafx.h"
#include <tchar.h>
#include <crtdbg.h>
#include "BQPacketLib.h"
#include "SerialIO.h"

// 2004/09/22 Embert Tsai
// Pack CRC and add start and end characters.
int CBQPacketLib::PackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest){
	_ASSERTE(pvSrc != NULL);
	_ASSERTE(nSrcLen != 0);
	_ASSERTE(*pvDest == NULL);
	int nRetLen = 0;
	const BYTE *pbSrc = (const BYTE *)pvSrc;
	BYTE **pbDest = (BYTE **)pvDest;
	*pbDest = new BYTE[nSrcLen * 2 + 4 ]; //including ESCAPE character and starting and ending characters and 2 CRC characters.
	ZeroMemory(*pbDest, nSrcLen*2 + 2 );

	//CCRCMachine oCRC;
	WORD wCRC = CCRCMachine::CRC16LCalculation((CHAR*)pbSrc, (DWORD)(nSrcLen * 8));
	BYTE loCRC = LOBYTE(wCRC);
	BYTE hiCRC = HIBYTE(wCRC);
	
	if ( chStart != PACKET_NULL_CHAR ){
		*((*pbDest) + nRetLen++ ) = (BYTE) chStart;
	}

	for(int i = 0; i < nSrcLen; i ++)
	{
		switch(pbSrc[i])
		{
		case ESCAPE_CHAR:
		case END_CHAR:
			
			*((*pbDest) + nRetLen++ ) = ESCAPE_CHAR;
			*((*pbDest) + nRetLen++ ) = *(pbSrc+i) ^ COMPLEMENT_CHAR;
			break;
		default:
			*((*pbDest) + nRetLen++ ) = *(pbSrc+i) ;
			break;
		}
	}

	switch(loCRC)
	{
	case ESCAPE_CHAR:
	case END_CHAR:
		*((*pbDest) + nRetLen++ ) = ESCAPE_CHAR;
		*((*pbDest) + nRetLen++ ) = loCRC ^ COMPLEMENT_CHAR;
		break;
	default:
		*((*pbDest) + nRetLen++ ) = loCRC;
		break;
	}
	
	switch(hiCRC)
	{
	case ESCAPE_CHAR:
	case END_CHAR:
		*((*pbDest) + nRetLen++ ) = ESCAPE_CHAR;
		*((*pbDest) + nRetLen++ ) = hiCRC ^ COMPLEMENT_CHAR;
		break;
	default:
		*((*pbDest) + nRetLen++ ) = hiCRC;
		break;
	}
	*((*pbDest) + nRetLen++ ) = chEnd;
	return nRetLen;

}

int CBQPacketLib::SpeedUpPackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest)
{
	int nRetLen = 0; //Dest's buffer length

	const BYTE *pbSrc = (const BYTE *)pvSrc;
	BYTE* pTempSrc = new BYTE[nSrcLen];
	if(pTempSrc)
	{
		memset(pTempSrc, 0, nSrcLen);
	
		if(pvSrc)
		{
			// remove special character
			int nTempSrcLen = 0;
			for(int i =0; i< nSrcLen; i++)
			{
				if(pbSrc[i] == ESCAPE_CHAR)
				{
					pTempSrc[nTempSrcLen] = 0x5D ^ COMPLEMENT_CHAR;
				}
				else
					pTempSrc[nTempSrcLen] = pbSrc[i];
				nTempSrcLen++;
			}
			
			
			if(nTempSrcLen < 8)
				nRetLen = nTempSrcLen+4;
			else
				nRetLen = nTempSrcLen+4+4;

			*(BYTE **)pvDest = new BYTE[nRetLen];
			memcpy((*(BYTE **)pvDest)+3, pTempSrc, nTempSrcLen);
			*(*(BYTE **)pvDest) = (BYTE)chStart;
			*((*(BYTE **)pvDest)+1) = nTempSrcLen & 0x00FF;
			*((*(BYTE **)pvDest)+2) = (nTempSrcLen & 0xFF00) >> 8;

			BYTE byteCheck[4] ={0};
			if(nTempSrcLen>8)
			{
				for(int n =0; n<4; n++)
				{
					byteCheck[n] = pTempSrc[n] ^ pTempSrc[4+n];
				}

				for(int i=1; i< (int)((nTempSrcLen/4)-1);i++)
				{
					for(int n =0; n<4; n++)
					{
						byteCheck[n] = byteCheck[n] ^ pTempSrc[(i+1)*4+n];
					}
				}

				for(int n =0; n<4; n++)
				{
					*((*(BYTE **)pvDest)+(nRetLen-5+n)) = byteCheck[n];
				}
			}

			*((*(BYTE **)pvDest)+(nRetLen-1)) = (BYTE)chEnd;
		}
		

		delete [] pTempSrc;
		pTempSrc = NULL;
	}

	return nRetLen;
}

int CBQPacketLib::UnpackBuffer(const void *pvSrc, int nSrcLen, CHAR chStart, CHAR chEnd, void **pvDest){

	_ASSERTE(pvSrc != NULL);
	_ASSERTE(nSrcLen > 2 );
	_ASSERTE(*pvDest == NULL);
	//CCRCMachine oCRC;
	
	int nRetLen = 0;
	const BYTE *pbSrc = (const BYTE *)pvSrc;
	BYTE **pbDest = (BYTE **)pvDest;
	*pbDest = new BYTE[nSrcLen];
	ZeroMemory(*pbDest, nSrcLen);
	int i = 0;

	if ( nSrcLen <= 3 ){	// including CRC and end character
		delete[] *pbDest;
		*pbDest = NULL;
		return ERROR_CBQPACKETLIB_BUFFER_ERROR;
	}
	if ( chStart == START_CHAR ){
		// buffer returned by download mode
		if ( (*pbSrc) == START_CHAR ){
			i++;	// Read counter skip first start character.	
		}else{
			delete[] *pbDest;
			*pbDest = NULL;
			return ERROR_CBQPACKETLIB_CRC_ERROR;
		}
	}

	WORD wCRC;
	for(; i < nSrcLen; i++)
	{
		switch( *(pbSrc+i) )
		{
		// #define ESCAPE_CHAR		0x7D
		// #define END_CHAR			0x7E
		// #define COMPLEMENT_CHAR	0x20
		case ESCAPE_CHAR:
			i++;
			*((*pbDest)+nRetLen++) = *(pbSrc+i) ^ COMPLEMENT_CHAR;
			break;
		case END_CHAR:
			if(nRetLen < 2)	// CRC is two byte long, so if length is smaller than 2 bytes, then the buffer is not correct.
			{
				wCRC = 0;
			}
			else
			{
				wCRC = CCRCMachine::CRC16LCalculation((CHAR*)*pbDest, (WORD)(nRetLen * 8));
			}
			if(wCRC != CRC16LOK)
			{
				delete[] *pbDest;
				*pbDest = NULL;
				nRetLen = ERROR_CBQPACKETLIB_CRC_ERROR;
				return nRetLen;
			}
			break;
		default:
			*((*pbDest) + nRetLen++ ) = *(pbSrc+i);
			break;
		}
	}
	return nRetLen;
}