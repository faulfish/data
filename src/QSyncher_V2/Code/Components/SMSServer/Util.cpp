/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   Util.cpp  $
 *  $Revision:   1.10  $
 *  $Date:   Nov 09 2007 12:59:08  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/Util.cpp.-arc  $
   
      Rev 1.10   Nov 09 2007 12:59:08   Eddy Guo
   Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Util.cpp
    @brief {brief description of Util.cpp}

    {detail description of Util.cpp}
*/
#include "StdAfx.h"
#include "Util.h"
 
using namespace SMSUtil;
 
bool CUtil::HEXToDigit(BYTE* pHexBuf, UINT uiHexBufLen, BYTE* pDigitBuf) {
    UINT uiLowValue = 0;
    UINT uiHighValue = 0;
    BYTE byLowByte;
    BYTE byHighByte;
    UINT i = 0;
    for (i = 0; i < (UINT)uiHexBufLen / 2; i++) {
        byHighByte = (*(pHexBuf + i) & 0xF0) >> 4;
        if (byHighByte >= 10)
            *(pDigitBuf + 2 * i) = (byHighByte - 10) + 'A';
        else
            *(pDigitBuf + 2 * i) = byHighByte + '0';
        byLowByte = *(pHexBuf + i) & 0x0F;
        if (byLowByte >= 10)
            *(pDigitBuf + 2 * i + 1) = (byLowByte - 10) + 'A';
        else
            *(pDigitBuf + 2 * i + 1) = byLowByte + '0';
    }
    if (uiHexBufLen % 2) {
        byHighByte = (*(pHexBuf + i) & 0xF0) >> 4;
        if (byHighByte >= 10)
            *(pDigitBuf + 2 * i) = (byHighByte - 10) + 'A';
        else
            *(pDigitBuf + 2 * i) = byHighByte + '0';
    }
    *(pDigitBuf + 2 * i + 1) = '\0';
    return true;
}
 
 
UINT CUtil::DigitToHEX(BYTE* pHexBuf, CStringA cstrDigit, char chDummyDigit) {
    UINT iLength = cstrDigit.GetLength();
    UINT i = 0;
    for (i = 0; i < iLength / 2; i++)
        *(pHexBuf + i) = HEXCHAR2BYTE(cstrDigit.GetAt(2 * i), cstrDigit.GetAt(2 * i + 1));
    if (iLength % 2)
        *(pHexBuf + i) = HEXCHAR2BYTE(cstrDigit.GetAt(2 * i), chDummyDigit);
    return iLength;
}
 
 
bool CUtil::BCDToDigit(BYTE* pBcdBuf, UINT uiBcdBufLen, BYTE* pDigitBuf) {
    UINT uiLowValue = 0;
    UINT uiHighValue = 0;
    BYTE byLowByte;
    BYTE byHighByte;
    UINT i = 0;
    for (i = 0; i < (UINT)uiBcdBufLen / 2; i++) {
        byLowByte = *(pBcdBuf + i) & 0x0F;
        *(pDigitBuf + 2 * i) = byLowByte + '0' ;
        if (*(pDigitBuf + 2 * i) > '9')
            return false;
 
        byHighByte = (*(pBcdBuf + i) & 0xF0) >> 4;
        *(pDigitBuf + 2 * i + 1) = byHighByte + '0';
        if (*(pDigitBuf + 2 * i + 1) > '9')
            return false;
    }
 
    if (uiBcdBufLen % 2) {
        byLowByte = *(pBcdBuf + i) & 0x0F;
        *(pDigitBuf + 2 * i) = byLowByte + '0' ;
        if (*(pDigitBuf + 2 * i) > '9')
            return false;
    }
 
    *(pDigitBuf + 2 * i + 1) = '\0';
 
    return true;
}
 
 
UINT CUtil::DigitToBCD(BYTE* pBcdBuf, CStringA cstrDigit, char chDummyDigit) {
    UINT iLength = cstrDigit.GetLength();
    UINT i = 0;
    for (i = 0; i < iLength / 2; i++) {
        *(pBcdBuf + i) = (BYTE)CHAR2BCD(cstrDigit.GetAt(2 * i), cstrDigit.GetAt(2 * i + 1));
    }
 
    if (iLength % 2) {
        *(pBcdBuf + i) = (BYTE)CHAR2BCD(cstrDigit.GetAt(2 * i), chDummyDigit);
    }
 
    return iLength;
}
 
 
bool CUtil::HEXBCDToDigit(BYTE* pHexBcdBuf, UINT uiHexBcdBufLen, BYTE* pDigitBuf) {
    UINT uiLowValue = 0;
    UINT uiHighValue = 0;
    BYTE byLowByte;
    BYTE byHighByte;
    UINT i = 0;
    for (i = 0; i < (UINT)uiHexBcdBufLen / 2; i++) {
        byLowByte = *(pHexBcdBuf + i) & 0x0F;
        if (byLowByte >= 10)
            *(pDigitBuf + 2 * i) = (byLowByte - 10) + 'a';
        else
            *(pDigitBuf + 2 * i) = byLowByte + '0';
 
        byHighByte = (*(pHexBcdBuf + i) & 0xF0) >> 4;
        if (byHighByte >= 10)
            *(pDigitBuf + 2 * i + 1) = (byHighByte - 10) + 'a';
        else
            *(pDigitBuf + 2 * i + 1) = byHighByte + '0';
    }
 
    if (uiHexBcdBufLen % 2) {
        byLowByte = *(pHexBcdBuf + i) & 0x0F;
        if (byLowByte >= 10)
            *(pDigitBuf + 2 * i) = (byLowByte - 10) + 'a';
        else
            *(pDigitBuf + 2 * i) = byLowByte + '0';
    }
 
    *(pDigitBuf + 2 * i + 1) = '\0';
 
    return true;
}
 
 
UINT CUtil::DigitToHEXBCD(BYTE* pHexBcdBuf, CStringA cstrDigit, char chDummyDigit) {
    UINT iLength = cstrDigit.GetLength();
    UINT i = 0;
    for (i = 0; i < iLength / 2; i++) {
        *(pHexBcdBuf + i) = (BYTE)CHAR2HEXBCD(cstrDigit.GetAt(2 * i), cstrDigit.GetAt(2 * i + 1));
    }
 
    if (iLength % 2) {
        *(pHexBcdBuf + i) = (BYTE)CHAR2HEXBCD(cstrDigit.GetAt(2 * i), chDummyDigit);
    }
 
    return iLength;
}
 
 
UINT CUtil::GSMString2Bytes(const char* pSrc, BYTE* pDst, UINT nSrcLength) {
    for (UINT i = 0; i < nSrcLength; i += 2) {
        // 輸出高4位
        if (*pSrc >= '0' && *pSrc <= '9') {
            *pDst = (*pSrc - '0') << 4;
        } else {
            *pDst = (*pSrc - 'A' + 10) << 4;
        }
        pSrc++;
        // 輸出低4位
        if (*pSrc >= '0' && *pSrc <= '9') {
            *pDst |= *pSrc - '0';
        } else {
            *pDst |= *pSrc - 'A' + 10;
        }
        pSrc++;
        pDst++;
    }
    // 返回目標資料長度
    return nSrcLength / 2;
}
 
 
UINT CUtil::GSMBytes2String(const BYTE* pSrc, char* pDst, UINT nSrcLength) {
    // 0x00-0x0f lookup table
    const char tab[] = "0123456789ABCDEF";
    for (UINT i = 0; i < nSrcLength; i++) {
        // 輸出低4位
        *pDst++ = tab[*pSrc >> 4];
        // 輸出高4位
        *pDst++ = tab[*pSrc & 0x0f];
        pSrc++;
    }
    // 輸出字串加個結束符
    *pDst = '\0';
    // 返回目標字串長度
    return nSrcLength * 2;
 
}
 
 
UINT CUtil::Encode7Bit(char* inBuf, UINT inBufLen, char* outBuf) {
    BYTE multi = inBufLen / 8;
    BYTE left = inBufLen - 8 * multi;
    char* inputBufTmp = new char [inBufLen];
    UINT inputBufTmpLoc = inBufLen - 1;
    UINT outBufCurOffet = 0;
    BYTE idx = 0;
 
    for (UINT i = 0; i < (UINT)inBufLen; i++) {
        char inputByte = *(inBuf + i);
 
        inputBufTmp[(inBufLen - 1) - i] = inputByte;
    }
 
    for (UINT j = 0; j < multi; j++) {
        for (UINT k = 7; k > 0; k--) {
            char outputByte, outputBytePrev;
            BYTE indicator = 1;
 
            outputByte = inputBufTmp[inputBufTmpLoc];
            idx = 7 - k;
            outputByte >>= idx;
            outputBytePrev = inputBufTmp[inputBufTmpLoc - 1];
 
            for (UINT l = 0; l < idx; l++)
                indicator |= (1 << (l + 1));
 
            outputBytePrev &= indicator;
            outputBytePrev <<= (7 - idx);
            outputByte |= outputBytePrev;
            outBufCurOffet = 8 * (j + 1) - k - 1 - j;
            memcpy(outBuf + outBufCurOffet, &outputByte, 1);
 
            inputBufTmpLoc--;
        }
 
        inputBufTmpLoc--;
    }
 
    if (multi)
        outBufCurOffet++;
 
    if (left != 0) {
        for (UINT m = left; m > 0; m--) {
            char outputByte, outputBytePrev;
            BYTE indicator = 1;
 
            outputByte = inputBufTmp[inputBufTmpLoc];
            idx = left - m;
            outputByte >>= idx;
 
            if (inputBufTmpLoc != 0)
                outputBytePrev = inputBufTmp[inputBufTmpLoc - 1];
            else
                outputBytePrev = 0;
            for (UINT n = 0; n < idx; n++)
                indicator |= (1 << (n + 1));
            outputBytePrev &= indicator;
            outputBytePrev <<= (7 - idx);
            outputByte |= outputBytePrev;
            memcpy(outBuf + outBufCurOffet, &outputByte, 1);
 
            outBufCurOffet++;
            inputBufTmpLoc--;
        }
    }
    delete [] inputBufTmp;
    return outBufCurOffet;
}
 
 
// 編7bit code
bool CUtil::Encode7BitCode(BYTE* cBuf, BYTE* ExtByte) {
    BYTE cTmp = *cBuf;
    if (cTmp == 0x40)
        *cBuf = 0x00;
    else if (cTmp == 0xA3)
        *cBuf = 0x01;
    else if (cTmp == 0x24)
        *cBuf = 0x02;
    else if (cTmp == 0xA5)
        *cBuf = 0x03;
    else if (cTmp == 0xE8)
        *cBuf = 0x04;
    else if (cTmp == 0xE9)
        *cBuf = 0x05;
    else if (cTmp == 0xF9)
        *cBuf = 0x06;
    else if (cTmp == 0xEC)
        *cBuf = 0x07;
    else if (cTmp == 0xF2)
        *cBuf = 0x08;
    else if (cTmp == 0xC7)
        *cBuf = 0x09;
    else if (cTmp == 0xD8)
        *cBuf = 0x0B;
    else if (cTmp == 0xF8)
        *cBuf = 0x0C;
    else if (cTmp == 0xC5)
        *cBuf = 0x0E;
    else if (cTmp == 0xE5)
        *cBuf = 0x0F;
    else if (cTmp == 0x5F)
        *cBuf = 0x11;
        // some special character cannot be found
    else if (cTmp == 0x5E) {
        *cBuf = 0x1B;
        *ExtByte = 0x14;
 
        return true;
    } else if (cTmp == 0x7B) {
        *cBuf = 0x1B;
        *ExtByte = 0x28;
 
        return true;
    } else if (cTmp == 0x7D) {
        *cBuf = 0x1B;
        *ExtByte = 0x29;
 
        return true;
    } else if (cTmp == 0x5C) {
        *cBuf = 0x1B;
        *ExtByte = 0x2F;
 
        return true;
    } else if (cTmp == 0x5B) {
        *cBuf = 0x1B;
        *ExtByte = 0x3C;
 
        return true;
    } else if (cTmp == 0x7E) {
        *cBuf = 0x1B;
        *ExtByte = 0x3D;
 
        return true;
    } else if (cTmp == 0x5D) {
        *cBuf = 0x1B;
        *ExtByte = 0x3E;
 
        return true;
    } else if (cTmp == 0x7C) {
        *cBuf = 0x1B;
        *ExtByte = 0x40;
 
        return true;
    } // some special character cannot be found
    else if (cTmp == 0xC6)
        *cBuf = 0x1C;
    else if (cTmp == 0xE6)
        *cBuf = 0x1D;
    else if (cTmp == 0xDF)
        *cBuf = 0x1E;
    else if (cTmp == 0xC9)
        *cBuf = 0x1F;
    else if (cTmp == 0xA4)
        *cBuf = 0x24;
    else if (cTmp == 0xA1)
        *cBuf = 0x40;
    else if (cTmp == 0xC4)
        *cBuf = 0x5B;
    else if (cTmp == 0xD6)
        *cBuf = 0x5C;
    else if (cTmp == 0xD1)
        *cBuf = 0x5D;
    else if (cTmp == 0xDC)
        *cBuf = 0x5E;
    else if (cTmp == 0xA7)
        *cBuf = 0x5F;
    else if (cTmp == 0xBF)
        *cBuf = 0x60;
    else if (cTmp == 0xE4)
        *cBuf = 0x7B;
    else if (cTmp == 0xF6)
        *cBuf = 0x7C;
    else if (cTmp == 0xF1)
        *cBuf = 0x7D;
    else if (cTmp == 0xFC)
        *cBuf = 0x7E;
    else if (cTmp == 0xE0)
        *cBuf = 0x7F;
    return false;
}
 
 
UINT CUtil::Decode7Bit(char* inBuf, UINT inBufLen, char* outBuf, UINT outBufLen, BYTE nFillBit) {
    BYTE multi = inBufLen / 7;
    BYTE left = inBufLen - 7 * multi;
    char *inputBufTmp = new char [inBufLen];
    UINT inputBufTmpLoc = inBufLen - 1;
    UINT outBufCurOffet = 0;
    BYTE idx = 0;
    bool bIsExtension = FALSE;
    int iExtension = 0;
 
    for (int i = 0; i < (int)inBufLen; i++)	{
        char inputByte = *(inBuf + i);
        inputBufTmp[(inBufLen - 1) - i] = inputByte;
    }
 
    if (nFillBit) {
        char cFillBitPreByte, cFillBitCurByte;
        BYTE indicator = 0x01, and = 0x01;
 
        for (int z = 0; z < nFillBit; z++)
            indicator |= (1 << z);
 
        for (int y = 0; y < 8 - nFillBit; y++)
            and |= (1 << y);
 
        for (int x = 0; x < (int)inBufLen; x++) {
            cFillBitCurByte = inputBufTmp[x];
            cFillBitCurByte >>= nFillBit;
            cFillBitCurByte &= and;
 
            if (x > 0) {
                cFillBitPreByte &= indicator;
                cFillBitPreByte <<= (8 - nFillBit);
                cFillBitCurByte |= cFillBitPreByte;
            }
            cFillBitPreByte = inputBufTmp[x];
            inputBufTmp[x] = cFillBitCurByte;
        }
    }
 
    for (int j = 0; j < multi; j++) {
        for (int k = 7; k > -1; k--) {
            unsigned char outputByte, outputByteNext, uchTmp;
            BYTE indicator = 1;
 
            if (k == 0)
                outputByte = inputBufTmp[inputBufTmpLoc + 1];
            else
                outputByte = inputBufTmp[inputBufTmpLoc];
 
            idx = 7 - k;
 
            for (int l = 0; l < idx; l++)
                indicator |= (1 << (l + 1));
 
            indicator <<= (7 - idx);
            indicator = ~indicator;
            outputByte &= indicator;
            outputByte <<= idx;
 
            if (k != 7)
                outputByteNext = inputBufTmp[inputBufTmpLoc + 1];
            else
                outputByteNext = inputBufTmp[inputBufTmpLoc];
 
            uchTmp = outputByteNext & 128;
            outputByteNext &= 127;
 
            if (idx != 0) {
                outputByteNext >>= (8 - idx);
                for (int l = 0; l < 8 - idx; l++)
                    uchTmp /= 2;
 
                outputByteNext |= uchTmp;
            } else {
                outputByteNext = 0;
            }
            outputByte |= outputByteNext;
 
            bIsExtension = Decode7BitCode((BYTE *)&outputByte, bIsExtension);
 
            if (bIsExtension) {
                iExtension++;
            } else {
                memcpy(outBuf + outBufCurOffet, &outputByte, 1);
                outBufCurOffet++;
            }
 
            inputBufTmpLoc--;
        }
        inputBufTmpLoc++;
    }
 
    char cZero = 0x00;
 
    if ((outBufCurOffet != 0) && (memcmp(outBuf + outBufCurOffet - 1, &cZero, 1) == 0))
        outBufCurOffet--;
 
    if (left != 0) {
        for (int m = left; m > 0; m--) {
            unsigned char outputByte, outputByteNext, uchTmp;
            BYTE indicator = 1;
 
            outputByte = inputBufTmp[inputBufTmpLoc];
            idx = left - m;
 
            for (int n = 0; n < idx; n++)
                indicator |= (1 << (n + 1));
 
            indicator <<= (7 - idx);
            indicator = ~indicator;
            outputByte &= indicator;
            outputByte <<= idx;
 
            if (m != left)
                outputByteNext = inputBufTmp[inputBufTmpLoc + 1];
            else
                outputByteNext = inputBufTmp[inputBufTmpLoc];
 
            uchTmp = outputByteNext & 128;
            outputByteNext &= 127;
            outputByteNext >>= (8 - idx);
 
            for (int l = 0; l < 8 - idx; l++)
                uchTmp /= 2;
 
            outputByteNext |= uchTmp;
            outputByte |= outputByteNext;
 
            bIsExtension = Decode7BitCode((BYTE *)&outputByte, bIsExtension);
 
            if (bIsExtension) {
                iExtension++;
            } else {
                memcpy(outBuf + outBufCurOffet, &outputByte, 1);
                outBufCurOffet++;
            }
 
            inputBufTmpLoc--;
        }
    }
    outBufCurOffet--;
    delete [] inputBufTmp;
    return iExtension;
}
 
 
// 解7bit code
bool CUtil::Decode7BitCode(BYTE* cBuf, bool bIsExtension) {
    BYTE cTmp = *cBuf;
    if (!bIsExtension) {
        switch (cTmp) {
            case 0x00:
            *cBuf = 0x40; break;
            case 0x01:
            *cBuf = 0xA3; break;
            case 0x02:
            *cBuf = 0x24; break;
            case 0x03:
            *cBuf = 0xA5; break;
            case 0x04:
            *cBuf = 0xE8; break;
            case 0x05:
            *cBuf = 0xE9; break;
            case 0x06:
            *cBuf = 0xF9; break;
            case 0x07:
            *cBuf = 0xEC; break;
            case 0x08:
            *cBuf = 0xF2; break;
            case 0x09:
            *cBuf = 0xC7; break;
            case 0x0B:
            *cBuf = 0xD8; break;
            case 0x0C:
            *cBuf = 0xF8; break;
            case 0x0E:
            *cBuf = 0xC5; break;
            case 0x0F:
            *cBuf = 0xE5; break;
            case 0x11:
            *cBuf = 0x5F; break;
            case 0x1B:
            return true;        // this is an extenstion character
            case 0x1C:
            *cBuf = 0xC6; break;
            case 0x1D:
            *cBuf = 0xE6; break;
            case 0x1E:
            *cBuf = 0xDF; break;
            case 0x1F:
            *cBuf = 0xC9; break;
            case 0x24:
            *cBuf = 0xA4; break;
            case 0x40:
            *cBuf = 0xA1; break;
            case 0x5B:
            *cBuf = 0xC4; break;
            case 0x5C:
            *cBuf = 0xD6; break;
            case 0x5D:
            *cBuf = 0xD1; break;
            case 0x5E:
            *cBuf = 0xDC; break;
            case 0x5F:
            *cBuf = 0xA7; break;
            case 0x60:
            *cBuf = 0xBF; break;
            case 0x7B:
            *cBuf = 0xE4; break;
            case 0x7C:
            *cBuf = 0xF6; break;
            case 0x7D:
            *cBuf = 0xF1; break;
            case 0x7E:
            *cBuf = 0xFC; break;
            case 0x7F:
            *cBuf = 0xE0; break;
        }
    } else {
        switch (cTmp) {
            case 0x14:
            *cBuf = 0x5E; break;
            case 0x28:
            *cBuf = 0x7B; break;
            case 0x29:
            *cBuf = 0x7D; break;
            case 0x2F:
            *cBuf = 0x5C; break;
            case 0x3C:
            *cBuf = 0x5B; break;
            case 0x3D:
            *cBuf = 0x7E; break;
            case 0x3E:
            *cBuf = 0x5D; break;
            case 0x40:
            *cBuf = 0x7C; break;
        }
    }
    return false;
}
//bool CUtil::Decode7BitCode(BYTE *cBuf, bool bIsExtension)
//{
//    BYTE cTmp = *cBuf;
//    if (!bIsExtension)
//    {
//        if (cTmp == 0x00)
//            *cBuf = 0x40;
//        else if (cTmp == 0x01)
//            *cBuf = 0xA3;
//        else if (cTmp == 0x02)
//            *cBuf = 0x24;
//        else if (cTmp == 0x03)
//            *cBuf = 0xA5;
//        else if (cTmp == 0x04)
//            *cBuf = 0xE8;
//        else if (cTmp == 0x05)
//            *cBuf = 0xE9;
//        else if (cTmp == 0x06)
//            *cBuf = 0xF9;
//        else if (cTmp == 0x07)
//            *cBuf = 0xEC;
//        else if (cTmp == 0x08)
//            *cBuf = 0xF2;
//        else if (cTmp == 0x09)
//            *cBuf = 0xC7;
//        else if (cTmp == 0x0B)
//            *cBuf = 0xD8;
//        else if (cTmp == 0x0C)
//            *cBuf = 0xF8;
//        else if (cTmp == 0x0E)
//            *cBuf = 0xC5;
//        else if (cTmp == 0x0F)
//            *cBuf = 0xE5;
//        else if (cTmp == 0x11)
//            *cBuf = 0x5F;
//        else if (cTmp == 0x1B)
//            return true;        // this is an extenstion character
//        else if (cTmp == 0x1C)
//            *cBuf = 0xC6;
//        else if (cTmp == 0x1D)
//            *cBuf = 0xE6;
//        else if (cTmp == 0x1E)
//            *cBuf = 0xDF;
//        else if (cTmp == 0x1F)
//            *cBuf = 0xC9;
//        else if (cTmp == 0x24)
//            *cBuf = 0xA4;
//        else if (cTmp == 0x40)
//            *cBuf = 0xA1;
//        else if (cTmp == 0x5B)
//            *cBuf = 0xC4;
//        else if (cTmp == 0x5C)
//            *cBuf = 0xD6;
//        else if (cTmp == 0x5D)
//            *cBuf = 0xD1;
//        else if (cTmp == 0x5E)
//            *cBuf = 0xDC;
//        else if (cTmp == 0x5F)
//            *cBuf = 0xA7;
//        else if (cTmp == 0x60)
//            *cBuf = 0xBF;
//        else if (cTmp == 0x7B)
//            *cBuf = 0xE4;
//        else if (cTmp == 0x7C)
//            *cBuf = 0xF6;
//        else if (cTmp == 0x7D)
//            *cBuf = 0xF1;
//        else if (cTmp == 0x7E)
//            *cBuf = 0xFC;
//        else if (cTmp == 0x7F)
//            *cBuf = 0xE0;
//    }
//    else
//    {
//        if (cTmp == 0x14)
//            *cBuf = 0x5E;
//        else if (cTmp == 0x28)
//            *cBuf = 0x7B;
//        else if (cTmp == 0x29)
//            *cBuf = 0x7D;
//        else if (cTmp == 0x2F)
//            *cBuf = 0x5C;
//        else if (cTmp == 0x3C)
//            *cBuf = 0x5B;
//        else if (cTmp == 0x3D)
//            *cBuf = 0x7E;
//        else if (cTmp == 0x3E)
//            *cBuf = 0x5D;
//        else if (cTmp == 0x40)
//            *cBuf = 0x7C;
//    }
//    return false;
//}
 
 
// 編碼UCS2, big5或是其他的MBCS編碼轉unicode
UINT CUtil::EncodeUCS2(char* inBuf, UINT inBufLen, char* outBuf) {
    UINT ucs2num;
    BYTE* msgTxt = new BYTE [(inBufLen + 1) * 2];
    wchar_t* ucs2Buf = new wchar_t [inBufLen + 1];
    wchar_t* pu16 = NULL;
    BYTE* pu8 = NULL;
 
    /* Convert to UCS2 string to get the character numbers, note that
       the return value of MultiByteToWideChar() include null char */
    ucs2num = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, inBuf, -1, ucs2Buf, inBufLen * 2);
 
    pu8 = msgTxt;
    pu16 = ucs2Buf;
 
    for (UINT i = 0; i < ucs2num; i++, pu8++, pu16++) {
        *pu8 = (BYTE)(*pu16 >> 8);
        pu8++;
        *pu8 = (BYTE)(*pu16 & 0x00FF);
    }
 
    memcpy(outBuf, msgTxt, ucs2num * 2);
 
    delete ucs2Buf;
    delete msgTxt;
 
    return (ucs2num - 1) * 2;
}
 
 
// 解UCS2, 主要用到WideCharToMultiByte, unicode轉big5
UINT CUtil::DecodeUCS2(char* inBuf, UINT inBufLen, char* outBuf) {
    char* inputUCS2 = inBuf;
    char* outputTmpBuf = new char [inBufLen + 2];
    ZeroMemory(outputTmpBuf, inBufLen + 2);
    /* Note that ucs2num doesn't include the NULL terminated */
    UINT ucs2num = inBufLen / 2;
 
    char* ucs2Buf = new char [inBufLen + 2];
    ZeroMemory(ucs2Buf, inBufLen + 2);
    for (UINT i = 0; i < ucs2num; i++) {
        ucs2Buf[2 * i] = (inputUCS2[2 * i + 1]);
        ucs2Buf[2 * i + 1] = inputUCS2[2 * i];
    }
 
    ucs2Buf[inBufLen] = 0;
    ucs2Buf[inBufLen + 1] = 0;
 
    UINT n8BitLen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)ucs2Buf, ucs2num + 1, outputTmpBuf, 0, NULL, NULL);
 
    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)ucs2Buf, ucs2num + 1, outputTmpBuf, n8BitLen, NULL, NULL);
 
    memcpy(outBuf, outputTmpBuf, inBufLen);
 
    delete ucs2Buf;
    delete outputTmpBuf;
 
    return ucs2num;
}
 
 
/////////////////////////////////////////////////////////////////////
// input: CString InputString
// output: 回傳一個UINT值
// ussage: 傳入一個0-F的16進位的字元, 回傳相對應的數值(UINT),
//         判斷是不是UCS2
/////////////////////////////////////////////////////////////////////
bool CUtil::IsUCS2(CStringA InputString) {
    bool IsUCS2 = false;
    UINT UINTLen = InputString.GetLength();
    for (UINT i = 0; i < UINTLen; i++) {
        wchar_t chTmp = InputString.GetAt(i);
        if (chTmp > 0x80) IsUCS2 = true;
    }
    return IsUCS2;
}
 
 
/////////////////////////////////////////////////////////////////////
// input: char *inBuf, UINT inBufLen, char *outBuf
// output: 回傳一個UINT值
// ussage: 解8bit
/////////////////////////////////////////////////////////////////////
UINT CUtil::Decode8BitFcn(char* inBuf, UINT inBufLen, char* outBuf) {
    for (UINT i = 0; i < (UINT)inBufLen; i++) {
        char inputByte = *(inBuf + i);
        memcpy(outBuf + (inBufLen - 1) - i, &inputByte, 1);
    }
    return inBufLen;
}
 
 
/////////////////////////////////////////////////////////////////////
// input: char c
// output: 回傳一個UINT值
// ussage: 傳入一個0-F的16進位的字元, 回傳相對應的數值(UINT)
/////////////////////////////////////////////////////////////////////
inline UINT CUtil::hex2dec(char c) {
    return ('A' <= (UINT)c && (UINT)c <= 'F') ? (c - 'A' + 10) : (c - '0');
}
 
 
// 給ConactenateMessages用, 回傳指定的reference number, current message number
CDecodedMessage* CUtil::GetDecodedMessageByCurrentNumber
    (CList<CDecodedMessage*>* DecodedMessages, UINT nReferenceNumber, UINT currentNumber, UINT nTotalMessageCount) {
    ASSERT(DecodedMessages);
    CDecodedMessage* retVal = NULL;;
    POSITION pos = DecodedMessages->GetHeadPosition();
    while (pos) {
        retVal = DecodedMessages->GetNext(pos);
        if (retVal->m_nReferenceNumber == nReferenceNumber &&
            retVal->m_nCurrentNumber == currentNumber &&
            retVal->m_nTotalMessageCount == nTotalMessageCount)
            return retVal;
    }
    TRACE("GetDecodedMessageByCurrentNumber return NULL!!!\n");
    return retVal;
}
 
 
/////////////////////////////////////////////////////////////////////
// input: UINT IDs[], UINT IDCount,
//        CList<CMessageObject>& ShortMessagesPoll,
//        CList<CDecodedMessage>& DecodedMessages
// output: 傳回bool值代表成功或失敗
// ussage: 傳入IDs代表要被Merge的message id, IDCount代表一共有多少筆
//         從ShortMessagesPoll裡組合後放到DecodedMessage裡, 其作法是
//         先把所有在Decoded裡頭的Message組合起來就完成了, 因為會各別
//         decoded完成
/////////////////////////////////////////////////////////////////////
bool CUtil::ConactenateMessages(UINT IDs[], UINT IDCount, CList<CMessageObject*>* ShortMessages, CList<CDecodedMessage*>* DecodedMessages) {
    // TODO: 實作多page的組合, 直接把DecodedMessage裡的東西合起來就可
    //       注意IDs裡一定要存放所有被merge起來的id
    ASSERT(DecodedMessages);
    ASSERT(ShortMessages);
    //if(DecodedMessages->IsEmpty() || ShortMessages->IsEmpty()) return false;
 
    if (IDCount == 0) {  // IDCount為0時代表組合所有的multi-page message
        UINT reference_number = 0;
        UINT current_number = 0;
        UINT total_message_count = 0;
        POSITION pos1;
        CDecodedMessage* mainMessage;
        CDecodedMessage* subMessage;
        pos1 = DecodedMessages->GetHeadPosition();
        while (pos1) {
            mainMessage = DecodedMessages->GetNext(pos1);
            // ↓如果只有一個page不重組
            if (mainMessage->m_nTotalMessageCount == 1 ||
                mainMessage->m_nTotalMessageCount == 0) {
                mainMessage->m_bIsConactenated = true;
                mainMessage->m_bIsComplete = true;
                mainMessage->m_nTotalMessageCount = 1;
                continue;
            }
            // ↓找第一個page
            if (mainMessage->m_nTotalMessageCount > 1 &&
                mainMessage->m_nCurrentNumber == 1) { // 第一個page
                reference_number = mainMessage->m_nReferenceNumber;
                current_number = mainMessage->m_nCurrentNumber;
                total_message_count = mainMessage->m_nTotalMessageCount;
                // 開始找第i個page, 必須符合三個條件↑
                UINT IDIndex = 1;
                for (UINT i = 2; i <= total_message_count; ++i) {
                    subMessage = GetDecodedMessageByCurrentNumber(DecodedMessages, reference_number, i, total_message_count);
                    if (subMessage != NULL) {
                        mainMessage->m_strMessage += subMessage->m_strMessage;
                        mainMessage->m_pMessageIDs[IDIndex++] =
                            subMessage->m_pMessageIDs[0];
                    }
                }
                mainMessage->m_bIsConactenated = true;
                if (IDIndex == total_message_count) mainMessage->m_bIsComplete = true;
                else
                    mainMessage->m_bIsComplete = false;
            }  // ←尋找current bunber為1
        } // ←結束尋找current number為1
    } else { // IDCount不為0時只組合指定ID的message, 當然可以亂指, 只是結果會很慘
        // TODO: 暫時不實作, 若實作注意下面刪除其他sub message
    }
    POSITION pos = DecodedMessages->GetHeadPosition();
    CList<CDecodedMessage*>* retList = new CList<CDecodedMessage*>();
    while (pos) {
        CDecodedMessage* dm = DecodedMessages->GetNext(pos);
        if (dm->m_bIsConactenated) {
            retList->AddTail(dm);
            CDecodedMessage* cdm = retList->GetHead();
        } else {
            //ASSERT(false);
            delete dm;
        }
    }
    TRACE("在DecodedMessages->RemoveAll()之前的size: %d\n", DecodedMessages->GetSize());
    DecodedMessages->RemoveAll();
    TRACE("在DecodedMessages->RemoveAll()之後的size: %d\n", DecodedMessages->GetSize());
    DecodedMessages->AddTail(retList);
    TRACE("加入新的List(retList)之後的size: %d\n", DecodedMessages->GetSize());
    retList->RemoveAll();
    delete retList;
    return true;
}
 
