#pragma once

#include "..\..\CommonUtil\commondatafile.h"
#include <string>
#include <vector>
#include "..\..\ShareLib\loki-0.1.5\include\loki\smartptr.h"
#include "SyncManager.h"

namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;

	class PIMObjectBase :	public CCommonDataFile
	{
	public:	
		PIMObjectBase(const std::string& strPIMType,const GUID& guid)
			:CCommonDataFile(guid),m_strPIMType(strPIMType){}
		virtual ~PIMObjectBase(void){}
		const std::string& GetPIMType(){return m_strPIMType;}
		const std::string& GetPIMDB(){return m_strDB;}
		void EnumCompatiablePIMSpecs(vector<PIMSpecData>& vectSPECs) const
		{
			vectSPECs = m_CompatiablePIMSpecs;
		}
	protected:
		void AddCompatiablePIMSpec(const std::string& strSpecName,const std::string& strVersion)
		{
			PIMSpecData specdata = {strSpecName,strVersion};
			SPECCIter cIter = find(m_CompatiablePIMSpecs.begin(),m_CompatiablePIMSpecs.end(),specdata);
			if(cIter == m_CompatiablePIMSpecs.end())
				m_CompatiablePIMSpecs.push_back(specdata);
		}
	private:
		vector<PIMSpecData>							m_CompatiablePIMSpecs;
		typedef vector<PIMSpecData>::const_iterator	SPECCIter;
		string										m_strPIMType;
		string										m_strDB;
	};

#ifdef WIN32
#include <Objbase.h>
	template <class clsPIMObject>
	class PIMFactory : public IPIM
	{
	public:
		virtual IPIMObjPt CreatePIMObject(GUID guid = GUID_NULL)
		{
			GUID genGuid = guid;
			if(genGuid == GUID_NULL)
			{
				HRESULT  hr = CoCreateGuid(&genGuid);
				assert(S_OK == hr);
			}
			return (new clsPIMObject(genGuid));
		}
	};
#endif


	////////////////////////////////////////////////////////////////////////////
	//Sample for PIMObjectBase
	class samplePIM : public PIMObjectBase
	{
	public:
		samplePIM(const GUID& guid):PIMObjectBase("testPIM",guid)
		{
			AddCompatiablePIMSpec("VCARD","1.0");
			AddCompatiablePIMSpec("QualcommVCARD","1.0");
		}
		virtual ~samplePIM(){}
		virtual bool encode(TSTRING& strRAW)
		{
			bool bRes = false;
			return bRes;
		}
		virtual bool decode(const TSTRING& strRAW)
		{
			bool bRes = false;

			CommonDataPt spData = new CommonData;
			spData->SetName("raw");
#ifdef _UNICODE
			spData->SetWString(strRAW.c_str());
#else
			spData->SetString(strRAW.c_str());
#endif

			SetByName("raw",spData);

			bRes = true;

			return bRes;
		}
		virtual bool encode(void** pVoid,long& nSize)
		{
			bool bRes = false;
			return bRes;
		}
		virtual bool decode(void* pVoid,const long& nSize)
		{
			bool bRes = false;
			return bRes;
		}
	};

	//End for Sample for PIMObjectBase
	////////////////////////////////////////////////////////////////////////////

}
