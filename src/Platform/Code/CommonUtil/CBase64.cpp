/************************************************
 *                                              *
 * CBase64.cpp                                  *
 * Base 64 de- and encoding class               *
 *                                              *
 * ============================================ *
 *                                              *
 * This class was written on 28.05.2003         *
 * by Jan Raddatz [jan-raddatz@web.de]          *
 *                                              *
 * ============================================ *
 *                                              *
 * Copyright (c) by Jan Raddatz                 *
 * This class was published @ codeguru.com      *
 * 28.05.2003                                   *
 *                                              *
 ************************************************/

#include "CBase64.h"
#include <assert.h>


CBase64::CBase64 ()
{
}

unsigned int CBase64::CalculateRecquiredEncodeOutputBufferSize (unsigned int p_InputByteCount)
{
    div_t result = div (p_InputByteCount, 3);

    unsigned int RecquiredBytes = 0;
    if (result.rem == 0)
    {
        // Number of encoded characters
        RecquiredBytes = result.quot * 4;

        // CRLF -> "\r\n" each 76 characters
        result = div (RecquiredBytes, 76);
        RecquiredBytes += result.quot * 2;

        // Terminating null for the Encoded String
        RecquiredBytes += 1;

        return RecquiredBytes;
    }
    else
    {
        // Number of encoded characters
        RecquiredBytes = result.quot * 4 + 4;

        // CRLF -> "\r\n" each 76 characters
        result = div (RecquiredBytes, 76);
        RecquiredBytes += result.quot * 2;

        // Terminating null for the Encoded String
        RecquiredBytes += 1;

        return RecquiredBytes;
    }
}

void CBase64::EncodeByteTriple  (char* p_pInputBuffer, unsigned int InputCharacters, char* p_pOutputBuffer)
{
    unsigned int mask = 0xfc000000;
    unsigned int buffer = 0;


    char* temp = (char*) &buffer;
    temp [3] = p_pInputBuffer [0];
    if (InputCharacters > 1)
        temp [2] = p_pInputBuffer [1];
    if (InputCharacters > 2)
        temp [1] = p_pInputBuffer [2];

    switch (InputCharacters)
    {
        case 3: {
            p_pOutputBuffer [0] = BASE64_ALPHABET [(buffer & mask) >> 26];
            buffer = buffer << 6;
            p_pOutputBuffer [1] = BASE64_ALPHABET [(buffer & mask) >> 26];
            buffer = buffer << 6;
            p_pOutputBuffer [2] = BASE64_ALPHABET [(buffer & mask) >> 26];
            buffer = buffer << 6;
            p_pOutputBuffer [3] = BASE64_ALPHABET [(buffer & mask) >> 26];
            break;
        }
        case 2: {
            p_pOutputBuffer [0] = BASE64_ALPHABET [(buffer & mask) >> 26];
            buffer = buffer << 6;
            p_pOutputBuffer [1] = BASE64_ALPHABET [(buffer & mask) >> 26];
            buffer = buffer << 6;
            p_pOutputBuffer [2] = BASE64_ALPHABET [(buffer & mask) >> 26];
            p_pOutputBuffer [3] = '=';
            break;
        }
        case 1: {
            p_pOutputBuffer [0] = BASE64_ALPHABET [(buffer & mask) >> 26];
            buffer = buffer << 6;
            p_pOutputBuffer [1] = BASE64_ALPHABET [(buffer & mask) >> 26];
            p_pOutputBuffer [2] = '=';
            p_pOutputBuffer [3] = '=';
            break;
        }
    }
}