/////////////////////////////////////////////////////////////////////
// input: char* pdu, UINT length, _PDU& out
// output: 回傳值寫在out裡
// ussage: 傳入byte string(ex. 0222365FF001A2A3A), 解開每一個部分, 回
//         傳值寫入out裡, 參考_PDU定義
/////////////////////////////////////////////////////////////////////
bool CUtil::PDUParser(char* pdu, UINT length, _PDU& out) {
    // TODO: 參考以前的code就可以了, 注意每個欄位的比對
    UINT smsLength = 0;              // SMS Length, including typeofaddress
    UINT typeofaddress = 0;          // SMS Type of Address
    CStringA phonenumber;           // Sender Address
    CStringA servicecenter;         // SC Address
    char smsdeliverstatus;          // First octet of this SMS-DELIVER message.
    UINT nSenderAddressLength = 0;    // Sender Address Length
    UINT tppid = 0;                  // TP-PID Protocal Identifier
    UINT tpdcs = 0;                  // TP-DCS Data Coding Scheme
    UINT tpudl = 0;                  // User Data Length
    UINT tpnMessageReference = 0;     // TP Message Reference
    UINT udh_length = 0;             // User Data Head Length
    UINT total_messages = 0;         // 一共有多少page
    UINT reference_number = 0;       // 指定一個唯一的數字, 給多page用
    UINT current_message = 0;        // 現在是第幾筆
    UINT nYear = 0;                   // 年
    UINT nMonth = 0;                  // 月
    UINT nDay = 0;                    // 日
    UINT nHour = 0;                   // 時
    UINT min = 0;                    // 分
    UINT sec = 0;                    // 秒
    UINT timezone;                   // 時區
    CStringA timestamp;             // 時間標記
    UINT nValidityPeriod = 0;         // 傳送時間
    CStringA message;               // User Data
    out.nCurrentnMessageID = 0;
    out.nReferenceNumber = 0;
    out.nTotalMessageCount = 1;
    out.nUserDataLength = 0;
    out.nUserDataHeaderLength = 0;
    // PDU的index
    UINT pos = 0;
    // SMS length
    smsLength = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nSMSCLength = smsLength;
 
    // SC address type
    typeofaddress = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nSenderAddressType = typeofaddress;
 
    // service center address
    servicecenter = "";
    for (UINT i = 0; i < smsLength * 2 - 2; i += 2) {
        servicecenter += (char)(pdu[pos + 1]);
        servicecenter += (char)(pdu[pos]);
        pos += 2;
    }
    if (typeofaddress == 145) servicecenter = "+" + servicecenter;
    servicecenter.Remove('F'); servicecenter.Remove('f');
    out.straSCAddress = servicecenter;
 
    // sms deliver status
    smsdeliverstatus = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nFirstOctectOfMessage = smsdeliverstatus;
 
    // 如果有message reference要抓出來, message reference是submit才有
    if ((smsdeliverstatus & 0x01) == 1) {
        tpnMessageReference = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
        out.nMessageReference = tpnMessageReference;
    }
 
    // sender address length
    nSenderAddressLength = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nSenderAddressLength = nSenderAddressLength;
    // sender address type
    typeofaddress = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nSenderAddressType = typeofaddress;
    // sender address
    phonenumber = _T("");
    for (UINT i = 0; i < nSenderAddressLength; i += 2) {
        phonenumber += (char)(pdu[pos + 1]);
        phonenumber += (char)(pdu[pos]);
        pos += 2;
    }
    if (typeofaddress == 145) phonenumber = "+" + phonenumber;
    phonenumber.Remove('F'); phonenumber.Remove('f');
    out.straSenderAddress = phonenumber;
 
    // TP-PID
    tppid = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nProtocolIdentifier = tppid;
 
    // TP-DCS
    tpdcs = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nDataCodingScheme = tpdcs;
 
    // 年月日時分秒時區
    out.TimeStamp.nYear = 0;
    out.TimeStamp.nMonth = 0;
    out.TimeStamp.nDay = 0;
    out.TimeStamp.nHour = 0;
    out.TimeStamp.nMinute = 0;
    out.TimeStamp.nSecond = 0;
    if ((smsdeliverstatus & 0x03) == 0) { // inbox有年月日時分秒時區等資訊
        timestamp = _T("");
        out.TimeStamp.nYear = nYear = (hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos])); pos += 2;
        out.TimeStamp.nMonth = nMonth = (hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos])); pos += 2;
        out.TimeStamp.nDay = nDay = (hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos])); pos += 2;
        out.TimeStamp.nHour = nHour = (hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos])); pos += 2;
        out.TimeStamp.nMinute = min = (hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos])); pos += 2;
        out.TimeStamp.nSecond = sec = (hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos])); pos += 2;
        out.TimeStamp.nTimeZone = timezone = (UINT)(hex2dec(pdu[pos + 1]) * 10 + hex2dec(pdu[pos]));
        pos += 2;
    } else {
        nValidityPeriod = hex2dec(pdu[pos++]) * 10 + hex2dec(pdu[pos++]);
        out.nValidityPeriod = nValidityPeriod;
    }
 
    // User Data Length
    tpudl = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
    out.nUserDataLength = tpudl;
    out.nUserDataHeaderLength = 0;
    // 檢查是不是有user data header, 檢查smsdeliverstatus
    if ((smsdeliverstatus & 0x40) > 0) { // 有
        // User Data Header Length
        udh_length = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
        out.nUserDataHeaderLength = udh_length;
        // ↓記錄最後結束的位置
        UINT endofud = pos + udh_length;
        while (pos < endofud) {
            UINT iei = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
            UINT iedl = hex2dec(pdu[pos++]) * 16 + hex2dec(pdu[pos++]);
            if (iei == 0 || iei == 8) { //conactenated messages
                reference_number = 0;
                if (iei == 0x08 && iedl == 0x04) // 這裡有可能是兩個byte的reference number
                    reference_number += hex2dec(pdu[pos++]) * 16 * 16 * 16 +
                        hex2dec(pdu[pos++]) * 16 * 16;
                reference_number = hex2dec(pdu[pos++]) * 16 +
                    hex2dec(pdu[pos++]);
                out.nReferenceNumber = reference_number;
                total_messages = hex2dec(pdu[pos++]) * 16 +
                    hex2dec(pdu[pos++]);
                out.nTotalMessageCount = total_messages;
                current_message = hex2dec(pdu[pos++]) * 16 +
                    hex2dec(pdu[pos++]);
                out.nCurrentnMessageID = current_message;
            } else { // unknow identifier
                pos = pos + iedl;
            }
            if (udh_length == 5) { // Know how to handl
            }
        } // end of while loop
        if (tppid == 0) tpudl = tpudl - (UINT)((((udh_length + 1) * 8) + 6) / 7);
    } // end of check user data header
    char* szUserData = &(pdu[pos]);
    TRACE("PDU: %s\n", pdu);
    TRACE("PUD Message: %s\n", szUserData);
    char* r = new char[strlen(szUserData) / 2 + 1];   // 從string轉成byte, 長度少一半
    memset(r, 0, strlen(szUserData) / 2 + 1);
    UINT byteLength = GSMString2Bytes(szUserData, (BYTE*)r, (UINT)strlen(szUserData));
    TRACE("%s\n", r);
    out.szUserData = new char[out.nUserDataLength + 10];
    memset(out.szUserData, 0, out.nUserDataLength + 10);
    char* szStrTmp = new char[out.nUserDataLength + 10];
    memset(szStrTmp, 0, out.nUserDataLength + 10);
    // 下面可以先不要解開, 可以等到呼叫DecodedMessages再完成所有的解碼工作
    // 基本上, 只要是standard的message, 解到這裡UCS2就可以不用再解, 而7bit
    // 的只要再換成wide char就可以show出在UI上面了
    if ((tpdcs & 0x08) > 0) {
        DecodeUCS2(r, (UINT)strlen(szUserData) / 2, szStrTmp);
        memcpy(out.szUserData, szStrTmp, (UINT)strlen(szUserData) / 2);
    } else {
        if(reference_number != 0 && total_messages != 0 && current_message != 0) {
            Decode7Bit(r, out.nUserDataLength, szStrTmp, out.nUserDataLength - out.nUserDataHeaderLength - 2, (14 - out.nUserDataHeaderLength) % 8);
            memcpy(out.szUserData, szStrTmp, out.nUserDataLength - out.nUserDataHeaderLength - 2);
        } else {
            Decode7Bit(r, out.nUserDataLength, szStrTmp, out.nUserDataLength, 0);
            memcpy(out.szUserData, szStrTmp, out.nUserDataLength);
        }
    }
    TRACE("result string: %s\n", out.szUserData);
    TRACE("szStrTmp: %s\n", szStrTmp);
    //::WideCharToMultiByte(CP_ACP, 0, (WCHAR*)r, strlen(szUserData), out.szUserData, byteLength, 0, 0);
    delete[] r;
    r = NULL;
    delete[] szStrTmp;
    szStrTmp = NULL;
    return true;
}
 
