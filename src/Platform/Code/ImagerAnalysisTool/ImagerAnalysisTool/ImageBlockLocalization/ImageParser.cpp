#include "stdafx.h"
#include "ImageParser.h"
#include <math.h>
#include "CannyEdgeDetectorCode.h"
#include <assert.h>
#include <gdiplus.h>
#include "QColorMatrix.h"
#include "QImageEncoder.h"
#include "../../../CommonUtil\unicodeutility.h"

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

BYTE *TestBuffer;
BYTE *GetTestBuffer()
{
	return TestBuffer;
}

void SwapArray(FPOINT* pArrayPoints,int nIndex1, int nIndex2)
{
	assert(pArrayPoints);

	FPOINT tempPoint = pArrayPoints[nIndex1];
	pArrayPoints[nIndex1] = pArrayPoints[nIndex2];
	pArrayPoints[nIndex2] = tempPoint;
}

// 已知大小為5
void ArrangePointAgain(FPOINT* pArrayPoints)
{
	assert(pArrayPoints);

	for(int j = 0 ; j < 5 ; ++j)
	{
		for (int i = j+1 ; i < 5 ; ++i)
		{
			if (pArrayPoints[j].y > pArrayPoints[i].y)
				SwapArray(pArrayPoints, i, j);
		}
	}

	if (pArrayPoints[0].x > pArrayPoints[1].x)
		SwapArray(pArrayPoints, 0, 1);
	if (pArrayPoints[3].x > pArrayPoints[4].x)
		SwapArray(pArrayPoints, 3, 4);
}

