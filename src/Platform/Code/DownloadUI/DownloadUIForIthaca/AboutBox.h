#pragma once


// CAboutBox dialog

class CAboutBox : public CDialog
{
	DECLARE_DYNAMIC(CAboutBox)

public:
	CAboutBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutBox();

// Dialog Data
	enum { IDD = IDD_DOWNLOAD_UI_ABOUT_BOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
