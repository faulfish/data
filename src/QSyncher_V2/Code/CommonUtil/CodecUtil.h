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
    // Byte String(ゅ计) to byte(计)
    int GSMString2Bytes(const char* pSrc, BYTE* pDst, int nSrcLength);
    // Byte to(计) byte string(ゅ计)
    int GSMBytes2String(const BYTE* pSrc, char* pDst, int nSrcLength);

    bool HEXToDigit(BYTE *pHexBuf, UINT uiHexBufLen, BYTE *pDigitBuf);
    UINT DigitToHEX(BYTE *pHexBuf, CStringA cstrDigit, char chDummyDigit);
    bool BCDToDigit(BYTE *pBcdBuf, UINT uiBcdBufLen, BYTE *pDigitBuf);
    UINT DigitToBCD(BYTE *pBcdBuf, CStringA cstrDigit, char chDummyDigit = '0');
    bool HEXBCDToDigit(BYTE *pHexBcdBuf, UINT uiHexBcdBufLen, BYTE *pDigitBuf);
    UINT DigitToHEXBCD(BYTE *pHexBcdBuf, CStringA cstrDigit, char chDummyDigit = '0');

    // encode 7bit﹃
    UINT Encode7Bit(char *inBuf, UINT inBufLen, char *outBuf);
 
    // decode 7bit﹃
    UINT Decode7Bit(char *inBuf, UINT inBufLen, char *outBuf, int outBufLen, BYTE nFillBit);
 
    // encode 7bitㄇ疭絏
    bool Encode7BitCode(BYTE *cBuf, BYTE *ExtByte);
 
    // decode 7bitㄇ疭絏
    bool Decode7BitCode(BYTE *cBuf, bool bIsExtension);
 
    // encode UCS2 (UnitCode)
    int EncodeUCS2(char *inBuf, UINT inBufLen, char *outBuf);
 
    // decode UCS2 (UnitCode)
    int DecodeUCS2(char *inBuf, UINT inBufLen, char *outBuf);
 
    // 耞琌ぃ琌UCS2絪絏
    bool IsUCS2(CStringA InputString);
 
    // decode 8bit﹃
    UINT Decode8Bit(char *inBuf, UINT inBufLen, char *outBuf);
 
    // 眖16秈ゅ计锣Θ10秈计
    inline int hex2dec(char c) {
        return ('A' <= (int)c && (int)c <= 'F') ? (c - 'A' + 10) : (c - '0');
    }
};


#endif