/////////////////////////////////////////////////////////////////////
// input: char* CMGLRet, UINT length, SM* sm, bool isstraAlpha
// output: 傳入一筆原始的message, 其中一定包含有如下的資訊, 首先去除
//         "+CMGL:", 之後就是真正的資料, 個別分解每個token
/////////////////////////////////////////////////////////////////////
bool CUtil::ParsrPDUHeader(char* CMGLRet, UINT length, SM* sm, bool isIncludingAlpha) {
    // TODO: 參考以前的code, SM的部分只有四個, 傳回的格式如下, 所以要抓出前四個
    // +CMGL:<index>,<stat>,[<straAlpha>],<length><CR><LF><pdu>
    //       [<CR><LF>+CMGL:<index>,<stat>,[<straAlpha>], <length><CR><LF><pdu>[...]]
    //       注意使用方法
    CStringA tokenString = CMGLRet;
    tokenString.TrimLeft("+CMGL: ");
    //char* str = new char[tokenString.GetLength() + 1];
    //memset(str, 0, tokenString.GetLength() + 1);
    //strncpy_s(str, tokenString.GetLength(), tokenString, tokenString.GetLength());
    char* delim = ",\r\n";
    char* id, * loc, * straAlpha, * len, * pdu;
    char* nextToken;
    // index
    if ((id = strtok_s(CMGLRet, delim, &nextToken)) != NULL)
        sm->nMessageID = atoi(id);
    else
        ASSERT(false);
    // location
    if ((loc = strtok_s(NULL, delim, &nextToken)) != NULL)
        sm->Location = mapIntToMsgLoc(atoi(loc));
    else
        ASSERT(false);
    // straAlpha
    if (isIncludingAlpha) {
        if ((straAlpha = strtok_s(NULL, delim, &nextToken))) {
            sm->straAlpha = straAlpha;
        }
    } else {
        sm->straAlpha = "";
    }
    // length
    if ((len = strtok_s(NULL, delim, &nextToken)) != NULL)
        sm->nLength = atoi(len);
    else
        ASSERT(false);
    // Undecoded pdu
    if ((pdu = strtok_s(NULL, delim, &nextToken)) != NULL)
        sm->straUnDecodedPDU = pdu;
    else
        ASSERT(false);
    TRACE("CUtil::ParsrPDUHeader==>\n");
    TRACE("id: %s\n", id);
    TRACE("loc: %s\n", loc);
    TRACE("straAlpha: %s\n", straAlpha);
    TRACE("length: %s\n", len);
    TRACE("pdu: %s\n", pdu);
    return true;
}
 
 
// 傳入字串, 字串長度, 收件人電話, 傳出字串, 傳出字串長度
bool CUtil::PDUComposer(char* msg, UINT msgLength, CStringA sender, char* out, UINT& outLength) {
    // TODO: 參考以前的code就可以了, 加上ComposePDUHeader的code就可以組合
    //       一個完整的pdu, 在這裡應該就是可以寫入或是傳送的message了
    return false;
}
 
 
// 組合PDU的header, 不包含內容, 可在之後加上
BYTE CUtil::ComposePDUHeader(char* outBuf, bool bStatusReq, bool bDataHeader, bool bReplyPath, BYTE AddressLen, bool bIsInter, char* cAddress, bool bIsUCS2, BYTE DataLen) {
    BYTE AddressValue;
    BYTE SubmitFirstValue = 17;
    char cTmp;
    char charTmp[2];
 
    if (AddressLen % 2) AddressValue = AddressLen + 1;
    else                AddressValue = AddressLen;
 
    BYTE outBufTmpLen = 2 * (1 /* fisrt byte */ + 1 /* TP-MR */
        + 1 /* Address length */ + 1 /* Address type */ +
        (AddressValue / 2) /* Adress value */ + 1 /* PID */ + 1 /* DCS */
        + 1 /* Valid period */ + 1/* User data length */);
 
    char* outBufTmp = new char [outBufTmpLen];
    ZeroMemory(outBufTmp, outBufTmpLen);
 
    // first byte
    if (bStatusReq) SubmitFirstValue |= (1 << 5);
 
    if (bDataHeader) SubmitFirstValue |= (1 << 6);
 
    if (bReplyPath) SubmitFirstValue |= (1 << 7);
 
    ZeroMemory(charTmp, 2);
    HEXToDigit(&SubmitFirstValue, 2, (BYTE*)charTmp);
    memcpy(outBufTmp, charTmp, 2);
 
    // TP-MR
    outBufTmp[2] = 0x30;
    outBufTmp[3] = 0x30;
 
    // Address length
    ZeroMemory(charTmp, 2);
    HEXToDigit(&AddressLen, 2, (BYTE*)charTmp);
    memcpy(outBufTmp + 4, charTmp, 2);
 
    // Address type
    if (bIsInter) {
        outBufTmp[6] = 0x39;
        outBufTmp[7] = 0x31;
    } else {
        outBufTmp[6] = 0x38;
        outBufTmp[7] = 0x31;
    }
 
    // Address value
    UINT iCount = 0;
 
    for (int i = 0; i < AddressValue / 2; i++) {
        memcpy(&outBufTmp[7 + 2 * (i + 1)], cAddress + 2 * i, 1);
        iCount++;
        if (iCount == AddressLen) {
            cTmp = 'F';
            memcpy(&outBufTmp[7 + 2 * i + 1], &cTmp, 1);
        } else {
            memcpy(&outBufTmp[7 + 2 * i + 1], cAddress + 2 * i + 1, 1);
            iCount++;
        }
    }
 
    // PID
    outBufTmp[8 + AddressValue] = 0x30;
    outBufTmp[9 + AddressValue] = 0x30;
 
    // DCS
    if (bIsUCS2) {
        outBufTmp[10 + AddressValue] = 0x31;
        outBufTmp[11 + AddressValue] = 0x39;
    } else {
        outBufTmp[10 + AddressValue] = 0x46;
        outBufTmp[11 + AddressValue] = 0x31;
    }
 
    // Valid period
    outBufTmp[12 + AddressValue] = 0x46;
    outBufTmp[13 + AddressValue] = 0x46;
 
    // Data length
    ZeroMemory(charTmp, 2);
    HEXToDigit(&DataLen, 2, (BYTE*)charTmp);
    memcpy(outBufTmp + 14 + AddressValue, charTmp, 2);
 
    memcpy(outBuf, outBufTmp, outBufTmpLen);
 
    delete [] outBufTmp;
 
    return outBufTmpLen;
}
 
 
// 分解BQ message, 分解完後傳回一個BQSM, 先不實作, 利用ParseBQSMHeader
UINT CUtil::BQSMParser(char* in, UINT msgLength, BQSM& out) {
 
    //return ParseBQSMHeader(in, msgLength, &out);
    return false;
}
 
