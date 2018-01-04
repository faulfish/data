#include "StdAfx.h"
#include "CodecUtil.h"

CCodecUtil::CCodecUtil(void)
{
}

CCodecUtil::~CCodecUtil(void)
{
}


/////////////////////////////////////////////////////////////////////
// input:   const char* pSrc, BYTE* pDst, int nSrcLength
// output:  int (byte length)
// ussage:  傳入Byte String轉成Byte, 也就是把16進位(00~FF文字)轉成
//          BYTE(0~255數值)
/////////////////////////////////////////////////////////////////////
int CCodecUtil::GSMString2Bytes(const char* pSrc, BYTE* pDst, int nSrcLength) {
    for (int i = 0; i < nSrcLength; i += 2) {
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

/////////////////////////////////////////////////////////////////////
// input:   const BYTE* pSrc, char* pDst, int nSrcLength
// output:  int (string length)
// ussage:  傳入字串轉成byte string也就是原有的BYTE(0~255數值)轉成字
//          串(16進位的字串..00~FF)
/////////////////////////////////////////////////////////////////////
int CCodecUtil::GSMBytes2String(const BYTE* pSrc, char* pDst, int nSrcLength) {
    // 0x00-0x0f lookup table
    const char tab[] = "0123456789ABCDEF";
    for (int i = 0; i < nSrcLength; i++) {
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

bool CCodecUtil::HEXToDigit(BYTE* pHexBuf, UINT uiHexBufLen, BYTE* pDigitBuf) {
    UINT uiLowValue = 0;
    UINT uiHighValue = 0;
    BYTE byLowByte;
    BYTE byHighByte;
    int i = 0;
    for (i = 0; i < (int)uiHexBufLen / 2; i++) {
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


UINT CCodecUtil::DigitToHEX(BYTE* pHexBuf, CStringA cstrDigit, char chDummyDigit) {
    int iLength = cstrDigit.GetLength();
    int i = 0;
    for (i = 0; i < iLength / 2; i++)
        *(pHexBuf + i) = HEXCHAR2BYTE(cstrDigit.GetAt(2 * i), cstrDigit.GetAt(2 * i + 1));
    if (iLength % 2)
        *(pHexBuf + i) = HEXCHAR2BYTE(cstrDigit.GetAt(2 * i), chDummyDigit);
    return iLength;
}


bool CCodecUtil::BCDToDigit(BYTE* pBcdBuf, UINT uiBcdBufLen, BYTE* pDigitBuf) {
    UINT uiLowValue = 0;
    UINT uiHighValue = 0;
    BYTE byLowByte;
    BYTE byHighByte;
    int i = 0;
    for (i = 0; i < (int)uiBcdBufLen / 2; i++) {
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


UINT CCodecUtil::DigitToBCD(BYTE* pBcdBuf, CStringA cstrDigit, char chDummyDigit) {
    int iLength = cstrDigit.GetLength();
    int i = 0;
    for (i = 0; i < iLength / 2; i++) {
        *(pBcdBuf + i) = (BYTE)CHAR2BCD(cstrDigit.GetAt(2 * i), cstrDigit.GetAt(2 * i + 1));
    }
    if (iLength % 2) {
        *(pBcdBuf + i) = (BYTE)CHAR2BCD(cstrDigit.GetAt(2 * i), chDummyDigit);
    }
    return iLength;
}


bool CCodecUtil::HEXBCDToDigit(BYTE* pHexBcdBuf, UINT uiHexBcdBufLen, BYTE* pDigitBuf) {
    UINT uiLowValue = 0;
    UINT uiHighValue = 0;
    BYTE byLowByte;
    BYTE byHighByte;
    int i = 0;
    for (i = 0; i < (int)uiHexBcdBufLen / 2; i++) {
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


UINT CCodecUtil::DigitToHEXBCD(BYTE* pHexBcdBuf, CStringA cstrDigit, char chDummyDigit) {
    int iLength = cstrDigit.GetLength();
    int i = 0;
    for (i = 0; i < iLength / 2; i++) {
        *(pHexBcdBuf + i) = (BYTE)CHAR2HEXBCD(cstrDigit.GetAt(2 * i), cstrDigit.GetAt(2 * i + 1));
    }

    if (iLength % 2) {
        *(pHexBcdBuf + i) = (BYTE)CHAR2HEXBCD(cstrDigit.GetAt(2 * i), chDummyDigit);
    }

    return iLength;
}

UINT CCodecUtil::Encode7Bit(char* inBuf, UINT inBufLen, char* outBuf) {
    BYTE multi = inBufLen / 8;
    BYTE left = inBufLen - 8 * multi;
    char* inputBufTmp = new char [inBufLen];
    int inputBufTmpLoc = inBufLen - 1;
    int outBufCurOffet = 0;
    BYTE idx = 0;

    for (int i = 0; i < (int)inBufLen; i++) {
        char inputByte = *(inBuf + i);

        inputBufTmp[(inBufLen - 1) - i] = inputByte;
    }

    for (int j = 0; j < multi; j++) {
        for (int k = 7; k > 0; k--) {
            char outputByte, outputBytePrev;
            BYTE indicator = 1;

            outputByte = inputBufTmp[inputBufTmpLoc];
            idx = 7 - k;
            outputByte >>= idx;
            outputBytePrev = inputBufTmp[inputBufTmpLoc - 1];

            for (int l = 0; l < idx; l++)
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
        for (int m = left; m > 0; m--) {
            char outputByte, outputBytePrev;
            BYTE indicator = 1;

            outputByte = inputBufTmp[inputBufTmpLoc];
            idx = left - m;
            outputByte >>= idx;

            if (inputBufTmpLoc != 0)
                outputBytePrev = inputBufTmp[inputBufTmpLoc - 1];
            else
                outputBytePrev = 0;
            for (int n = 0; n < idx; n++)
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
bool CCodecUtil::Encode7BitCode(BYTE* cBuf, BYTE* ExtByte) {
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


UINT CCodecUtil::Decode7Bit(char* inBuf, UINT inBufLen, char* outBuf, int outBufLen, BYTE nFillBit) {
    BYTE multi = inBufLen / 7;
    BYTE left = inBufLen - 7 * multi;
    char* inputBufTmp = new char [inBufLen];
    memset(inputBufTmp, 0, inBufLen);
    UINT inputBufTmpLoc = inBufLen - 1;
    UINT outBufCurOffet = 0;
    BYTE idx = 0;
    bool bIsExtension = false;
    int iExtension = 0;

    for (int i = 0; i < (int)inBufLen; i++) {
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
            BYTE outputByte, outputByteNext, uchTmp;
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
            } else
                outputByteNext = 0;

            outputByte |= outputByteNext;

            bIsExtension = Decode7BitCode((BYTE*)&outputByte, bIsExtension);

            if (bIsExtension)
                iExtension++;
            else {
                memcpy(outBuf + outBufCurOffet, &outputByte, 1);
                outBufCurOffet++;
                if ((int)outBufCurOffet > outBufLen) break;
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
            BYTE outputByte, outputByteNext, uchTmp;
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

            bIsExtension = Decode7BitCode((BYTE*)&outputByte, bIsExtension);

            if (bIsExtension)
                iExtension++;
            else {
                memcpy(outBuf + outBufCurOffet, &outputByte, 1);
                outBufCurOffet++;
                if ((int)outBufCurOffet > outBufLen) break;
            }

            inputBufTmpLoc--;
        }
    }
    outBufCurOffet--;
    delete [] inputBufTmp;
    return iExtension;
}


// 解7bit code
bool CCodecUtil::Decode7BitCode(BYTE* cBuf, bool bIsExtension) {
    BYTE cTmp = *cBuf;
    if (!bIsExtension) {
        switch (cTmp) {
            case 0x00: *cBuf = 0x40; break;
            case 0x01: *cBuf = 0xA3; break;
            case 0x02: *cBuf = 0x24; break;
            case 0x03: *cBuf = 0xA5; break;
            case 0x04: *cBuf = 0xE8; break;
            case 0x05: *cBuf = 0xE9; break;
            case 0x06: *cBuf = 0xF9; break;
            case 0x07: *cBuf = 0xEC; break;
            case 0x08: *cBuf = 0xF2; break;
            case 0x09: *cBuf = 0xC7; break;
            case 0x0B: *cBuf = 0xD8; break;
            case 0x0C: *cBuf = 0xF8; break;
            case 0x0E: *cBuf = 0xC5; break;
            case 0x0F: *cBuf = 0xE5; break;
            case 0x11: *cBuf = 0x5F; break;
            case 0x1B: return true;        // this is an extenstion character
            case 0x1C: *cBuf = 0xC6; break;
            case 0x1D: *cBuf = 0xE6; break;
            case 0x1E: *cBuf = 0xDF; break;
            case 0x1F: *cBuf = 0xC9; break;
            case 0x24: *cBuf = 0xA4; break;
            case 0x40: *cBuf = 0xA1; break;
            case 0x5B: *cBuf = 0xC4; break;
            case 0x5C: *cBuf = 0xD6; break;
            case 0x5D: *cBuf = 0xD1; break;
            case 0x5E: *cBuf = 0xDC; break;
            case 0x5F: *cBuf = 0xA7; break;
            case 0x60: *cBuf = 0xBF; break;
            case 0x7B: *cBuf = 0xE4; break;
            case 0x7C: *cBuf = 0xF6; break;
            case 0x7D: *cBuf = 0xF1; break;
            case 0x7E: *cBuf = 0xFC; break;
            case 0x7F: *cBuf = 0xE0; break;
        }
    } else {
        switch (cTmp) {
            case 0x14: *cBuf = 0x5E; break;
            case 0x28: *cBuf = 0x7B; break;
            case 0x29: *cBuf = 0x7D; break;
            case 0x2F: *cBuf = 0x5C; break;
            case 0x3C: *cBuf = 0x5B; break;
            case 0x3D: *cBuf = 0x7E; break;
            case 0x3E: *cBuf = 0x5D; break;
            case 0x40: *cBuf = 0x7C; break;
        }
    }
    return false;
}

// 編碼UCS2
int CCodecUtil::EncodeUCS2(char* inBuf, UINT inBufLen, char* outBuf) {
    int ucs2num;
    BYTE* msgTxt = new BYTE [(inBufLen + 1) * 2];
    wchar_t* ucs2Buf = new wchar_t [inBufLen + 1];
    wchar_t* pu16 = NULL;
    BYTE* pu8 = NULL;

    /* Convert to UCS2 string to get the character numbers, note that
       the return value of MultiByteToWideChar() include null char */
    ucs2num = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, inBuf, -1, ucs2Buf, inBufLen * 2);

    pu8 = msgTxt;
    pu16 = ucs2Buf;

    for (int i = 0; i < ucs2num; i++, pu8++, pu16++) {
        *pu8 = (BYTE)(*pu16 >> 8);
        pu8++;
        *pu8 = (BYTE)(*pu16 & 0x00FF);
    }

    memcpy(outBuf, msgTxt, ucs2num * 2);

    delete ucs2Buf;
    delete msgTxt;

    return (ucs2num - 1) * 2;
}


// 解UCS2, 主要用到WideCharToMultiByte
int CCodecUtil::DecodeUCS2(char* inBuf, UINT inBufLen, char* outBuf) {
    char* inputUCS2 = inBuf;
    char* outputTmpBuf = new char [inBufLen + 2];
    ZeroMemory(outputTmpBuf, inBufLen + 2);
    /* Note that ucs2num doesn't include the NULL terminated */
    int ucs2num = inBufLen / 2;

    char* ucs2Buf = new char [inBufLen + 2];
    ZeroMemory(ucs2Buf, inBufLen + 2);
    for (int i = 0; i < ucs2num; i++) {
        ucs2Buf[2 * i] = (inputUCS2[2 * i + 1]);
        ucs2Buf[2 * i + 1] = inputUCS2[2 * i];
    }

    ucs2Buf[inBufLen] = 0;
    ucs2Buf[inBufLen + 1] = 0;

    int n8BitLen = WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)ucs2Buf, ucs2num + 1, outputTmpBuf, 0, NULL, NULL);

    WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, (LPCWSTR)ucs2Buf, ucs2num + 1, outputTmpBuf, n8BitLen, NULL, NULL);

    memcpy(outBuf, outputTmpBuf, inBufLen);

    delete ucs2Buf;
    delete outputTmpBuf;

    return ucs2num;
}


/////////////////////////////////////////////////////////////////////
// input: CString InputString
// output: 回傳一個int值
// ussage: 傳入一個0-F的16進位的字元, 回傳相對應的數值(int),
//         判斷是不是UCS2
/////////////////////////////////////////////////////////////////////
bool CCodecUtil::IsUCS2(CStringA InputString) {
    bool IsUCS2 = false;
    int intLen = InputString.GetLength();
    for (int i = 0; i < intLen; i++) {
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
UINT CCodecUtil::Decode8Bit(char* inBuf, UINT inBufLen, char* outBuf) {
    for (int i = 0; i < (int)inBufLen; i++) {
        char inputByte = *(inBuf + i);
        memcpy(outBuf + (inBufLen - 1) - i, &inputByte, 1);
    }
    return inBufLen;
}