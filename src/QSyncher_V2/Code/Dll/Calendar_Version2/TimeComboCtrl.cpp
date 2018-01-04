// TimeComboCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TimeComboCtrl.h"


// CTimeComboCtrl

IMPLEMENT_DYNAMIC(CTimeComboCtrl, CComboBox)

CTimeComboCtrl::CTimeComboCtrl()
{

}

CTimeComboCtrl::~CTimeComboCtrl()
{
}


BOOL CTimeComboCtrl::GetTimeText(int& nHour, int& nMinute)
{
	// get the current edit text.
	CString strText;
	GetWindowText(strText);

	strText.TrimLeft();
	strText.TrimRight();

	CString strHour, strMinute;
	BOOL bContinue = !strText.IsEmpty();

	if(bContinue)
	{
		// retrieve hour & minute text from time text.
		int nFound = strText.Find(_T(':'));
		if(nFound == -1)
		{
			if(strText.GetLength() <= 2)
			{
				strHour = strText;
				strMinute = _T("");
			}
			else
			{
				strHour = strText.Left(strText.GetLength()-2);
				strMinute = strText.Right(2);
			}
		}
		else
		{
			strHour = strText.Left(nFound);
			strMinute = strText.Mid(nFound+1);
		}
	}

	if(bContinue)
	{
		int i;
		for(i = 0; bContinue && (i < strHour.GetLength()); i++)
			bContinue = _istdigit(strHour[i]);

		for(i = 0; bContinue && (i < strMinute.GetLength()); i++)
			bContinue = _istdigit(strMinute[i]);
	}
	if(bContinue)
	{
		nHour = _ttoi(strHour);
		nMinute = _ttoi(strMinute);
	}

	return bContinue;
}


BEGIN_MESSAGE_MAP(CTimeComboCtrl, CComboBox)
END_MESSAGE_MAP()




// CTimeComboCtrl message handlers


