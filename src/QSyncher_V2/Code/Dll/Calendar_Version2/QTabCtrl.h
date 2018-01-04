#pragma once


// CQTabCtrl

class CQTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CQTabCtrl)

public:
	CQTabCtrl();
	virtual ~CQTabCtrl();
	virtual void DrawItem(LPDRAWITEMSTRUCT);

protected:
	DECLARE_MESSAGE_MAP()
};