// 傳入字串, 字串長度, 收件人電話, 傳出字串, 傳出字串長度
// 目前先使用standard的, 一樣可以傳送簡訊
bool CUtil::BQSMComposer(char* msg, UINT msgLength,
                         CStringA strReceiver, char* out,
                         UINT& outLength, BYTE nLocation)
{
    char MessageData[1600] = {0};
    CStringA year, month, day, hour, minute, second;
    char cTimeTmp[5] = {0};
    BOOL bIsDataheader = FALSE;
    int numMsg = 1;
    if(msgLength > 141) {
        numMsg = (msgLength / MAX_CON_8BIT_LEN) + (msgLength % MAX_CON_8BIT_LEN) ? 1 : 0;
        bIsDataheader = TRUE;
    }
 
    GetCurrentDateTime(year, month, day, hour, minute, second);
    // 年
    DigitToBCD((BYTE*)cTimeTmp, year);
    memcpy(MessageData + 5, cTimeTmp + 1, 1);
    memcpy(MessageData + 6, cTimeTmp, 1);
    // 月
    ZeroMemory(cTimeTmp, 5);
    DigitToBCD((BYTE*)cTimeTmp, month);
    memcpy(MessageData + 7, cTimeTmp, 1);
    // 日
    ZeroMemory(cTimeTmp, 5);
    DigitToBCD((BYTE*)cTimeTmp, day);
    memcpy(MessageData + 8, cTimeTmp, 1);
    // 時
    ZeroMemory(cTimeTmp, 5);
    DigitToBCD((BYTE*)cTimeTmp, hour);
    memcpy(MessageData + 9, cTimeTmp, 1);
    // 分
    ZeroMemory(cTimeTmp, 5);
    DigitToBCD((BYTE*)cTimeTmp, minute);
    memcpy(MessageData + 10, cTimeTmp, 1);
    // 秒
    ZeroMemory(cTimeTmp, 5);
    DigitToBCD((BYTE*)cTimeTmp, second);
    memcpy(MessageData + 11, cTimeTmp, 1);
    // location
    memcpy(MessageData + 12, &nLocation, 1);
    // Byte 1
    MessageData[13] = (char)((numMsg) ? 0xF1 : 0xB1);
    // TON type "+"
    MessageData[14] = (char)((strReceiver.GetAt(0) == '+') ? 0x01 : 0x00);
    // Address Length
    MessageData[15] = (char)strReceiver.GetLength();
    int x = strReceiver.GetLength() * 2;
    // Address
    char *cAddress = new char[x + 2];
    memset(cAddress, 0, x + 2);
    for(int i = 0; i < strReceiver.GetLength(); ++i)
        cAddress[2 * i] = strReceiver.GetAt(i);
    memcpy(MessageData + 16, cAddress, x);
    delete [] cAddress;
    // Data Coding Scheme
    MessageData[16 + x] = (char)(IsUCS2(msg) ? 0x02 : 0x00);
    // Total Message Page
    MessageData[19 + x] = (char)numMsg;
    int n = 20 + x;
    // Complete
    MessageData[22 + x] = (char)0x01;
    // Total Number
    MessageData[27 + x] = (char)numMsg;
 
    return false;
}
 
 
UINT CUtil::ParseBQSMHeader(char* inBuf, UINT inDataBufLen, BQSM* Msg, UINT uiMessageID) {
 
    UINT outDataBufLen = 0;
    char cMsgIDTmp[4] = {0};
    char cTimeTmp[10] = {0};
    char cNumPacketTmp[4] = {0};
    char cSizeTmp[4] = {0};
    UINT iTotalSize = 0, iSizeTmp = 0;
 
    // BQSM ID
    //for(UINT i = 0; i < 4; i++)
    //    cMsgIDTmp[3 - i] = *(inBuf + i);
    Msg->MsgHdr.uiMsgID = uiMessageID;
    //memcpy(&Msg->MsgHdr.uiMsgID, cMsgIDTmp, 4);
 
 
    // BQSM Type
    memcpy(&Msg->MsgHdr.MsgType, inBuf + 4, 1);
 
    // Time - nYear
    ZeroMemory(cTimeTmp, 5);
    HEXBCDToDigit((BYTE*)(inBuf + 6), 2, (BYTE*)(cTimeTmp));
    HEXBCDToDigit((BYTE*)(inBuf + 5), 2, (BYTE*)(cTimeTmp + 2));
    Msg->MsgHdr.SentTime.nYear = atoi(cTimeTmp);
 
    // Time - nMonth
    ZeroMemory(cTimeTmp, 5);
    HEXBCDToDigit((BYTE*)(inBuf + 7), 2, (BYTE*)(cTimeTmp));
    Msg->MsgHdr.SentTime.nMonth = atoi(cTimeTmp);
 
    // Time - nDay
    ZeroMemory(cTimeTmp, 5);
    HEXBCDToDigit((BYTE*)(inBuf + 8), 2, (BYTE*)(cTimeTmp));
    Msg->MsgHdr.SentTime.nDay = atoi(cTimeTmp);
 
    // Time - nHour
    ZeroMemory(cTimeTmp, 5);
    HEXBCDToDigit((BYTE*)(inBuf + 9), 2, (BYTE*)(cTimeTmp));
    Msg->MsgHdr.SentTime.nHour = atoi(cTimeTmp);
 
    // Time - nMinute
    ZeroMemory(cTimeTmp, 5);
    HEXBCDToDigit((BYTE*)(inBuf + 10), 2, (BYTE*)(cTimeTmp));
    Msg->MsgHdr.SentTime.nMinute = atoi(cTimeTmp);
 
    // Time - nSecond
    ZeroMemory(cTimeTmp, 5);
    HEXBCDToDigit((BYTE*)(inBuf + 11), 2, (BYTE*)(cTimeTmp));
    Msg->MsgHdr.SentTime.nSecond = atoi(cTimeTmp);
 
    // Loc
    memcpy(&Msg->MsgHdr.Loc, inBuf + 12, 1);
 
    // Byte 1
    memcpy(&Msg->MsgHdr.ContentHdr.Byte1, inBuf + 13, 1);
 
    // TON
    memcpy(&Msg->MsgHdr.ContentHdr.TON, inBuf + 14, 1);
 
    // Address length
    memcpy(&Msg->MsgHdr.ContentHdr.AddrLen, inBuf + 15, 1);
    UINT x = Msg->MsgHdr.ContentHdr.AddrLen * 2;
    // Address
    if(*(inBuf + 16 + 1) == 0x00) {
        UINT j = 0;
        for (j = 0; j < Msg->MsgHdr.ContentHdr.AddrLen; j++)
            memcpy(&Msg->MsgHdr.ContentHdr.Addr[j], inBuf + 16 + 2 * j, 1);
 
        Msg->MsgHdr.ContentHdr.Addr[j] = '\0';
    } else {
        char outBuf[40];
        ZeroMemory(outBuf, 40);
        // 解UCS2
        DecodeUCS2((char*)inBuf + 16, x, outBuf);
        memcpy(Msg->MsgHdr.ContentHdr.Addr, outBuf, x);
    }
 
    // DCS
    memcpy(&Msg->MsgHdr.ContentHdr.DCS, inBuf + 16 + x, 1);
 
    // Reference number
    memcpy(&Msg->MsgHdr.ContentHdr.ref, inBuf + 17 + x, 1);
 
    // PID
    memcpy(&Msg->MsgHdr.ContentHdr.pid, inBuf + 18 + x, 1);
 
    // Max number
    memcpy(&Msg->MsgHdr.ContentHdr.max, inBuf + 19 + x, 1);
 
    // Current index number
    memcpy(&Msg->MsgHdr.ContentHdr.idx, inBuf + 20 + x, 1);
 
    // Is read
    memcpy(&Msg->MsgHdr.bIsRead, inBuf + 21 + x, 1);
 
    // Is completed
    memcpy(&Msg->MsgHdr.bISCompleted, inBuf + 22 + x, 1);
 
    // Is DRM
    memcpy(&Msg->MsgHdr.bIsDRM, inBuf + 23 + x, 1);
 
    // Number of packet
    for(UINT k = 0; k < 4; k++)
        cNumPacketTmp[3 - k] = *(inBuf + 24 + x + k);
 
    memcpy(&Msg->MsgHdr.iNumPacket, cNumPacketTmp, 4);
 
    // Total BQSM content size
    for (UINT m = 0; m < Msg->MsgHdr.iNumPacket; m++) {
        ZeroMemory(cSizeTmp, 4);
 
        for (UINT n = 0; n < 4; n++)
            cSizeTmp[3 - n] = *(inBuf + 28 + 8 * m + x + iTotalSize + n);
 
        memcpy(&iSizeTmp, cSizeTmp, 4);
 
        iTotalSize += iSizeTmp;
    }
 
    // BQSM content from size
    Msg->MsgData = (BYTE*)(inBuf + 28 + x);
 
    return iTotalSize;
}
 
