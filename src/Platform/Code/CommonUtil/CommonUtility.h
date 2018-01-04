/*********************************************************************************\
FileName: CommonUtility.h

Descript: A wapper class for CommonUtility

Creater: Alan tu

Create Date:2007/3/27
\*********************************************************************************/

#if !defined(_INC_COMMON_UTILITY_)
#define _INC_COMMON_UTILITY_

#include <Gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")

#include <comutil.h>
#pragma comment(lib, "comsupp.lib")

//for stop warning C4786: identifier was truncated to '255' characters in the debug information
#pragma warning( disable : 4786 )
#include <map>

//for stop warning C4003: not enough actual parameters for macro 'GlobalFree'
#pragma warning( disable : 4003 )

namespace COMMONCONTROL
{
#ifdef _UNICODE
#define ConverWString(TSTR_S) TSTR_S
#else
#define ConverWString(TSTR_S) _com_util::ConvertStringToBSTR(TSTR_S)
#endif

#ifdef _UNICODE
#define ConverString(TSTR_S) _com_util::ConvertBSTRToString(TSTR_S)
#else
#define ConverString(TSTR_S) TSTR_S
#endif

	using namespace Gdiplus;

	template<class Creater>
	class GlobalCreater
	{
	public:
		template<class Creater>
		class GlobalCreaterBase
		{
			friend class GlobalCreater<Creater>;
		protected:
			static void init(){
				m_nRefCoune()++;
				if(!isInited()){
					inited();
					Creater::init();
				}
			}
			static void unInit(){
				m_nRefCoune()--;
				if(isInited() && m_nRefCoune() == 0){
					uninited();
					Creater::unInit();
				}
			}
		private:
			GlobalCreaterBase(){}
			~GlobalCreaterBase(){}
			static bool isInited() {return m_inited();}
			static void inited() {m_inited() = true;}
			static void uninited(){m_inited() = false;}
			static bool& m_inited(){static bool m_bInited = false; return m_bInited;}
			static int& m_nRefCoune(){static int m_nRefCoune = 0;return m_nRefCoune;}
		};

	public:
		GlobalCreater(){GlobalCreaterBase<Creater>::init();}
		~GlobalCreater(){GlobalCreaterBase<Creater>::unInit();}
	};

	class _COMCreaterPolicy {
		friend class GlobalCreater<_COMCreaterPolicy>::GlobalCreaterBase<_COMCreaterPolicy>;
	protected:
		static void init() {
			::CoInitialize(NULL);
		}
		static void unInit() {
			::CoUninitialize();
		}
	};

	class _GdiplusCreaterPolicy
	{
		friend class GlobalCreater<_GdiplusCreaterPolicy>::GlobalCreaterBase<_GdiplusCreaterPolicy>;
	protected:
		static void init(){
			GdiplusStartupInput gdiplusStartupInput;
			GdiplusStartup(&m_gdiplusToken(), &gdiplusStartupInput, NULL);
		}
		static void unInit(){
			GdiplusShutdown(m_gdiplusToken());
		}
	private:
		static ULONG_PTR& m_gdiplusToken(){static ULONG_PTR m_gdiplusToken = 0; return m_gdiplusToken;}
	};

	class _BestPerformancePolicy
	{
	public:
		static void setSmoothingMode(Graphics& graphics,const CRect& drawingRect){
			if(drawingRect.Width()*drawingRect.Height() > 40000)
				graphics.SetSmoothingMode(SmoothingModeHighSpeed);
			else
				graphics.SetSmoothingMode(SmoothingModeHighQuality);
		}				
	};

	class _HighSpeedPolicy
	{
	public:
		static void setSmoothingMode(Graphics& graphics,const CRect& drawingRect){
			graphics.SetSmoothingMode(SmoothingModeHighSpeed);
		}				
	};

	class _HighQualityPolicy
	{
	public:
		static void setSmoothingMode(Graphics& graphics,const CRect& drawingRect){
			graphics.SetSmoothingMode(SmoothingModeHighQuality);
		}				
	};

