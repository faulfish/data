#include "stdafx.h"
#include "ImageParser.h"
#include <math.h>
#include "CannyEdgeDetectorCode.h"
#include <gdiplus.h>
using namespace Gdiplus;

void ArrangePoints(FPOINT *points);
void LoadImage(const char *filePath, BYTE **pImage, int *width, int *height);
void ImageParser(const char *filePath, FPOINT *pPoint, const int blockNum, const int threshold);
void SmoothImage(BYTE *image, int width, int height);
int  GEtBackgroundColor(BYTE *image, int width, int height);
void BinarizeImage(BYTE *image, int width, int height, int threshold, const int tolerance);
void ConCom(int width,int height, unsigned char *image, int *&label, int &comNum);
void Contour_Tracing(int width, int current_i, int current_j, int d, int C, unsigned char *image, int *label, int &flag, int *S, int *S_next);
int mask_i_map(int d,int i);
int mask_j_map(int d,int j);
void EroseImage(BYTE *image, int width, int height);
void DilateImage(BYTE *image, int width, int height);
void GetMaxComponent(const BYTE *oriImage, const int *pixelLabel, const int width, const int height, const int comNum, FPOINT *pPoint, const int maxNum);
void TuneByEdge(const BYTE *oriImage, const int width, const int height, FPOINT *pPoint, const int blockNum);
void RotatePoints(const BYTE *image, const int width, const int height, FPOINT *points);

// 已知大小為5
void ArrangePoints(FPOINT *points)
{
	ASSERT(NULL != points);
	FPOINT temp;
	int i, j;

	for (i = 0; i < 5; i++)
		if (points[i].x < 0)
			return;

	// left-top point
	j = 0;
	for (i = 1; i < 5; i++)
		if ((points[i].x < points[j].x) && (points[i].y < points[j].y))
			j = i;

	if (j != 0)
	{
		temp = points[j];
		points[j] = points[0];
		points[0] = temp;
	}

	// right-top point
	j = 1;
	for (i = 2; i < 5; i++)
		if ((points[i].x > points[j].x) && (points[i].y < points[j].y))
			j = i;

	if (j != 1)
	{
		temp = points[j];
		points[j] = points[1];
		points[1] = temp;
	}

	// right-bottom point
	j = 4;
	for (i = 3; i < 5; i++)
		if ((points[i].x > points[j].x) && (points[i].y > points[j].y))
			j = i;

	if (j != 4)
	{
		temp = points[j];
		points[j] = points[4];
		points[4] = temp;
	}

	// left-bottom point
	if (points[2].x < points[3].x)
	{
		temp = points[3];
		points[3] = points[2];
		points[2] = temp;
	}
}

void RotatePoints(const BYTE *image, const int width, const int height, FPOINT *points)
{
	ASSERT((NULL != image) && (NULL != points));

	int n, m, x, p;
	int sumL, sumR;

	for (n = 0; n < 5; n++)
		if (points[n].x < 0)
			return;

	// check if the image is rotated about 180 degree
	sumL = sumR = 0;
	for (n = 0; n < 5; n++)
		for (m = 0; m < 5; m++)
		{
			x = (int)points[n].x-m;
			p = (int)points[n].x+m;
			if (x < 0 || x >= width || p < 0 || p >= width)
				continue;

			sumL += image[(int)(points[n].y)*width+x];
			sumR += image[(int)(points[n].y)*width+p];
		}

	if (sumL > sumR)
		return;

	FPOINT temp;
	temp = points[4];
	points[4] = points[0];
	points[0] = temp;

	temp = points[1];
	points[1] = points[3];
	points[3] = temp;
}

