///////////////////////////////////////////////////////////
//  IQListBoxUI.h
//  Implementation of the Interface IQListView
//  Created on:      08-Feb-2007 08:08:09
//  Original author: Eddy Guo
///////////////////////////////////////////////////////////

#if !defined(BENQ_11AF4308_7773_456a_B0BB_779BA6F9AEF3__INCLUDED_)
#define BENQ_11AF4308_7773_456a_B0BB_779BA6F9AEF3__INCLUDED_

#include <map>
#include "..\..\..\CommonUtil\notifybase.h"
#include "..\CommUIExportAPI.h"

namespace QSYNC_LIB
{
	using namespace std;

	class COMMONUI_API ColumnData
	{
	public:
		ColumnData():m_nOrder(-1),m_nWidth(0)  {}
		virtual ~ColumnData() {}
	public:
		int			m_nOrder;
		int			m_nWidth;
		CString		m_strKey;
		CString		m_strDisplay;
	};

	class COMMONUI_API CBasicData
	{
	public:
		enum Alignment
		{
			Alignment_Left,
			Alignment_Center,
			Alignment_Right
		};
	public:
		CBasicData(const CString& strText = _T(""),long lparam = NULL) : 
		  m_LPARAM(lparam), 
			  m_nValue(-1),
			  m_Alignment(Alignment_Center),
			  m_nclrA(255),
			  m_nclrR(0),
			  m_nclrG(0),
			  m_nclrB(0),
			  m_nBGclrR(255),
			  m_nBGclrG(255),
			  m_nBGclrB(255),
			  m_nFontSize(8),
			  m_nImageWidth(0),
			  m_bAutoSize(true),
			  m_strText(strText)
		  {}
		  virtual			~CBasicData(){}
		  void				SetTextString(const CString& strText) {m_strText = strText;}
		  const CString&	GetTextString() const {return m_strText;}
		  void				SetPARAM(long lparam) {m_LPARAM = lparam;}
		  long				GetPARAM() {return m_LPARAM;}
		  void				SetValue(int nValue) {m_nValue = nValue;}
		  int				GetPARAM() const {return m_nValue;}
		  void				SetAlignment(Alignment lparam) {m_Alignment = lparam;}
		  long				GetAlignment() {return m_Alignment;}
		  bool operator==(CBasicData cbd) const 
		  {
			  return (m_strText == cbd.m_strText && m_LPARAM == cbd.m_LPARAM);
		  }
		  //private:
		  /**
		  * 顯示在UI上的字串
		  */
		  CString			m_strText;
		  /**
		  * 顯示在UI上的圖示
		  */
		  std::wstring		m_strImagePath;//BMP, ICON, GIF, JPEG, Exif, PNG, TIFF, WMF, and EMF.
		  int				m_nImageWidth;
		  bool				m_bAutoSize;//if true, ingore m_nImageWidth
		  /**
		  * 其他資料
		  */
		  long				m_LPARAM;
		  int				m_nValue;
		  //Display information
		  Alignment			m_Alignment; 
		  int				m_nclrA;
		  int				m_nclrR;
		  int				m_nclrG;
		  int				m_nclrB;
		  std::wstring		m_strFont;
		  int				m_nFontSize;
		  int				m_nBGclrR;
		  int				m_nBGclrG;
		  int				m_nBGclrB;
	};

	class COMMONUI_API CQListData
	{

	public:
		CQListData::CQListData():m_LPARAM(NULL),m_bDelete(false)
		{
			m_pMapData = &m_MapData;
		}

		virtual CQListData::~CQListData() 
		{
		}

		CQListData& operator=(const CQListData& rhs)
		{
			for(MAPDATA::const_iterator Iter = rhs.m_MapData.begin();Iter != rhs.m_MapData.end();Iter++)
				m_MapData[Iter->first] = Iter->second;
			return *this;
		}

		bool IsMarkDelete()
		{
			return m_bDelete;
		}

		void MarkDelete()
		{
			m_bDelete = true;
		}

		// 取得資料筆數
		int GetDataCount() const
		{
			return static_cast<int>(m_MapData.size());
		}

		// 借由key(column name)來取得value(BasicData)
		virtual CBasicData* GetValueByKey(const CString& key)
		{
			CBasicData* pBasicData = NULL;
			map<CString, CBasicData>::iterator Iter = m_MapData.find(key);
			if(Iter != m_MapData.end())
				pBasicData = &(Iter->second);
			return pBasicData;
		}

		// 設定值(一個key通常為column name, 值是BasicData)
		virtual bool SetData(const CString& columnName, const CBasicData& data)	
		{
			//assert(m_bDelete == false);
			map<CString, CBasicData>::iterator Iter = m_MapData.find(columnName);
			if(Iter != m_MapData.end())
				Iter->second = data;
			else
				m_MapData.insert(pair<CString, CBasicData>(columnName,data));
			return true;
		}