bool CUtil::DecodeMessages(CList<CMessageObject*>* Messages, CList<CDecodedMessage*>* DecodedMessages) {
    // 先delete所有在DecodedMessages裡的訊息
    // 再解開所有的ShortMessage存到DecodedMessages裡
    POSITION pos = DecodedMessages->GetHeadPosition();
    while (pos) {
        CDecodedMessage* cdm = DecodedMessages->GetNext(pos);
        delete cdm;
    }
    DecodedMessages->RemoveAll();
    pos = Messages->GetHeadPosition();
    while (pos) {
        CMessageObject* cmo = Messages->GetNext(pos);
        if (cmo->m_MessageType == STD_MESSAGE) {
            CSM* csm = dynamic_cast<CSM*>(cmo);
            CDecodedMessage* cdm = new CDecodedMessage();
            cdm->m_nCurrentNumber = csm->m_ShortMessage.PDU.nCurrentnMessageID;
            cdm->m_nReferenceNumber = csm->m_ShortMessage.PDU.nReferenceNumber;
            cdm->m_nTotalMessageCount = csm->m_ShortMessage.PDU.nTotalMessageCount;
            cdm->m_bIsConactenated = false;
            cdm->m_pMessageIDs[0] = csm->m_ShortMessage.nMessageID;
            cdm->m_MessageLocation = csm->m_ShortMessage.Location;
            UINT nSize = csm->m_ShortMessage.PDU.straSenderAddress.GetLength() + 1;
            char* strMultiByte = new char[nSize];
            memset(strMultiByte, 0, nSize);
            strcpy_s(strMultiByte, nSize, csm->m_ShortMessage.PDU.straSenderAddress);
            WCHAR* strWideChar = new WCHAR[nSize];
            memset(strWideChar, 0, sizeof(WCHAR) * nSize);
            MultiByteToWideChar(CP_ACP, 0, strMultiByte, nSize, strWideChar, nSize);
            cdm->m_strSenderNumber = strWideChar;
            if ((csm->m_ShortMessage.PDU.nDataCodingScheme & 0x08) > 0) {   // UCS2
                // 轉換成為TCHAR, 但是長度會少一半
                cdm->m_strMessage = (TCHAR*)(csm->m_ShortMessage.PDU.szUserData);
            } else {    // 7bit
                char* out = new char[csm->m_ShortMessage.PDU.nUserDataLength];
                //Decode7Bit (csm->m_ShortMessage.PDU.szUserData,
                //            csm->m_ShortMessage.PDU.nszUserDataLength, out, 0);
                WCHAR* strWideChar = new
                    WCHAR[csm->m_ShortMessage.PDU.nUserDataLength + 1];
                memset(strWideChar, 0, sizeof(WCHAR) *
                    (csm->m_ShortMessage.PDU.nUserDataLength + 1));
                // 7bit換轉換成Unicode編碼........
                // ::MultiByteToWideChar
                MultiByteToWideChar(CP_ACP, 0, out, csm->m_ShortMessage.PDU.nUserDataLength, strWideChar, csm->m_ShortMessage.PDU.nUserDataLength);
                cdm->m_strMessage = strWideChar;
            }
        } else if (cmo->m_MessageType == BQ_MESSAGE) {
            CBQSM* cbqsm = dynamic_cast<CBQSM*>(cmo);
            CDecodedMessage* cdm = new CDecodedMessage();
            cdm->m_pMessageIDs[0] = cbqsm->m_BQShortMessage.MsgHdr.uiMsgID;
            cdm->m_MessageLocation = (enumMsgLoc)cbqsm->m_BQShortMessage.MsgHdr.Loc;
            DecodedBQMessage(cbqsm, *cdm, cbqsm->m_BQShortMessage.MsgLen[0]);
            cdm->m_bIsComplete = cbqsm->m_bIsComplete;
            cdm->m_bIsConactenated = true;
            cdm->m_nCurrentNumber = 0;
            cdm->m_nTotalMessageCount = 1;
            DecodedMessages->AddTail(cdm);
            // 未完成.
        }
    }
    return false;
}
 