void ImageParser(const char *filePath, FPOINT *pPoint, const int blockNum, const int threshold)
{
	ASSERT(NULL != filePath && NULL != pPoint);

	if (0 >= blockNum)
		return;

	BYTE *oriImage = NULL, *image = NULL;
	int width, height;

	LoadImage(filePath, &oriImage, &width, &height);
	if (NULL == oriImage)
		return;

	// initialize points
	for (int i = 0; i < blockNum; i++)
		pPoint[i].x = pPoint[i].y = -1;

	const int size = width*height;
	image = new BYTE[size];
	memcpy(image, oriImage, sizeof(BYTE)*size);

	SmoothImage(image, width, height);

	int BKColor = GEtBackgroundColor(image, width, height);

	BinarizeImage(image, width, height, BKColor, threshold);

	DilateImage(image, width, height);

	EroseImage(image, width, height);

	int *pixelLabel = NULL, comNum = 0;
	ConCom(width, height, image, pixelLabel, comNum);

	GetMaxComponent(oriImage, pixelLabel, width, height, comNum, pPoint, blockNum);

	TuneByEdge(oriImage, width, height, pPoint, blockNum);

	// only execution when pPoint array size is 5
	ArrangePoints(pPoint);
	RotatePoints(oriImage, width, height, pPoint);

	if (NULL != oriImage)
		delete []oriImage;
	if (NULL != image)
		delete []image;
	if (NULL != pixelLabel)
		delete []pixelLabel;
}

void LoadImage(const char *filePath, BYTE **pImage, int *width, int *height)
{
	ASSERT(NULL != filePath);

	WCHAR wcFile[512];
	int length;
	
	length = MultiByteToWideChar(CP_ACP, 0, filePath, -1, NULL, NULL); 
	MultiByteToWideChar(CP_ACP, 0, filePath, -1, wcFile, length);

	Bitmap image(wcFile);

	const int w = *width = image.GetWidth();
	const int h = *height = image.GetHeight();
	
	*pImage = new BYTE[w*h];
	if (NULL == *pImage)
		return ;

	int i, j;
	Color pixelColor;

	for (j = 0; j < h; ++j)
		for (i = 0; i < w; ++i)
		{
			image.GetPixel(i, j, &pixelColor);
			pImage[0][j*w+i] = (pixelColor.GetR()+pixelColor.GetG()+
						pixelColor.GetB())/3;
		}

	return;
}

void SmoothImage(BYTE *image, int width, int height)
{
	ASSERT(NULL != image);

	BYTE *buf = NULL;
	buf = new BYTE[width*height];
	if (NULL == buf)
		return;

	const int msk_x = 3;
	const int msk_y = 3;
	const int msk_sz = msk_x*msk_y;
	int i, j, x, y, avg;

	memcpy(buf, image , sizeof(BYTE)*width*height);
	for (j = msk_y/2; j < height - msk_y/2; j++)
		for (i = msk_x/2; i < width - msk_y/2 ; i++)
		{
			avg = 0;
			for (y = j-msk_y/2; y <= j + msk_y/2; y++)
				for (x = i-msk_x/2; x <= i + msk_x/2; x++)
					avg += buf[y*width+x];

			image[j*width+i] = avg/msk_sz;
		}

	delete []buf;
}

int GEtBackgroundColor(BYTE *image, int width, int height)
{
	ASSERT(NULL != image);

	int i; 
	int historgram[256] = {0};
	const int size = width*height;

	for (i = 0; i < size; i++)
		historgram[image[i]]++;

	int color = 0, max = historgram[0];
	for (i = 1; i < 256; i++)
		if (historgram[i] > max)
		{
			max = historgram[i];
			color = i;
		}

	return color;
}

void BinarizeImage(BYTE *image, int width, int height, int threshold, const int tolerance)
{
	const int size = width*height;
	int i = 0;

	for (i = 0; i < size; i++)
	{
		if (abs(image[i]-threshold) < tolerance)
			image[i] = 255;
		else
			image[i] = 0;
	}
}

