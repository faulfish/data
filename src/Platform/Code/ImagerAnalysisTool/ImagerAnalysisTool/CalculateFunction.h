#pragma once
#include "math.h"
#include "ImageBlockLocalization\ImageParser.h"
#define M_PI       3.14159265358979323846

//return value is reported by degree.
bool CalclateRotation(FPOINT* pArrayPoints, double& dwRotation)
{
	double dw_xUL = pArrayPoints[0].x;
	double dw_yUL = pArrayPoints[0].y;

	double dw_xUR = pArrayPoints[1].x;
	double dw_yUR = pArrayPoints[1].y;

	//None used.
	//double dw_xCenter = pArrayPoints[2].x;
	//double dw_yCenter = pArrayPoints[2].y;

	double dw_xLL = pArrayPoints[3].x;
	double dw_yLL = pArrayPoints[3].y;

	double dw_xLR = pArrayPoints[4].x;
	double dw_yLR = pArrayPoints[4].y;

	if((dw_xUR - dw_xUL) < 0.000001)
		return false;
	if((dw_xLR - dw_xLL) < 0.000001)
		return false;
	if((dw_yLR - dw_yUR)< 0.000001)
		return false;
	if((dw_yLL - dw_yUL)< 0.000001)
		return false;
	double dw_Rotation_Up = atan((dw_yUL - dw_yUR) / (dw_xUR - dw_xUL));
	double dw_Rotation_Low = atan((dw_yLL - dw_yLR) / (dw_xLR - dw_xLL));
	double dw_Rotation_Right = atan((dw_xLR - dw_xUR) / (dw_yLR - dw_yUR));
	double dw_Rotation_Left = atan((dw_xLL - dw_xUL) / (dw_yLL - dw_yUL));

	dwRotation = (dw_Rotation_Up + dw_Rotation_Low + dw_Rotation_Right + dw_Rotation_Left)* 180 /(4 * M_PI);
	return true;
}

//return value is reported by degree.
bool CalclateTilt(FPOINT* pArrayPoints,
				  const double dw_Ideal_X_Center ,const double dw_Ideal_Y_Center,
				  const double dw_Chart_Width, const double dw_Chart_Height,
				  const double dw_Distance,
				  double& dwTiltX, double& dwTiltY)
{
	if (dw_Chart_Width < 0.000001)
		return false;
	if (dw_Chart_Height < 0.000001)
		return false;
	if (dw_Distance < 0.000001)
		return false;

	double dw_xUL = pArrayPoints[0].x;
	double dw_yUL = pArrayPoints[0].y;

	double dw_xUR = pArrayPoints[1].x;
	double dw_yUR = pArrayPoints[1].y;

	double dw_xCenter = pArrayPoints[2].x;
	double dw_yCenter = pArrayPoints[2].y;

	double dw_xLL = pArrayPoints[3].x;
	double dw_yLL = pArrayPoints[3].y;

	double dw_xLR = pArrayPoints[4].x;
	double dw_yLR = pArrayPoints[4].y;
	
	double dw_xOffset = (dw_Ideal_X_Center - dw_xCenter);
	double dw_yOffset = (dw_Ideal_Y_Center - dw_yCenter);

	double dw_xUp = dw_xUL - dw_xUR;
	double dw_yUp = dw_yUL - dw_yUR;
	double MPlumUp = sqrt(dw_xUp*dw_xUp + dw_yUp*dw_yUp)/dw_Chart_Width;

	double dw_xLow = dw_xLL - dw_xLR;
	double dw_yLow = dw_yLL - dw_yLR;
	double MPlumLow = sqrt(dw_xLow*dw_xLow + dw_yLow*dw_yLow)/dw_Chart_Width;

	double dw_xRight = dw_xUR - dw_xLR;
	double dw_yRight = dw_yUR - dw_yLR;
	double MPlumRight = sqrt(dw_xRight*dw_xRight + dw_yRight*dw_yRight)/dw_Chart_Height;

	double dw_xLeft = dw_xUL - dw_xLL;
	double dw_yLeft = dw_yUL - dw_yLL;
	double MPlumLeft = sqrt(dw_xLeft*dw_xLeft + dw_yLeft*dw_yLeft)/dw_Chart_Height;

	double MPlumAvg = (MPlumUp + MPlumLow + MPlumRight + MPlumLeft) / 4;
	if (MPlumAvg < 0.000001)
		return false;

	dwTiltX = asin( dw_xOffset / dw_Distance / MPlumAvg) * 180 / M_PI;
	dwTiltY = asin( dw_yOffset / dw_Distance / MPlumAvg) * 180 / M_PI;
	return true;
}