unsigned int CBase64::DecodeByteQuartet (char* p_pInputBuffer, char* p_pOutputBuffer)
{
    unsigned int buffer = 0;

    if (p_pInputBuffer[3] == '=')
    {
        if (p_pInputBuffer[2] == '=')
        {
            buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[0]]) << 6;
            buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[1]]) << 6;
            buffer = buffer << 14;

            char* temp = (char*) &buffer;
            p_pOutputBuffer [0] = temp [3];
            
            return 1;
        }
        else
        {
            buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[0]]) << 6;
            buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[1]]) << 6;
            buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[2]]) << 6;
            buffer = buffer << 8;

            char* temp = (char*) &buffer;
            p_pOutputBuffer [0] = temp [3];
            p_pOutputBuffer [1] = temp [2];
            
            return 2;
        }
    }
    else
    {
        buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[0]]) << 6;
        buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[1]]) << 6;
        buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[2]]) << 6;
        buffer = (buffer | BASE64_DEALPHABET [p_pInputBuffer[3]]) << 6; 
        buffer = buffer << 2;

        char* temp = (char*) &buffer;
        p_pOutputBuffer [0] = temp [3];
        p_pOutputBuffer [1] = temp [2];
        p_pOutputBuffer [2] = temp [1];

        return 3;
    }

    return -1;
}

void CBase64::EncodeBuffer (char* p_pInputBuffer, unsigned int p_InputBufferLength, char* p_pOutputBufferString)
{
    unsigned int FinishedByteQuartetsPerLine = 0;
    unsigned int InputBufferIndex  = 0;
    unsigned int OutputBufferIndex = 0;

    memset (p_pOutputBufferString, 0, CalculateRecquiredEncodeOutputBufferSize (p_InputBufferLength));

    while (InputBufferIndex < p_InputBufferLength)
    {
        if (p_InputBufferLength - InputBufferIndex <= 2)
        {
            FinishedByteQuartetsPerLine ++;
            EncodeByteTriple (p_pInputBuffer + InputBufferIndex, p_InputBufferLength - InputBufferIndex, p_pOutputBufferString + OutputBufferIndex);
            break;
        }
        else
        {
            FinishedByteQuartetsPerLine++;
            EncodeByteTriple (p_pInputBuffer + InputBufferIndex, 3, p_pOutputBufferString + OutputBufferIndex);
            InputBufferIndex  += 3;
            OutputBufferIndex += 4;
        }

        if (FinishedByteQuartetsPerLine == 19)
        {
            p_pOutputBufferString [OutputBufferIndex  ] = '\r';
            p_pOutputBufferString [OutputBufferIndex+1] = '\n';
            p_pOutputBufferString += 2;
            FinishedByteQuartetsPerLine = 0;
        }
    }
}

unsigned int CBase64::CreateMatchingEncodingBuffer (unsigned int p_InputByteCount, char** p_ppEncodingBuffer)
{
    unsigned int Size = CalculateRecquiredEncodeOutputBufferSize (p_InputByteCount);
    (*p_ppEncodingBuffer) = (char*) malloc (Size);
    memset (*p_ppEncodingBuffer, 0, Size);
    return Size;
}

unsigned int CBase64::CreateMatchingDecodingBuffer (char* p_pInputBufferString, char** p_ppDecodingBuffer)
{
    unsigned int Size = CalculateRecquiredDecodeOutputBufferSize (p_pInputBufferString);
    (*p_ppDecodingBuffer) = (char*) malloc (Size);
    memset (*p_ppDecodingBuffer, 0, Size);
    return Size;
}

unsigned int CBase64::EncodeFile (char* p_pSourceFileName, char* p_pEncodedFileName)
{
    CFile InputFile;
    CFile OutputFile;

    if (!InputFile.Open (p_pSourceFileName, CFile::modeRead))
        return UNABLE_TO_OPEN_INPUT_FILE;

    if (!OutputFile.Open (p_pEncodedFileName, CFile::modeCreate|CFile::modeWrite))
        return UNABLE_TO_OPEN_OUTPUT_FILE;

    char InputBuffer [19 * 3];
    char* pOutputBuffer;
    CreateMatchingEncodingBuffer (sizeof (InputBuffer), &pOutputBuffer);

    if (pOutputBuffer == 0)
        return UNABLE_TO_CREATE_OUTPUTBUFFER;

    unsigned int ReadBytes = 0;
    while ((ReadBytes = InputFile.Read (InputBuffer, sizeof (InputBuffer))) != 0)
    {
        EncodeBuffer (InputBuffer, ReadBytes, pOutputBuffer);
        OutputFile.Write (pOutputBuffer, strlen (pOutputBuffer));
    }

    OutputFile.Flush ();
    OutputFile.Close ();
    InputFile.Close  ();

    return 0;
}

