#pragma once


// CRFCalibrationToolWnd

class CRFCalibrationToolWnd : public CFrameWnd
{
	DECLARE_DYNAMIC(CRFCalibrationToolWnd)
public:
	void CreateTimer();
	void StopTimer();

private:
	UINT m_nTimer;	

public:
	CRFCalibrationToolWnd();
	virtual ~CRFCalibrationToolWnd();

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
};


