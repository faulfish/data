#ifndef _Cursor_Utility_H_
#define _Cursor_Utility_H_

namespace QSYNC_LIB
{
	class CCursorWait
	{
	public:
		CCursorWait(LPCTSTR lpCursorName = IDC_WAIT)
		{
			HCURSOR hcurWait = ::LoadCursor(NULL,lpCursorName);
			m_hcurOld = ::SetCursor(hcurWait);
		}
		~CCursorWait()
		{
			::SetCursor(m_hcurOld);
		}
	private:
		HCURSOR m_hcurOld;
	};
}
#endif //_Cursor_Utility_H_