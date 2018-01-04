#ifndef __UTIL_CODECUTIL_H__
#define __UTIL_CODECUTIL_H__

#pragma once

inline int CHAR2BCD(char x, char y) {
    return (((x - '0')) + ((y - '0') << 4));
}
inline int CHAR2HEXBCD(char x, char y) {
    return (((x >= 'A')?(x - 'A' + 10):(x - '0'))+(((y >= 'A')?(y - 'A' + 10):(y - '0')) << 4));
}
inline int HEXCHAR2BYTE(char x,char y) {
    return ((((x >= 'A')?(x - 'A' + 10):(x - '0')) << 4 )+((y >= 'A')?(y - 'A' + 10):(y - '0')));
}


class CCodecUtil
{
public:
    CCodecUtil(void);
    ~CCodecUtil(void);

public:
    // Byte String(��Ʀr) to byte(�Ʀr)
    int GSMString2Bytes(const char* pSrc, BYTE* pDst, int nSrcLength);
    // Byte to(�Ʀr) byte string(��Ʀr)
    int GSMBytes2String(const BYTE* pSrc, char* pDst, int nSrcLength);

    bool HEXToDigit(BYTE *pHexBuf, UINT uiHexBufLen, BYTE *pDigitBuf);
    UINT DigitToHEX(BYTE *pHexBuf, CStringA cstrDigit, char chDummyDigit);
    bool BCDToDigit(BYTE *pBcdBuf, UINT uiBcdBufLen, BYTE *pDigitBuf);
    UINT DigitToBCD(BYTE *pBcdBuf, CStringA cstrDigit, char chDummyDigit = '0');
    bool HEXBCDToDigit(BYTE *pHexBcdBuf, UINT uiHexBcdBufLen, BYTE *pDigitBuf);
    UINT DigitToHEXBCD(BYTE *pHexBcdBuf, CStringA cstrDigit, char chDummyDigit = '0');

    // encode 7bit�r��
    UINT Encode7Bit(char *inBuf, UINT inBufLen, char *outBuf);
 
    // decode 7bit�r��
    UINT Decode7Bit(char *inBuf, UINT inBufLen, char *outBuf, int outBufLen, BYTE nFillBit);
 
    // encode 7bit�@�ǯS��X
    bool Encode7BitCode(BYTE *cBuf, BYTE *ExtByte);
 
    // decode 7bit�@�ǯS��X
    bool Decode7BitCode(BYTE *cBuf, bool bIsExtension);
 
    // encode UCS2 (UnitCode)
    int EncodeUCS2(char *inBuf, UINT inBufLen, char *outBuf);
 
    // decode UCS2 (UnitCode)
    int DecodeUCS2(char *inBuf, UINT inBufLen, char *outBuf);
 
    // �P�_�O���OUCS2�s�X
    bool IsUCS2(CStringA InputString);
 
    // decode 8bit�r��
    UINT Decode8Bit(char *inBuf, UINT inBufLen, char *outBuf);
 
    // �q16�i���Ʀr�ন10�i��Ʀr
    inline int hex2dec(char c) {
        return ('A' <= (int)c && (int)c <= 'F') ? (c - 'A' + 10) : (c - '0');
    }
};


#endif