	template<class HandleManager>
	class Win32Handle
	{
	public:
		Win32Handle(HGLOBAL handle = NULL):m_handle(handle),m_pData(NULL){}
		~Win32Handle() {
			if(m_pData)::GlobalUnlock(m_handle);
			HandleManager::Free(m_handle);
		}
		LPVOID data(HGLOBAL handle = NULL)
		{
			if(m_handle == NULL) {
				m_handle = handle;
				if(m_handle)
					m_pData = ::GlobalLock(m_handle);
			}
			return m_pData;
		}
	private:
		HGLOBAL m_handle;
		LPVOID	m_pData;
	};

	template<class HandleManager>
	class DIBtoIMAGE
	{
	public:
		DIBtoIMAGE(HANDLE hDib):m_Image(NULL)
		{
			using namespace Gdiplus;
			if (hDib != INVALID_HANDLE_VALUE) {
				BITMAPINFO *pBitmapInfo=(BITMAPINFO *)m_dibHandle.data(hDib);
				if( pBitmapInfo ) {
					void *bitmapData = dibData((char *)pBitmapInfo);
					if (bitmapData) {
						m_Image = Bitmap::FromBITMAPINFO(pBitmapInfo, bitmapData);
						ASSERT(m_Image);
					}
				}
			}
		}
		~DIBtoIMAGE(){if(m_Image)delete m_Image;}
		inline Gdiplus::Image* image() const{return m_Image;}
	private:
		void *dibData(char *buffer)
		{
			BITMAPINFO *bmpinfo=(BITMAPINFO *)buffer;
			long offset=sizeof(BITMAPINFOHEADER);
			switch( bmpinfo->bmiHeader.biBitCount ) {
			case 1:offset+=8;break;
			case 4:offset+=64;break;
			case 8:offset+=1024;
			}
			return buffer+offset;
		}
		Gdiplus::Image* m_Image;
		Win32Handle<HandleManager> m_dibHandle;
		GlobalCreater<_GdiplusCreaterPolicy> m_GdiplusUtility;
	};

	//HandleManager policy
	class _AutoFreeHandle
	{
	public:
		static void Free(HANDLE handle)
		{
			GlobalFree(handle);
			handle = NULL;
		}
	};

	//HandleManager policy
	class _KeepHandle
	{
	public:
		static void Free(HANDLE handle)
		{
		}
	};
/*
	using namespace std;

	template <class KEY,class DATA>
	class SingletonMap
	{
	public:
		static SingletonMap<KEY,DATA>& getInstance() {
			return *getPointToInstance();
		}		
		bool create(const KEY& key,const DATA& data) {			
			bool bRes = false;
			getInstance().m_Storage.insert(Storage::value_type(key,data));
			bRes = true;
			return bRes;
		}		
		bool remove(const KEY& key) {
			bool bRes = false;
			sIter Iter = getInstance().m_Storage.find(key);
			if(Iter != getInstance().m_Storage.end()) {
				getInstance().m_Storage.erase(Iter);
				bRes = true;
			}
			return bRes;
		}		
		bool get(const KEY& key,DATA& data) {
			bool bRes = false;
			sIter Iter = getInstance().m_Storage.find(key);
			if(Iter != getInstance().m_Storage.end()) {
				data = Iter->second;
				bRes = true;
			}			
			return bRes;
		}
		int size(){return getInstance().m_Storage.size();}
		bool getKey(int nIndex,KEY& key) {
			bool bRes = false;
			if(nIndex >= 0 && nIndex < size()) {
				map <KEY, DATA> :: const_iterator cIter;
				cIter = getInstance().m_Storage.begin();
				advance(cIter, nIndex);
				key = cIter->first;
				bRes = true;
			}
			return bRes;
		}
	private:
		SingletonMap(){}
		SingletonMap(const SingletonMap&){}
		SingletonMap& operator=(const SingletonMap&){}
		static SingletonMap<KEY,DATA>* getPointToInstance()	{
			static SingletonMap<KEY,DATA> *m_pInstance = NULL;
			if(!m_pInstance) {
				static SingletonMap<KEY,DATA> theInstance;
				m_pInstance = &theInstance;
			}
			return m_pInstance;
		}

	private:
		typedef map<KEY,DATA> Storage;
		typedef Storage::iterator sIter;
		Storage m_Storage;
	};
	*/
};

#endif //_INC_COMMON_UTILITY_