void ConCom(int width, int height, unsigned char *image, int *&label, int &comNum)
{
	comNum = 1;
	int i, j;
	int n = 3;
	int S[2], S_next[2], flag;

	label = new int[width*height];    
	memset((void*)label, 0, sizeof(int)*width*height);

	//去除邊界
	for( i = 0 ; i < width; ++i)
	{
		image[i] = 1;     
		image[(height-1)*width+i] = 1;
	}

	for( j = 0 ; j < height; ++j)
	{
		image[j*width] = 1;
		image[j*width+width-1] = 1;
	}

	for( j = 0; j < height; ++j)
	{
		for( i = 0; i < width; ++i )
		{
			if( image[j*width+i] == 0 )
			{
				if( label[j*width+i] == 0 && image[j*width+i-1] != 0 )	//case1
				{              
					label[j*width+i] = comNum;
					S[0] = i;
					S[1] = j;
					flag = 1;				  
					Contour_Tracing(width, i, j, 0, comNum, image, label, flag, S, S_next);
					comNum++;
				}
				else if( label[j*width+i+1] == 0 && image[j*width+i+1]!= 0 )		//case 2
				{ 
					if( label[j*width+i] == 0 && label[j*width+i-1] > 0 )
					{
						label[j*width+i] = label[j*width+i-1];  
						S[0] = i;
						S[1] = j;
						flag = 1;					
						Contour_Tracing(width, i, j, 1, label[j*width+i-1], image, label, flag, S, S_next);  
					}
					else if( label[j*width+i] != 0 )
					{
						S[0] = i;
						S[1] = j;
						flag = 1;					 
						Contour_Tracing(width, i, j, 1, label[j*width+i], image, label, flag, S, S_next);  
					}
				}
				else if( label[j*width+i] == 0 )
					label[j*width+i] = label[j*width+i-1];      //case 3
			}
			else
				label[j*width+i] = -1;					   
		}
	}

	for( i = 0; i < width*height; ++i )
		if( label[i] == -1 )
			label[i] = 0;

	--comNum;
}

void Contour_Tracing(int width, int current_i, int current_j, int d, int C, unsigned char *image, int *label, 
					 int &flag, int *S, int *S_next)
{
	int I, J;
	int temp;
	int terminate, Count = 0;

	terminate = 0;
	while( terminate < 1 )
	{
		I = mask_i_map(d, current_i);
		J = mask_j_map(d, current_j);   
		if( image[J*width+I] == 0 )
		{
			label[J*width+I] = C;
			if(flag)
			{
				S_next[0] = I;
				S_next[1] = J;
				flag = 0;
			}
			else 
			{
				if( current_i == S[0] && current_j == S[1] )
					if( S_next[0] == I && S_next[1] == J )
						terminate = 1;
			}
			temp = (d + 4)%8;
			d = (temp+2)%8;
			current_i = I;
			current_j = J;	
			Count = 0;
		}
		else
		{
			label[J*width+I] = -1;
			++d;
			d = d%8;
			Count++;
		}

		if( Count == 8 )
			break;
	}
}

int mask_i_map(int d,int i)
{
	if( d < 2  || d == 7 )
		return (i+1);
	else if (d < 6 && d > 2)
		return (i-1);
	else
		return i;
}

int mask_j_map(int d,int j)
{
	if(d < 8 && d > 4)
		return (j-1);
	else if (d < 4 && d > 0)
		return (j+1);
	else
		return j;
}

void DilateImage(BYTE *image, int width, int height)
{
	ASSERT(NULL != image);

	BYTE *buf = NULL;
	buf = new BYTE[width*height];
	if (NULL == buf)
		return;

	const int msk_x = 3;
	const int msk_y = 3;
	const int msk_sz = msk_x*msk_y;
	int i, j, x, y, min;

	memcpy(buf, image , sizeof(BYTE)*width*height);
	for (j = msk_y/2; j < height - msk_y/2; j++)
		for (i = msk_x/2; i < width - msk_y/2 ; i++)
		{
			min = 256;
			for (y = j-msk_y/2; y <= j + msk_y/2; y++)
				for (x = i-msk_x/2; x <= i + msk_x/2; x++)
					if (min > buf[y*width+x])
						min = buf[y*width+x];

			image[j*width+i] = min;
		}

	delete []buf;
}

