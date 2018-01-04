#pragma once

typedef struct Point
{
	double x, y;
}FPOINT;

/*threshold about 20 - 25*/
void ImageParser(const char *filePath, FPOINT *pPoint, const int blockNum, const int threshold = 25);