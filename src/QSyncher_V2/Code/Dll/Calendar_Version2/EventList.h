#pragma once


/*! @class		CEventList
 * @brief		Represents the output given to the UI
 * @author		Evelyn
 * @date		2007/01/31
 */

class CEventList
{
public:
	CEventList();
	virtual ~CEventList();

	int				m_DateYear;
	int				m_DateMonth;
	int				m_DateDay;
	GUID        	m_EventIndex;

protected:

private:
	

};