		// map from string(column name) to value(CBasicData)
		typedef map<CString, CBasicData>	MAPDATA;
		MAPDATA								m_MapData;
		MAPDATA*							m_pMapData;
		long								m_LPARAM;
		bool								m_bDelete;
	};


	// Fire 一筆CListData資料, 其中包涵有CListData及一個row的位置
	// 加上被fire的字串, 可以知道是新增, 刪除或是修改一個row的資料
	// FireDataParam
	class FireListDataParameter {
	public:
		FireListDataParameter() : nFlags(0), pListData(NULL), isFocused(false) {
			point.x = 0;
			point.y = 0;
		}
		virtual ~FireListDataParameter() {
		}
	public:
		bool isFocused;
		CPoint point;
		int nRow;
		UINT nFlags;
		CQListData* pListData;
	};
	typedef FireListDataParameter fdp;

	// Fire 一筆ColumnData資料, 其中有一個column
	// 加上被fire的字串, 可以知道是新增, 刪除或是修改一個column
	// FireColumnParam
	class FireColumnDataParameter {
	public:
		FireColumnDataParameter() : nCol(0), nFlags(0), pColumnData(NULL){
			point.x = 0;
			point.y = 0;
		}
		virtual ~FireColumnDataParameter() {
		}
	public:
		CPoint point;
		int nCol;
		UINT nFlags;
		ColumnData* pColumnData;
	};
	typedef FireColumnDataParameter fcp;

	interface IQListCtrlUI
	{
		virtual int InsertDataByIndex(CQListData* data, int nRowIndex) =0;
		virtual bool InsertDataByRowColumn(CBasicData* data, int nRow, int nCol) =0;
		virtual bool UpdateRowByIndex(int nRow, CQListData* data) =0;
		virtual bool UpdateDataByRowColumn(CBasicData* data, int nRow, int nCol) =0;
		virtual bool DeleteRowByIndex(int nRow) =0;	
		virtual int GetRowCount() =0;

		virtual bool Order(int nColumn, int sortType) =0;

		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
		virtual bool Register(INotify* pINotify, const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pINotify, const TSTRING& strEvent) = 0;
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) = 0;
		virtual void Event(const TSTRING& strEvent, long nParam) =0;

		virtual bool InsertColumn(int nIndex, ColumnData* pColumnData) = 0;
		virtual bool DeleteColumnByIndex(int nIndex) = 0;
		virtual bool DeleteColumnByKey(const CString& strKey) = 0;
		virtual bool UpdateColumnByIndex(int nIndex, ColumnData* pColumnData) = 0;
		virtual bool UpdateColumnByKey(const CString& strKey, const CString& vlaue) = 0;
		virtual bool GetColumnByIndex(int nIndex, ColumnData* pColumnData) = 0;
		virtual bool GetColumnByKey(const CString& strKey, ColumnData* pColumnData) = 0;
		virtual int GetColumnCount() =0;

		virtual DWORD SetExtendedStyle(DWORD dwStyle) = 0;
		virtual long SetDropable(bool bIsDropable) =0;
		virtual bool GetSelectedItem(vector<FireListDataParameter>& vectSelected) =0;
	};

	// Fire 一筆DropData資料, 其中Drop的資料放在fileNameList
	class CDragDropFireParam
	{
	public:
		CDragDropFireParam(){}
		virtual ~CDragDropFireParam(){}	

		int GetFileCount() const
		{
			_W64 int nCount = m_FileNameList.GetCount();
			return static_cast<int>(nCount);
		}
		void Insert(const CString& strFile)
		{
			m_FileNameList.AddTail(strFile);
		}
		const CString& GetByIndex (int nIndex) const
		{
			static CString strTemp;
			if(nIndex >= 0 && nIndex < GetFileCount())
			{
				int nRealINdex = 0;
				POSITION pos = m_FileNameList.GetHeadPosition();
				while(pos != NULL)
				{
					CString strBuffer = m_FileNameList.GetNext(pos);
					if(nRealINdex == nIndex)
					{
						strTemp = strBuffer;
						break;
					}
					nRealINdex++;
				}
			}
			return strTemp;
		}
		const DROPEFFECT	GetEffect() const {return m_Effect;}
		void				SetEffect(DROPEFFECT effect) {m_Effect = effect;}
		const CPoint&		GetPoint() const {return m_Point;}
		void				SetPoint(const CPoint& point) {m_Point = point;}
	private:
		CStringList		m_FileNameList;
		DROPEFFECT		m_Effect;	
		CPoint			m_Point;
	};

}
#endif // !defined(BENQ_11AF4308_7773_456a_B0BB_779BA6F9AEF3__INCLUDED_)
