#pragma once

typedef struct Point
{
	double x, y;
}FPOINT;

/*threshold about 20 - 25*/
void ImageParser(const char *filePath, FPOINT *pPoint, const int blockNum, const int threshold = 25);
void SwapArray(FPOINT* pArrayPoints,int nIndex1, int nIndex2);
void ArrangePointAgain(FPOINT* pArrayPoints);
bool ContrastSave(const char * szOrgFilePath,const char *szPath,float nContrast);