void RotatePoints(const BYTE *image, const int width, const int height, FPOINT *points)
{
	ASSERT((NULL != image) && (NULL != points));

	int m, x, p;
	int sumL, sumR;

	for (x = 0; x < 5; x++)
		if (points[x].x < 0)
			return;

	// check if the image is rotated about 180 degree
	sumL = sumR = 0;
	for (m = 1; m < 10; m++)
	{
		x = (int)points[2].x-m;
		p = (int)points[2].x+m;
		if (x < 0 || x >= width || p < 0 || p >= width)
			continue;

		sumL += image[(int)(points[2].y)*width+x];
		sumR += image[(int)(points[2].y)*width+p];
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

//	TestBuffer = new BYTE[size];

	SmoothImage(image, width, height);

	int BKColor = GEtBackgroundColor(image, width, height);

	BinarizeImage(image, width, height, BKColor, threshold);

//	memcpy(TestBuffer, image, size);

	DilateImage(image, width, height);

	EroseImage(image, width, height);

	int *pixelLabel = NULL, comNum = 0;
	ConCom(width, height, image, pixelLabel, comNum);

	GetMaxComponent(oriImage, pixelLabel, width, height, comNum, pPoint, blockNum);

	TuneByEdge(oriImage, width, height, pPoint, blockNum);

	// only execution when pPoint array size is 5
	ArrangePointAgain(pPoint);
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

	RECT *rect = NULL;

	rect = new RECT[comNum];
	if (NULL == rect)
	{
		delete []pixelCount;
		return;
	}

	const int size = width*height;
	int i, j, k, rw, rh;

	// 先算出每個component的pixel數量
	for (i = 0; i < size; i++)
		if (0 != pixelLabel[i])
			pixelCount[pixelLabel[i]-1]++;

	// Initialize Rect
	for (i = 0; i < comNum; i++)
	{
		rect[i].left = width;
		rect[i].top = height;
		rect[i].right = 0;
		rect[i].bottom = 0;
	}

	for (j = 0; j < height; j++)
		for (i = 0; i < width; i++)
		{
			k = pixelLabel[j*width+i];
			if (0 != k)
			{
				if (i < rect[k-1].left)
					rect[k-1].left = i;
				if (j < rect[k-1].top)
					rect[k-1].top = j;
				if (i > rect[k-1].right)
					rect[k-1].right = i;
				if (j > rect[k-1].bottom)
					rect[k-1].bottom = j;
			}
		}
	
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

		// 若長寬比差太多, ignore it
		rw = rect[maxLabel].right - rect[maxLabel].left;
		rh = rect[maxLabel].bottom - rect[maxLabel].top;
		if ( (float)rw/rh > 2 || (float)rh/rw > 2)
		{
			pixelCount[maxLabel] = 0;
			i--;
			continue;
		}

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
	delete []rect;
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

void ToColorSpace(Bitmap * pBitmap, const ColorMatrix& mat)
{
	Rect rc(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight());

	ImageAttributes attr;
	attr.SetColorMatrix(& mat);

	// Although undocumented, it seems perfectly legal to draw a bitmap on itself.
	Graphics g(pBitmap);
	g.DrawImage(pBitmap,
		rc,
		0, 0, rc.Width, rc.Height,
		UnitPixel,
		& attr);
}
bool ContrastSave(const char * szOrgFilePath,const char * szTargetPath,float nContrast)
{
	using namespace QSYNC_LIB;
	// We want to save the file with it's original dimension, so we reload
	Bitmap * pPicture = new Bitmap(ansitowide(szOrgFilePath).c_str());

	if (! pPicture || (pPicture && pPicture->GetLastStatus() != Ok))
	{
		delete pPicture;
		return false;
	}

	const ColorMatrix MatRGBtoYUV =
	{
		0.5f, 0.3086f, -0.1681f, 0, 0,
		-0.4407f, 0.6094f, -0.3391f, 0, 0,
		-0.0593f, 0.082f, 0.5f, 0, 0,
		0, 0, 0, 1, 0,
		0.5f, 0, 0.5f, 0, 1
	};
	// Convert tot YUV color space
	ToColorSpace(pPicture,MatRGBtoYUV);

	UINT w = pPicture->GetWidth();
	UINT h = pPicture->GetHeight();

	// Create a temporary bitmap with the same size and the default pixelformat
	// (Don't take pPicture's pixel format. I tried it, but it is definitely
	// wrong: pictures loaded as gif won't be saved correctly.)
	Bitmap bmSave(w, h, PixelFormat24bppRGB);
	if (bmSave.GetLastStatus() != Ok)
	{
		delete pPicture;
		return false;
	}

	// Base a Graphics object on it
	Graphics g(& bmSave);

	bmSave.SetResolution(pPicture->GetHorizontalResolution(),
		pPicture->GetVerticalResolution());

	// Copy properties. In a real application this probably needs refining,
	// because not all properties are valid in the copy. For now, we just skip
	// the thumbnail properties.
	UINT nProps = pPicture->GetPropertyCount();
	if (nProps)
	{
		PROPID * pPropList = new PROPID[nProps];
		pPicture->GetPropertyIdList(nProps, pPropList);
		for (UINT i = 0; i < nProps; i++)
		{
			// skip thumbnail properties
			if (pPropList[i] >= PropertyTagThumbnailFormat &&
				pPropList[i] <= PropertyTagChrominanceTable) continue;

			UINT szProp = pPicture->GetPropertyItemSize(pPropList[i]);
			BYTE * pBytes = new BYTE[szProp];
			PropertyItem * pProp = (PropertyItem *) pBytes;

			pPicture->GetPropertyItem(pPropList[i], szProp, pProp);
			bmSave.SetPropertyItem(pProp);

			delete[] pBytes;
		}
		delete[] pPropList;
	}

	// Set one property, just as an example
	LPSTR app = "Tinter 1.1";	// plain chars, no TCHARs
	PropertyItem prop;
	prop.id = PropertyTagSoftwareUsed;
	prop.type = PropertyTagTypeASCII;
	prop.length = strlen(app) + 1;	// including 0-byte
	prop.value = app;
	bmSave.SetPropertyItem(& prop);

	//pPicture = GetFiltered(pPicture);

	QColorMatrix m_ColorMatrix;

	// Image is kept in YUV color space; compensate
	m_ColorMatrix = MatRGBtoYUV;

	// Update color matrix with current values
	m_ColorMatrix.Scale(nContrast, nContrast, nContrast, 1.0f, MatrixOrderPrepend);
	//m_ColorMatrix.TranslateColors(pSliderBar->GetBrightness(), MatrixOrderAppend);
	//m_ColorMatrix.SetSaturation(pSliderBar->GetSaturation(), MatrixOrderAppend);
	//m_ColorMatrix.RotateHue(pSliderBar->GetHue());
	//m_ColorMatrix.SetTint(pSliderBar->GetTintHue(), pSliderBar->GetTintAmount());

	ImageAttributes attr;
	attr.SetColorMatrix(& m_ColorMatrix);
	//attr.SetGamma(m_Gamma);

	// Draw the picture, using the image attributes
	Rect rc(0, 0, w, h);
	g.DrawImage(pPicture, rc,
		0, 0,
		w, h,
		UnitPixel,
		& attr);

	// pPicture not needed anymore
	delete pPicture;

	QImageEncoder encoder(ansitowide(szTargetPath).c_str());
	encoder.m_Quality = 90;

	// Finally, save it!
	Status stat = encoder.Save(& bmSave, ansitowide(szTargetPath).c_str());

	bool r = (stat == Ok);
	return r;
}