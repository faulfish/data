// Tool.h : interface of the CTool class
//

#pragma once

/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/

#define CHAR2BCD(x,y)			(((x-'0'))+((y-'0')<<4))
#define CHAR2HEXBCD(x,y)		(((x>='a') ? (x-'a'+10) : (x-'0'))+(((y>='a') ? (y-'a'+10) : (y-'0'))<<4))
#define HEXCHAR2BYTE(x,y)		((((x>='a') ? (x-'a'+10) : (x-'0'))<<4)+((y>='a') ? (y-'a'+10) : (y-'0')))

class CTool
{
public:
	static int BcdToDigit(BYTE *pucBcdBuf, int iBcdBufSize, char *sDigitBuf, int iDigitBufSize)
	{
		// do nothing if parameters are not correct.
		if ((pucBcdBuf == NULL) || (iBcdBufSize <= 0)) return 0;
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if (iBcdBufSize*2 > iDigitBufSize) return 0;

		BYTE ucLowByte, ucHighByte;
		int i;
		for ( i = 0; i < iBcdBufSize; i++)
		{
			ucLowByte = *(pucBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = ucLowByte+'0';
			if (*(sDigitBuf+(2*i))>'9')
				return 0;

			ucHighByte = (*(pucBcdBuf+i)&0xF0)>>4;
			*(sDigitBuf+(2*i+1)) = ucHighByte+'0';
			if (*(sDigitBuf+(2*i+1))>'9')
				return 0;
		}

		if (iBcdBufSize%2)
		{
			ucLowByte = *(pucBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = ucLowByte+'0';
			if (*(sDigitBuf+(2*i))>'9')
				return 0;
		}

		return (iBcdBufSize%2) ? (2*i+1) : (2*i);
	}

	static int DigitToBcdB(char *sDigitBuf, int iDigitBufSize, BYTE *pucBcdBuf, int iBcdBufSize, char cDummyDigit)
	{
		// do nothing if parameters are not correct.
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if ((pucBcdBuf == NULL) || (iBcdBufSize <= 0)) return 0;
		if (iDigitBufSize > iBcdBufSize*2) return 0;

		int i = 0, j = 0;

		if (iDigitBufSize%2)
		{
			*(pucBcdBuf+i) = (BYTE)CHAR2BCD(cDummyDigit, sDigitBuf[j]);
			i++; j++;
		}

		for (; j < iDigitBufSize; i++, j+=2)
			*(pucBcdBuf+i) = (BYTE)CHAR2BCD(sDigitBuf[j], sDigitBuf[j+1]);

		return i;
	}

	static int DigitToBcdE(char *sDigitBuf, int iDigitBufSize, BYTE *pucBcdBuf, int iBcdBufSize, char cDummyDigit)
	{
		// do nothing if parameters are not correct.
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if ((pucBcdBuf == NULL) || (iBcdBufSize <= 0)) return 0;
		if (iDigitBufSize > iBcdBufSize*2) return 0;
		int i;
		for ( i = 0; i < iDigitBufSize/2; i++)
			*(pucBcdBuf+i) = (BYTE)CHAR2BCD(sDigitBuf[2*i], sDigitBuf[2*i+1]);

		if (iDigitBufSize%2)
			*(pucBcdBuf+i) = (BYTE)CHAR2BCD(sDigitBuf[2*i], cDummyDigit);

		return i;
	}

	static int HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize)
	{
		// do nothing if parameters are not correct.
		if ((pucHexBcdBuf == NULL) || (iHexBcdBufSize <= 0)) return 0;
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if (iHexBcdBufSize*2 > iDigitBufSize) return 0;

		BYTE ucLowByte, ucHighByte;
		int i;
		for ( i = 0; i < iHexBcdBufSize; i++)
		{
			ucLowByte = *(pucHexBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');

			ucHighByte = (*(pucHexBcdBuf+i)&0xF0)>>4;
			*(sDigitBuf+(2*i+1)) = (ucHighByte>=10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');
		}	

		if (iHexBcdBufSize%2)
		{
			ucLowByte = *(pucHexBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');
		}

		return (iHexBcdBufSize%2) ? (2*i+1) : (2*i);
	}

	static int DigitToHexBcdB(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize, char cDummyDigit)
	{
		// do nothing if parameters are not correct.
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if ((pucHexBcdBuf == NULL) || (iHexBcdBufSize <= 0)) return 0;
		if (iDigitBufSize > iHexBcdBufSize*2) return 0;

		int i = 0, j = 0;

		if (iDigitBufSize%2)
		{
			*(pucHexBcdBuf+i) = (BYTE)Char2HexBcd(cDummyDigit, sDigitBuf[j]);
			i++; j++;
		}

		for (; j < iDigitBufSize; i++, j+=2)
			*(pucHexBcdBuf+i) = (BYTE)Char2HexBcd(sDigitBuf[j], sDigitBuf[j+1]);

		return i;
	}

	static int DigitToHexBcdE(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize, char cDummyDigit)
	{
		// do nothing if parameters are not correct.
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if ((pucHexBcdBuf == NULL) || (iHexBcdBufSize <= 0)) return 0;
		if (iDigitBufSize > iHexBcdBufSize*2) return 0;
		int i;
		for ( i = 0; i < iDigitBufSize/2; i++)
			*(pucHexBcdBuf+i) = (BYTE)Char2HexBcd(sDigitBuf[2*i], sDigitBuf[2*i+1]);

		if (iDigitBufSize%2)
			*(pucHexBcdBuf+i) = (BYTE)Char2HexBcd(sDigitBuf[2*i], cDummyDigit);

		return i;
	}

	static int HexToDigit(char *sHexBuf, int iHexBufSize, char *sDigitBuf, int iDigitBufSize)
	{
		// do nothing if parameters are not correct.
		if ((sHexBuf == NULL) || (iHexBufSize <= 0)) return 0;
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if (iHexBufSize*2 > iDigitBufSize) return 0;

		BYTE ucLowByte, ucHighByte;
		int i;
		for ( i = 0; i < (int)iHexBufSize; i++)
		{
			ucHighByte = (*(sHexBuf+i)&0xF0)>>4;
			*(sDigitBuf+(2*i)) = (ucHighByte >= 10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');

			ucLowByte = *(sHexBuf+i)&0x0F;
			*(sDigitBuf+(2*i+1)) = (ucLowByte >= 10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');
		}

		if (iHexBufSize%2)
		{
			ucHighByte = (*(sHexBuf+i)&0xF0)>>4;
			*(sDigitBuf+(2*i)) = (ucHighByte >= 10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');
		}

		return (iHexBufSize%2) ? (2*i+1) : (2*i);
	}

	static int DigitToHexB(char *sDigitBuf, int iDigitBufSize, char *sHexBuf, int iHexBufSize, char cDummyDigit)
	{
		// do nothing if parameters are not correct.
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if ((sHexBuf == NULL) || (iHexBufSize <= 0)) return 0;
		if (iDigitBufSize > iHexBufSize*2) return 0;

		int i = 0, j = 0;

		if (iDigitBufSize%2)
		{
			*(sHexBuf+i) = (BYTE)HexChar2Byte(cDummyDigit, sDigitBuf[j]);
			i++; j++;
		}

		for (; j < iDigitBufSize; i++, j+=2)
			*(sHexBuf+i) = (BYTE)HexChar2Byte(sDigitBuf[j], sDigitBuf[j+1]);

		return i;
	}

	static int DigitToHexE(char *sDigitBuf, int iDigitBufSize, char *sHexBuf, int iHexBufSize, char cDummyDigit)
	{
		// do nothing if parameters are not correct.
		if ((sDigitBuf == NULL) || (iDigitBufSize <= 0)) return 0;
		if ((sHexBuf == NULL) || (iHexBufSize <= 0)) return 0;
		if (iDigitBufSize > iHexBufSize*2) return 0;
		int i;
		for ( i = 0; i < iDigitBufSize/2; i++)
			*(sHexBuf+i) = (BYTE)HexChar2Byte(sDigitBuf[2*i], sDigitBuf[2*i+1]);

		if (iDigitBufSize%2)
			*(sHexBuf+i) = (BYTE)HexChar2Byte(sDigitBuf[2*i], cDummyDigit);

		return i;
	}

protected:
	static BYTE Char2HexBcd(BYTE ucX, BYTE ucY)
	{
		if ((ucX >= 'A') && (ucX <= 'F')) ucX += 0x20;
		if ((ucY >= 'A') && (ucY <= 'F')) ucY += 0x20;
		return CHAR2HEXBCD(ucX, ucY);
	}

	static BYTE HexChar2Byte(BYTE ucX, BYTE ucY)
	{
		if ((ucX >= 'A') && (ucX <= 'F')) ucX += 0x20;
		if ((ucY >= 'A') && (ucY <= 'F')) ucY += 0x20;
		return HEXCHAR2BYTE(ucX, ucY);
	}
};
