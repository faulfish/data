#pragma once


// CTimeComboCtrl

class CTimeComboCtrl : public CComboBox
{
	DECLARE_DYNAMIC(CTimeComboCtrl)

public:
	CTimeComboCtrl();
	virtual ~CTimeComboCtrl();

	BOOL GetTimeText(int& nHour, int& nMinute);

protected:
	DECLARE_MESSAGE_MAP()

	

};


