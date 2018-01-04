///////////////////////////////////////////////////////////
//  IQListBoxUI.h
//  Implementation of the Interface IQListView
//  Created on:      08-Feb-2007 08:08:09
//  Original author: Eddy Guo
///////////////////////////////////////////////////////////

#if !defined(BENQ_11AF4308_7773_456a_B0BB_779BA6F9AEF3__INCLUDED_)
#define BENQ_11AF4308_7773_456a_B0BB_779BA6F9AEF3__INCLUDED_

#include <vector>
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
		TSTRING		m_strKey;
		TSTRING		m_strDisplay;
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
		CBasicData(const TSTRING& strText = _T(""),long lparam = NULL) : 
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
		  void				SetTextString(const TSTRING& strText) {m_strText = strText;}
		  const TSTRING&	GetTextString() const {return m_strText;}
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
		  TSTRING			m_strText;
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
		CQListData::CQListData()
			:m_LPARAM(NULL)
			,m_bDelete(false)
			,m_nState(0)
		{
		}

		virtual CQListData::~CQListData() 
		{
		}

		virtual CQListData& operator=(const CQListData& rhs)
		{
			m_LPARAM = rhs.m_LPARAM;
			m_bDelete = rhs.m_bDelete;

			for(MAPDATA::const_iterator Iter = rhs.m_MapData.begin();Iter != rhs.m_MapData.end();Iter++)
				SetData(Iter->first,Iter->second);

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
		virtual CBasicData* GetValueByKey(const TSTRING& key)
		{
			CBasicData* pBasicData = NULL;
			map<TSTRING, CBasicData>::iterator Iter = m_MapData.find(key);
			if(Iter != m_MapData.end())
				pBasicData = &(Iter->second);
			return pBasicData;
		}

		// 設定值(一個key通常為column name, 值是BasicData)
		virtual bool SetData(const TSTRING& columnName, const CBasicData& data)	
		{
			//assert(m_bDelete == false);
			map<TSTRING, CBasicData>::iterator Iter = m_MapData.find(columnName);
			if(Iter != m_MapData.end())
				Iter->second = data;
			else
				m_MapData.insert(pair<TSTRING, CBasicData>(columnName,data));
			return true;
		}

		bool Find(const TSTRING& strKey) const
		{
			return m_MapData.find(strKey) != m_MapData.end();
		}

		CBasicData* GetValue(const TSTRING& strKey)
		{
			CBasicData* pValue = NULL;
			if(Find(strKey))
				pValue = &(m_MapData[strKey]);
			return pValue;
		}

		long GetLPARAM() const {return m_LPARAM;}
		void SetLPARAM(long LPARAM){m_LPARAM = LPARAM;}
		UINT GetState() const {return m_nState;}
		void SetState(UINT nState){m_nState = nState;}
	private:
		// map from string(column name) to value(CBasicData)
		typedef map<TSTRING, CBasicData>	MAPDATA;
		MAPDATA					m_MapData;
		long								m_LPARAM;
		bool								m_bDelete;
		UINT							m_nState; 
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

	class CEditParam
	{
	public:
		CEditParam():m_bEditable(false),m_bFinishEdit(false),m_nItemIndex(-1){}
		void SetEditable(bool bEditable){m_bEditable = bEditable;}
		bool GetEditable() const {return m_bEditable;}		
		void SetEditString(const TSTRING& strEditString){m_strEditString = strEditString;}		
		const TSTRING& GetEditString() const{return m_strEditString;}
		void SetFinishEdit(bool bFinishEdit){m_bFinishEdit = bFinishEdit;}
		bool GetFinishEdit() const {return m_bFinishEdit;}
		void SetItemIndex(int nItemIndex){m_nItemIndex = nItemIndex;}
		int GetItemIndex() const {return m_nItemIndex;}
	private:
		int		m_nItemIndex;
		bool	m_bEditable;
		bool	m_bFinishEdit;
		TSTRING m_strEditString;
	};

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
		virtual bool DeleteColumnByKey(const TSTRING& strKey) = 0;
		virtual bool UpdateColumnByIndex(int nIndex, ColumnData* pColumnData) = 0;
		virtual bool UpdateColumnByKey(const TSTRING& strKey, const TSTRING& vlaue) = 0;
		virtual bool GetColumnByIndex(int nIndex, ColumnData* pColumnData) = 0;
		virtual bool GetColumnByKey(const TSTRING& strKey, ColumnData* pColumnData) = 0;
		virtual int GetColumnCount() =0;

		virtual DWORD SetExtendedStyle(DWORD dwStyle) = 0;
		virtual long SetDropable(bool bIsDropable) =0;
		virtual bool GetSelectedItem(vector<FireListDataParameter>& vectSelected) =0;
		virtual void RefreshClear() = 0;
	};

	// Fire 一筆DropData資料, 其中Drop的資料放在fileNameList
	class CDragDropFireParam
	{
	public:
		CDragDropFireParam():m_pAppData(NULL),m_Effect(DROPEFFECT_NONE),m_hTargetWnd(NULL){}
		virtual ~CDragDropFireParam(){}	

		int GetFileCount() const
		{
			return static_cast<int>(m_FileNameList.size());
		}
		void Insert(const TSTRING& strFile)
		{
			m_FileNameList.push_back(strFile.c_str());
		}
		const TSTRING GetByIndex (int nIndex) const
		{
			TSTRING strTemp;
			if(nIndex >= 0 && nIndex < GetFileCount())
				strTemp = m_FileNameList[nIndex];
			return strTemp;
		}
		const DROPEFFECT	GetEffect() const {return m_Effect;}
		void				SetEffect(DROPEFFECT effect) {m_Effect = effect;}
		const CPoint&		GetPoint() const {return m_Point;}
		void				SetPoint(const CPoint& point) {m_Point = point;}
		void*				GetPara(){return m_pAppData;}
		void				SetPara(void* pAppData){m_pAppData = pAppData;}
		HWND		GetTargetWnd() const {return m_hTargetWnd;}
		void				SetTargetWnd(HWND hTargetWnd){m_hTargetWnd = hTargetWnd;}
		DWORD		GetKeyState() const {return m_dwKeyState;}
		void				SetKeyState(DWORD dwKeyState){m_dwKeyState = dwKeyState;}
	private:
		vector<TSTRING>		m_FileNameList;
		DROPEFFECT			m_Effect;	
		CPoint				m_Point;
		void*				m_pAppData;
		HWND			m_hTargetWnd;
		DWORD			m_dwKeyState;
	};
    class FireOnSizeParam {
    public:
        FireOnSizeParam(): m_nType(0), m_cx(0), m_cy(0) {}
        virtual ~FireOnSizeParam() {}
        void SetParam(UINT nType, int cx, int cy) {
            m_nType = nType;
            m_cx = cx;
            m_cy = cy;
        }
    public:
        UINT m_nType;
        int  m_cx;
        int  m_cy;
    };
    class FireOnSizingParam {
    public:
        FireOnSizingParam(): m_fwSide(0), m_pRect(NULL) {}
        virtual ~FireOnSizingParam() {}
        void SetParam(UINT fwSide, LPRECT pRect) {
            m_fwSide = fwSide;
            m_pRect = pRect;
        }
    public:
        UINT m_fwSide;
        LPRECT m_pRect;
    };

}
#endif // !defined(BENQ_11AF4308_7773_456a_B0BB_779BA6F9AEF3__INCLUDED_)