BOOL CUtil::IsEMS(char *inBuf, UINT iSize) {
    BYTE nHeaderLen = (BYTE)*inBuf;
    BYTE nIEI, nIELen = 0, nPos = 0;
    if(nHeaderLen > iSize) return FALSE;
    nIEI = *(inBuf + 1);
    nIELen = *(inBuf + 2);
    nPos = *(inBuf + 3);
    if(nIEI == 0x00) {
        if (nHeaderLen == 5) return FALSE;
        nIEI = *(inBuf + 6);
        nIELen = *(inBuf + 7);
        nPos = *(inBuf + 8);
    } else if (nIEI == 0x08) {
        if (nHeaderLen == 6) return FALSE;
        nIEI = *(inBuf + 7);
        nIELen = *(inBuf + 8);
        nPos = *(inBuf + 9);
    }
    if((nIEI == EMS_TEXT_FORMAT) && ((nIELen == 4) || (nIELen == 3)) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_PRE_SOUND) && (nIELen == 2) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_USER_SOUND) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_PRE_ANIMATION) && (nIELen == 2) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_LARGE_ANIMATION) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_SMALL_ANIMATION) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_LARGE_PIC) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_SMALL_PIC) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_VAR_PIC) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_USER_PROMPT_IND) && (nIELen == 1))
        return TRUE;
    if((nIEI == EMS_EXT_OBJ) && (nIELen < nHeaderLen))
        return TRUE;
    if((nIEI == EMS_REUSED_EXT_OBJ) && (nIELen == 3))
        return TRUE;
    if((nIEI == EMS_COMPRESSION_CTRL) && (nIELen < nHeaderLen))
        return TRUE;
    if((nIEI == EMS_OBJ_DISTRIBUTION_IND) && (nIELen == 2))
        return TRUE;
    if((nIEI == EMS_STD_WVG_OBJ) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    if((nIEI == EMS_CHAR_SIZE_WVG_OBJ) && (nIELen < nHeaderLen) && (nPos < 140))
        return TRUE;
    return FALSE;
}
 
 
BYTE CUtil::GetObjCount(char *inBuf, UINT iSize) {
    BYTE nIEI, nIELen = 0;
    BYTE nOffset = 0;
    BYTE nObjCount = 0;
    while (nOffset < iSize) {
        nIEI = *(inBuf + nOffset);
        nIELen = *(inBuf + nOffset + 1);
        nObjCount++;
        nOffset += nIELen;
        nOffset += 2;
    }
    return nObjCount;
}
 
 
UINT CUtil::ReCheckPacketNum(BQSM* pMsg) {
    UINT iRealNumPacket = 0;
    for(UINT i = 0; i < pMsg->MsgHdr.iNumPacket; i++) {
        if(pMsg->MsgLen[i]!=0) iRealNumPacket++;
    }
    pMsg->MsgHdr.iNumPacket = iRealNumPacket;
    return iRealNumPacket;
}
 
