/*****************************************************************************/
/** @file:		RequestManager.cpp
This is the file that contains the functions related to the General class.

$Author:   Alan Tu  $
$Version:		1.0 $
$Date:   Mar 24 2008 10:53:22  $
*/
/*****************************************************************************/
#include "stdafx.h"
#include "General.h"
#include "RequestManager.h"
#include "GeneralRequest.h"

/*---------------------------------------------------------------------------*/
/*!
construction

@param		None
@return		None
@author		Eddy Guo
@date			2007/01/30
*/
CGeneral::CGeneral():RequestFunctionBase(INFINITE)
{
}

/*---------------------------------------------------------------------------*/
/*!
destruction

@param		None
@return		None
@author		Eddy Guo
@date			2007/01/30
*/
CGeneral::~CGeneral()
{
}

/*---------------------------------------------------------------------------*/
/*!
set port

@param		CString cstrPort - the port to be set
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date			2007/01/30
*/
HRESULT CGeneral::SetPort(CString cstrPort)
{
	HRESULT hr = S_OK;
	int nWaitCounter = 0;

	if (!g_RequestManager.IsWorkingThreadActive())
		g_RequestManager.StartUp();

	try
	{
		CRequestSetPortPtr spRequest = new CRequestSetPort(cstrPort);

		hr = g_RequestManager.EnqueueRequest(spRequest.GetRawPointer());

		if (hr == S_OK)
		{
			while (1)
			{
				if (spRequest->IsRequstCompleted())
				{
					if (spRequest->IsRequstCompletedFailed())
						hr = E_FAIL;

					break;
				}

				// time out
				if (nWaitCounter >= 50)
				{
					hr = E_FAIL;
					break;
				}

				Sleep(100);
				nWaitCounter++;
			}
		}
		else
			hr = E_FAIL;
	}
	catch(...)
	{
		hr = E_FAIL;
	}

	return hr;
}

HRESULT CGeneral::ResetPort()
{
	CRequestResetPortPtr spRequest = new CRequestResetPort();
	return g_RequestManager.EnqueueRequest(spRequest.GetRawPointer());
}

HRESULT CGeneral::GetPhoneModel(CString *pstrModel)
{
	CRequestPtr ptRequest = new CRequestGetPhoneModel(pstrModel);
	return Execute(ptRequest,m_nTimeOut) > 0;	
}

HRESULT CGeneral::GetIMEI(CString *pstrIMEI)
{
	HRESULT hr = eCommErrWithoutIMEI;
	bool bRes = false;
	TSTRING strIMEI;
	CRequestPtr ptRequest = new CRequestGetIMEI(strIMEI,bRes,m_nTimeOut);	
	Execute(ptRequest,m_nTimeOut);
	if(bRes && pstrIMEI)
	{
		*pstrIMEI = strIMEI.c_str();
		hr = S_OK;
	}
	return hr;
}

HRESULT CGeneral::GetSoftwareVer(CString* pstrSoftwareVer)
{
	CRequestPtr ptRequest = new CRequestGetSoftwareVer(pstrSoftwareVer);
	return Execute(ptRequest,m_nTimeOut) > 0;	
}

/*---------------------------------------------------------------------------*/
/*!
get get manufacturer

@param		CString* pcstrManufacturer - a referenced pointer to deliver the 
manufacturer
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date			2007/01/30
*/
HRESULT CGeneral::GetManufacturer(CString *pstrManufacturer)
{
	CRequestPtr ptRequest = new CRequestGetManufacturer(pstrManufacturer);
	return Execute(ptRequest,m_nTimeOut) > 0;	
}

HRESULT CGeneral::GetFDNStatus(bool& bIsFDN)
{
	CRequestPtr ptRequest = new CRequestGetFDN(bIsFDN);
	return Execute(ptRequest,m_nTimeOut) > 0;	
}

HRESULT CGeneral::GetConnectivity(bool& bIsOnline) {
    CRequestPtr ptRequest = new CRequestCheckConnectivity(bIsOnline);
    return Execute(ptRequest,m_nTimeOut) > 0;	
}

/*---------------------------------------------------------------------------*/
/*!
get mobile information

@param		CString *pcstrModel - a referenced pointer to delever the phone 
model
@param		CString *pcstrIMEI - a referenced pointer to delever the IMEI
@param		CString* pcstrSoftwareVer - a referenced pointer to delever the 
software version
@return		HRESULT - S_OK if successful; E_FAIL if failed
@author		Eddy Guo
@date			2007/01/30
*/
HRESULT CGeneral::GetMobileInfo(CString* pstrModel, CString* pstrIMEI, CString* pstrSoftwareVer)
{
	HRESULT hr = GetIMEI(pstrIMEI);
	if(hr == S_OK)
		hr = GetSoftwareVer(pstrSoftwareVer);
	if(hr == S_OK)
		hr = GetPhoneModel(pstrModel);
	return hr;
}

HRESULT CGeneral::CheckPassword(CString strPassword, BOOL *pbPasswordValid)
{
	HRESULT hr = S_OK;

	*pbPasswordValid = FALSE;

	if ((strPassword.GetLength() > 8) || (strPassword.GetLength() == 0))
		hr = E_INVALIDARG;
	else
	{
		try
		{
			CRequestCheckPasswordPtr spRequest = new CRequestCheckPassword(strPassword);

			hr = g_RequestManager.EnqueueRequest(spRequest.GetRawPointer());

			if (hr == S_OK)
			{
				while (1)
				{
					if (spRequest->IsRequstCompleted())
					{
						if (!spRequest->IsRequstCompletedFailed())
							*pbPasswordValid = TRUE;
						else
							hr = E_FAIL;

						break;
					}

					if (!g_RequestManager.IsConnected())
					{
						hr = E_FAIL;
						break;
					}
				}
			}
			else
				hr = E_FAIL; 
		}
		catch(...)
		{
			hr = E_FAIL;
		}
	}

	return hr;
}

HRESULT CGeneral::SetCommandEchoOff(bool bIsEcho)
{
    HRESULT hr = S_OK;

    try
    {
        CRequestEchoOffPtr spRequest = new CRequestEchoOff();

        hr = g_RequestManager.EnqueueRequest(spRequest.GetRawPointer());

        if (hr == S_OK)
        {
            while (1)
            {
                if (spRequest->IsRequstCompleted())
                {
                    if (!spRequest->IsRequstCompletedFailed())
                        //*pbPasswordValid = TRUE;
                        ;
                    else
                        hr = E_FAIL;

                    break;
                }

                if (!g_RequestManager.IsConnected())
                {
                    hr = E_FAIL;
                    break;
                }
            }
        }
        else
            hr = E_FAIL; 
    }
    catch(...)
    {
        hr = E_FAIL;
    }

    return hr;
}