void EroseImage(BYTE *image, int width, int height)
{
	ASSERT(NULL != image);

	BYTE *buf = NULL;
	buf = new BYTE[width*height];
	if (NULL == buf)
		return;

	const int msk_x = 3;
	const int msk_y = 3;
	const int msk_sz = msk_x*msk_y;
	int i, j, x, y, max;

	memcpy(buf, image , sizeof(BYTE)*width*height);
	for (j = msk_y/2; j < height - msk_y/2; j++)
		for (i = msk_x/2; i < width - msk_y/2 ; i++)
		{
			max = 0;
			for (y = j-msk_y/2; y <= j + msk_y/2; y++)
				for (x = i-msk_x/2; x <= i + msk_x/2; x++)
					if (max < buf[y*width+x])
						max = buf[y*width+x];

			image[j*width+i] = max;
		}

	delete []buf;
}

void GetMaxComponent(const BYTE *oriImage, const int *pixelLabel, const int width, const int height, const int comNum, FPOINT *pPoint, const int maxNum)
{
	ASSERT(NULL != pixelLabel && NULL != pPoint);

	if (0 == comNum || 0 == maxNum)
		return;

	int *pixelCount = NULL;

	pixelCount = new int[comNum];
	if (NULL == pixelCount)
		return;

	memset(pixelCount, 0, sizeof(int)*comNum);

	const int size = width*height;
	int i, j;

	// 先算出每個component的pixel數量
	for (i = 0; i < size; i++)
		if (0 != pixelLabel[i])
			pixelCount[pixelLabel[i]-1]++;
	
	// 求出最大component的center point
	int maxLabel = 0, maxCount = -1;
	for (i = 0; i < maxNum; i++)
	{
		maxLabel = 0;
		maxCount = -1;
		for (j = 0; j < comNum; j++)
			if (pixelCount[j] > maxCount)
			{
				maxCount = pixelCount[j];
				maxLabel = j;
			}

		if (maxCount == -1)
			break;	

		if (maxCount < 20)
			break;

		pPoint[i].x = 0;
		pPoint[i].y = 0;

		for (j = 0; j < size; j++)
			if (pixelLabel[j] == maxLabel+1)
			{
				pPoint[i].x += j%width;
				pPoint[i].y += j/width;
			}

		pPoint[i].x /= pixelCount[maxLabel];
		pPoint[i].y /= pixelCount[maxLabel];

		pixelCount[maxLabel] = 0;
	}

	delete []pixelCount;
}

void TuneByEdge(const BYTE *oriImage, const int width, const int height, FPOINT *pPoint, const int blockNum)
{
	ASSERT(NULL != oriImage && NULL != pPoint && width > 0 && height > 0);
	if ( 0 >= blockNum)
		return;

	const int size = width*height;
	BYTE *edgeImage = NULL;
	int i, j;

	edgeImage = new BYTE[size];
	memcpy(edgeImage, oriImage, sizeof(BYTE)*size);

	CCannyEdgeDetectorCode cannyDetector;
	
	cannyDetector.EdgeDetection(edgeImage, width, height, 3, 50, 80);

	const int msk_sz = 10;
	int n;
	double dist, minDist, minx, miny;

	for (n = 0; n < blockNum; n++)
	{
		minDist = width;
		minx = pPoint[n].x;
		miny = pPoint[n].y;
		for (j = (int)(pPoint[n].y-msk_sz); j < (int)(pPoint[n].y+msk_sz); j++)
			for (i = (int)(pPoint[n].x-msk_sz); i < (int)(pPoint[n].x+msk_sz); i++)
			{
				if (i < 0 || j < 0 || i >= width || j >= height)
					continue;

				if (edgeImage[j*width+i] == 255)
				{
					dist = sqrt((i-pPoint[n].x)*(i-pPoint[n].x)+(j-pPoint[n].y)*(j-pPoint[n].y));
					if (dist < minDist)
					{
						minDist = dist;
						minx = (double)i;
						miny = (double)j;
					}
				}
			}

		pPoint[n].x = minx;
		pPoint[n].y = miny;
	}

	delete []edgeImage;
}