// parse all tokens in s string, using these delimiter to separate
bool CUtil::GetToken(TCHAR* str, UINT strLength, TCHAR* delim, UINT delLength, vector<CString> &outTokenVec) {
    TCHAR *next_token;
    TCHAR *token = _tcstok_s(str, delim, &next_token);
    while(token != NULL) {
        outTokenVec.push_back(token);
        token = _tcstok_s(str, delim, &next_token);
        outTokenVec.push_back(token);
    }
    return true;
}
 
// change endian
inline bool CUtil::ChangeEndian(char* szDst, char* szSrc, UINT nSrcLength) {
    if(nSrcLength % 2 != 0) ASSERT(false);
    for(UINT i = 0; i < nSrcLength; i += 2) {
        szDst[i] = szSrc[i+1];
        szDst[i+1] = szSrc[i];
    }
    return false;
}
 
 
void CUtil::DecodedBQMessage(CBQSM* pMessage, CDecodedMessage& decodedMessage, int nOriginalLength) {
    UINT szMsgDataLangth = (UINT)strlen((char*)pMessage->m_BQShortMessage.MsgData);
    decodedMessage.m_TimeStamp = pMessage->m_BQShortMessage.MsgHdr.SentTime;
    wchar_t szSender[100] = {0};
    ::MultiByteToWideChar(CP_ACP, 0, pMessage->m_BQShortMessage.MsgHdr.ContentHdr.Addr, (int)strlen(pMessage->m_BQShortMessage.MsgHdr.ContentHdr.Addr), szSender, 100);
    decodedMessage.m_strSenderNumber = szSender;
    switch(pMessage->m_BQShortMessage.MsgHdr.ContentHdr.DCS) {
        case 0x00: { // 7bit
            char* szRet = new char[nOriginalLength+2];
            memset(szRet, 0, nOriginalLength+2);
            CUtil::Decode7Bit((char*)pMessage->m_BQShortMessage.MsgData,
                szMsgDataLangth, szRet, nOriginalLength+2, 0);
            wchar_t* szDecodedMsg = new wchar_t[nOriginalLength+1];
            memset(szDecodedMsg, 0, sizeof(wchar_t)*(nOriginalLength+1));
            ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szRet, nOriginalLength, szDecodedMsg, nOriginalLength);
            decodedMessage.m_strMessage = szDecodedMsg;
            delete[] szDecodedMsg;
            delete[] szRet;
        }
        break;
        case 0x01: { // ASCII(8bit)
            // 原有的8bit應該可以直接轉成Unicode就可以秀出來
            ASSERT(false);
        }
        break;
        case 0x02: { // UCS2
            char* szRet = new char[_tcslen((TCHAR*)pMessage->m_BQShortMessage.MsgData)*2+2];
            memset(szRet, 0, _tcslen((TCHAR*)pMessage->m_BQShortMessage.MsgData)*2+2);
            // 如果是unicode, change endian就可以了
            // 如果不是unicode就必須再轉一次, 使用::WideCharToMultiByte
            ChangeEndian(szRet, (char*)pMessage->m_BQShortMessage.MsgData, (UINT)_tcslen((TCHAR*)pMessage->m_BQShortMessage.MsgData)*2+2);
            CString strMsg = (wchar_t*)szRet;
            decodedMessage.m_strMessage = strMsg;
            delete[] szRet;
        }
        break;
    }
}
 