unsigned int CBase64::DecodeFile (char* p_pSourceFileName, char* p_pDecodedFileName)
{
    CStdioFile  InputFile;
    CFile       OutputFile;

    if (!InputFile.Open (p_pSourceFileName, CFile::modeRead))
        return UNABLE_TO_OPEN_INPUT_FILE;

    if (!OutputFile.Open (p_pDecodedFileName, CFile::modeCreate|CFile::modeWrite))
        return UNABLE_TO_OPEN_OUTPUT_FILE;

    CString InputBuffer;
    char    OutputBuffer[64];

    unsigned int ReadBytes = 0;
    while ((ReadBytes = InputFile.ReadString (InputBuffer)) != 0)
    {
        InputBuffer.Remove ('\r');
        InputBuffer.Remove ('\n');
        unsigned int DecodedBytes = DecodeBuffer ((LPTSTR) (LPCTSTR) InputBuffer, OutputBuffer);
        OutputFile.Write (&OutputBuffer [0], DecodedBytes);
    }


    OutputFile.Flush ();
    OutputFile.Close ();
    InputFile.Close  ();

    return 0;
}

unsigned long CBase64::CalculateRecquiredDecodeOutputBufferSize (const char* p_pInputBufferString)
{
    unsigned long BufferLength = strlen (p_pInputBufferString);
    
    div_t input = div (BufferLength, 4);

    const char* pLastQuarter = p_pInputBufferString + input.quot * 4;

    char* pNewLastQuarter = NULL;
    int numQuarters = numQuarters = input.quot;

    if (input.rem != 0) {
        pNewLastQuarter = new char[4];
        for (int i = 0; i < input.rem; i++) {
            pNewLastQuarter[i] = pLastQuarter[i];
        }
        for (; i < 4; i++) {
            pNewLastQuarter[i] = '=';
        }
        pLastQuarter = pNewLastQuarter;
        numQuarters = input.quot + 1;
    } 

    unsigned long res;
    if (*(pLastQuarter + 3) != '=')
    {
        res = numQuarters * 3;
    }
    else
    {
        if (*(pLastQuarter + 2) == '=')
        {
            res = numQuarters * 3 - 2;
        }
        else
        {
            res = numQuarters * 3 - 1;
        }
    }

    delete[] pNewLastQuarter;

    return res;
}

unsigned long CBase64::DecodeBuffer (const char* p_pInputBufferString, char* p_pOutputBuffer)
{
    unsigned long InputBufferIndex  = 0;
    unsigned long OutputBufferIndex = 0;
    unsigned long InputBufferLength = strlen(p_pInputBufferString);

    char ByteQuartet [4];
    int i = 0;
    while (InputBufferIndex < InputBufferLength)
    {
        // Ignore all characters except the ones in BASE64_ALPHABET
        if ((p_pInputBufferString [InputBufferIndex] >= 48 && p_pInputBufferString [InputBufferIndex] <=  57) ||
            (p_pInputBufferString [InputBufferIndex] >= 65 && p_pInputBufferString [InputBufferIndex] <=  90) ||
            (p_pInputBufferString [InputBufferIndex] >= 97 && p_pInputBufferString [InputBufferIndex] <= 122) ||
            p_pInputBufferString [InputBufferIndex] == '+' || 
            p_pInputBufferString [InputBufferIndex] == '/' || 
            p_pInputBufferString [InputBufferIndex] == '=')
        {
            ByteQuartet [i] = p_pInputBufferString [InputBufferIndex];
            i++;
        }
        
        InputBufferIndex++;
        
        if (i == 4) {
            OutputBufferIndex += DecodeByteQuartet(ByteQuartet, p_pOutputBuffer + OutputBufferIndex);
            i = 0;
        }
    }
    // if i != 0 mean that there was some problem with input data
    assert(i == 0);

    // OutputBufferIndex gives us the next position of the next decoded character
    // inside our output buffer and thus represents the number of decoded characters
    // in our buffer.
    return OutputBufferIndex;
}
