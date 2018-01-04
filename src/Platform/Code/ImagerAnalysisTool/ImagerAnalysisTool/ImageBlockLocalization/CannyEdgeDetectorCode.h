// CanyEdgeDetectorCode.h: interface for the CCanyEdgeDetectorCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CANYEDGEDETECTORCODE_H__19C60215_6A3E_11D3_A924_0080C88BF7B5__INCLUDED_)
#define AFX_CANYEDGEDETECTORCODE_H__19C60215_6A3E_11D3_A924_0080C88BF7B5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CCannyEdgeDetectorCode  
{
public:
	int EdgeDetection(unsigned char *ByteImage, int  column, int  row, double smoothing, double threshold,double min_threshold);

	CCannyEdgeDetectorCode();
	virtual ~CCannyEdgeDetectorCode();

private:
	int inw, inh;
	int high, low;
	double *deri_mag; /*mag of del G before non-maximum suppression*/
    unsigned char *mag_data; /* mag after non-max sppression*/
	double *orient_data,*GxImg,*GyImg; /* orient after non-max sppression*/
 
	void non_maximum_supress(int cols, int rows);
	int canny_core( double s, int cols, int rows, unsigned char * data);
	void  thresholding_tracker(int cols, int rows,unsigned char *ByteImage);
	int follow(int i,int j, int cols, int rows, unsigned char * data);
    double hypotenuse(double x, double y);
    void DoGMask(double s);
    double gaussian(double x, double s);
};

#endif // !defined(AFX_CANYEDGEDETECTORCODE_H__19C60215_6A3E_11D3_A924_0080C88BF7B5__INCLUDED_)