void CUtil::DecodedSTDMessage(CSM* pMessage, CDecodedMessage& decodedMessage) {
}
 
// 取得目前的時間, 傳回年月時時分秒
bool CUtil::GetCurrentDateTime(CStringA &strYear, CStringA &strMonth, CStringA &strDay,
                               CStringA &strHour, CStringA &strMinute, CStringA &strSecond)
{
    CTime time = CTime::GetCurrentTime();
    strYear.Format("%.4d", time.GetYear());
    strMonth.Format("%.2d", time.GetMonth());
    strDay.Format("%.2d", time.GetDay());
    strHour.Format(".2d", time.GetHour());
    strMinute.Format("%.2d", time.GetMinute());
    strSecond.Format("%.2d", time.GetSecond());
    return true;
}
 
void CUtil::ExtractObjectFromBQSM(BQSM* pBqsm, UINT& iTotalSize, UINT& iMsgDataSize, UINT& iSizeTmp, UINT& iSizeTag, UINT& iSizeObjTag, UINT& iOffset, char* cSizeTmp, UINT& iMsgObjDataSize) {
    for(UINT x = 0; x < pBqsm->MsgHdr.iNumPacket; x++) {
        iSizeTmp = 0;
        ZeroMemory(cSizeTmp, 4);
        for(UINT m = 0; m < 4; m++) cSizeTmp[3 - m] = *(pBqsm->MsgData + 8 * x + iOffset + m);
        memcpy(&iSizeTmp, cSizeTmp, 4);
        pBqsm->MsgLen[x] = (BYTE)iSizeTmp;
        iMsgDataSize += iSizeTmp;
        ParseObjectFromBQSM(pBqsm, //iTotalSize,
            iSizeTmp, iSizeTag,
            iSizeObjTag, x, iOffset,
            cSizeTmp, iMsgObjDataSize);
        iOffset += pBqsm->MsgLen[x];
    }
}
 
 
void CUtil::ParseObjectFromBQSM(BQSM* pBqsm, UINT& iSizeTmp, UINT& iSizeTag, UINT& iSizeObjTag, UINT& x, UINT& iOffset, char* cSizeTmp, UINT& iMsgObjDataSize)
{
    BOOL bIsEMS = FALSE;
    if(bIsEMS = IsEMS((char *)(pBqsm->MsgData + 8 * (x + 1) + iOffset), iSizeTmp)) {
        pBqsm->bIsEMS = TRUE;
        UINT iSizeTmpHeader = 0;
        ZeroMemory(cSizeTmp, 4);
        cSizeTmp[0] = *(pBqsm->MsgData + 8 * x + iOffset + 8);
        memcpy(&iSizeTmpHeader, cSizeTmp, 4);
        if (pBqsm->MsgHdr.iNumPacket > 1) {
            if (*(pBqsm->MsgData + 8 * x + iOffset + 9) == IEI_CON_16BIT) iSizeTmpHeader -= CON_16BIT_LEN;
            else iSizeTmpHeader -= CON_8BIT_LEN;
        }
        pBqsm->MsgObjLen[x] = (BYTE)iSizeTmpHeader;
        iMsgObjDataSize += pBqsm->MsgObjLen[x];
        BYTE nObjCount = 0;
        if (pBqsm->MsgHdr.iNumPacket > 1) {
            if (*(pBqsm->MsgData + 8 * x + iOffset + 9) == IEI_CON_16BIT)
                nObjCount = GetObjCount((char *)(pBqsm->MsgData + 8 * (x + 1) + iOffset + USER_DATA_HEADER_LEN + CON_16BIT_LEN), pBqsm->MsgObjLen[x]);
            else
                nObjCount = GetObjCount((char *)(pBqsm->MsgData + 8 * (x + 1) + iOffset + USER_DATA_HEADER_LEN + CON_8BIT_LEN), pBqsm->MsgObjLen[x]);
        } else {
            nObjCount = GetObjCount((char *)(pBqsm->MsgData + 8 * (x + 1) + iOffset + USER_DATA_HEADER_LEN), pBqsm->MsgObjLen[x]);
        }
        pBqsm->MsgObjCount[x] = nObjCount;
    }
}
 
void CUtil::GetMessageObject7Bit(BQSM* pBqsm, UINT iSizeTag, UINT iSizeObjTag, UINT iOffset, BYTE* m_MsgData, BYTE* m_MsgObjData, UINT& l)
{
    UINT iObjLen = pBqsm->MsgObjLen[l];
    UINT iBlockSize = pBqsm->MsgLen[l];
    UINT iMsgLen = pBqsm->MsgLen[l];
    if (l == pBqsm->MsgHdr.iNumPacket - 1) {
        if (pBqsm->bIs16BitConcatenated) {
            if(iMsgLen) memcpy(m_MsgData + iSizeTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_16BIT_LEN + iObjLen, pBqsm->MsgLen[l] - (USER_DATA_HEADER_LEN + CON_16BIT_LEN +  iObjLen) - 1);
            if (iObjLen) memcpy(m_MsgObjData + iSizeObjTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_16BIT_LEN, iObjLen);
        } else {
            if(iMsgLen) memcpy(m_MsgData + iSizeTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_8BIT_LEN + iObjLen, pBqsm->MsgLen[l] - (USER_DATA_HEADER_LEN + CON_8BIT_LEN + iObjLen) - 1);
            if (iObjLen) memcpy(m_MsgObjData + iSizeObjTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_8BIT_LEN, iObjLen);
        }
    } else {
        if (pBqsm->bIs16BitConcatenated) {
            UINT iL = (iBlockSize - 8) % 8;
            UINT iCopy = ((iBlockSize - 8) / 8);
            iCopy *= 7;
            iCopy += iL;
            iCopy -= iObjLen;
            if(iMsgLen) memcpy(m_MsgData + iSizeTag, pBqsm->MsgData + 8 * (l + 1) + iOffset + USER_DATA_HEADER_LEN + CON_16BIT_LEN + iObjLen, iCopy);
            if (iObjLen) memcpy(m_MsgObjData + iSizeObjTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_16BIT_LEN, iObjLen);
            iOffset += pBqsm->MsgLen[l];
            iSizeTag += iCopy;
        } else {
            UINT iL = (iBlockSize - 7) % 8;
            UINT iCopy = ((iBlockSize - 7) / 8);
            iCopy *= 7;
            iCopy += iL;
            iCopy -= iObjLen;
            if(iMsgLen) memcpy(m_MsgData + iSizeTag, pBqsm->MsgData + 8 * (l + 1) + iOffset + USER_DATA_HEADER_LEN + CON_8BIT_LEN + iObjLen, iCopy);
            if (iObjLen) memcpy(m_MsgObjData + iSizeObjTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_8BIT_LEN, iObjLen);
            iOffset += pBqsm->MsgLen[l];
            iSizeTag += iCopy;
        }
        iSizeObjTag += iObjLen;
    }
 
}
 
void CUtil::GetMessageObject16Bit(BQSM* pBqsm, UINT& iSizeTag, UINT& iSizeObjTag, UINT& iOffset, BYTE* m_MsgData, BYTE* m_MsgObjData, UINT& l)
{
    UINT iObjLen = pBqsm->MsgObjLen[l];
    UINT iMsgLen = pBqsm->MsgLen[l];
    if (pBqsm->bIs16BitConcatenated) {
        if(iMsgLen) memcpy(m_MsgData + iSizeTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_16BIT_LEN + iObjLen, pBqsm->MsgLen[l] - (USER_DATA_HEADER_LEN + CON_16BIT_LEN + iObjLen));
        if(iObjLen) memcpy(m_MsgObjData + iSizeObjTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_16BIT_LEN, iObjLen);
    } else {
        if(iMsgLen) memcpy(m_MsgData + iSizeTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_8BIT_LEN + iObjLen, pBqsm->MsgLen[l] - (USER_DATA_HEADER_LEN + CON_8BIT_LEN + iObjLen));
        if(iObjLen) memcpy(m_MsgObjData + iSizeObjTag, pBqsm->MsgData + iOffset + 8 * (l + 1) + USER_DATA_HEADER_LEN + CON_8BIT_LEN, iObjLen);
    }
    iOffset += pBqsm->MsgLen[l];
    iSizeTag += pBqsm->MsgLen[l];
    iSizeTag -= pBqsm->MsgObjLen[l];
    if (pBqsm->bIs16BitConcatenated) iSizeTag -= 7;
    else iSizeTag -= 6;
    iSizeObjTag += iObjLen;
}
