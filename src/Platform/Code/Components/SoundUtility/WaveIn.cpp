// WaveIn.cpp: implementation of the CWaveIn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WaveIn.h"

//////////////////////////////////////////////////////////////////////
#include <Winsock2.h>
#include <assert.h>
#include "..\ConfigureTool\IConfigure.h"

using namespace QSYNC_LIB;

double g_nFilterRate = 0.1;
bool g_bFirstGetIConfigure = true;

CWaveIn* g_pThis = NULL;
WAVEFORMATEX g_tagFormat;

double GetFilterFromINI(double nDefaultFilterRate)
{
	double nFilterRate = nDefaultFilterRate;

	if(g_bFirstGetIConfigure)
	{		
		typedef IConfigure* (*pFnCreateIConfigure)(LPCTSTR szConfigureTitle, LPCTSTR szFolderName);
		typedef bool (*pFnDeleteIConfigure)(LPCTSTR szConfigureTitle);
		g_bFirstGetIConfigure = false;
#ifdef _DEBUG
		HMODULE hConfigureDll =::LoadLibrary(_T("ConfigureTool_StaticUD.dll"));
#else
		HMODULE hConfigureDll =::LoadLibrary(_T("ConfigureTool_StaticU.dll"));
#endif    
		if(hConfigureDll)
		{			
			pFnCreateIConfigure g_CreateIConfigure = NULL;
			pFnDeleteIConfigure g_DeleteIConfigure = NULL;
			if(g_CreateIConfigure == NULL)
			{
				g_CreateIConfigure = (pFnCreateIConfigure)GetProcAddress(hConfigureDll, "CreateIConfigure");
				g_DeleteIConfigure = (pFnDeleteIConfigure)GetProcAddress(hConfigureDll, "DeleteIConfigure");
			}
			if(g_CreateIConfigure)
			{
				IConfigure* pIConfigure = g_CreateIConfigure(L"SoundUtility",L"SoundUtility");
				if(pIConfigure)
				{
					pIConfigure->Load();
					CCfgInt nCfgFilterRate(pIConfigure,L"FilterRatePercent",nFilterRate*100);
					nFilterRate = nCfgFilterRate/100.0;
					//pIConfigure->Save();
					g_DeleteIConfigure(L"SoundUtility");
				}
			}

			::FreeLibrary(hConfigureDll);
		}
	}

	return nFilterRate;
}

void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	g_nFilterRate = GetFilterFromINI(g_nFilterRate);

	switch(uMsg) {
	case MM_WIM_DATA:

		WAVEHDR* pWaveHdr = ( (WAVEHDR*)dwParam1 );
		CWaveIn* pWaveIn = (CWaveIn*)(pWaveHdr->dwUser);

		if (pWaveHdr && hwi && pWaveIn) 
		{
			if (pWaveHdr->dwFlags && WHDR_DONE == WHDR_DONE) 
			{
				pWaveHdr->dwFlags = 0;
				if ( pWaveIn->IsError(waveInUnprepareHeader(hwi, pWaveHdr, sizeof(WAVEHDR))) ) 
				{
					break;
				}
				if (pWaveHdr->dwBytesRecorded > 0) 
				{
					pWaveIn->AddNewBuffer(pWaveHdr);

#define Realtime_Analysis
#ifdef Realtime_Analysis
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//Analysis
					LPSTR pData = pWaveHdr->lpData;

					//Make sure samples is enough
					const int nNeedSampleCount = 1000;					
					const double n16Delta = 65536 * 2 * g_nFilterRate;

					int nSampleByteSize = g_tagFormat.nBlockAlign / g_tagFormat.nChannels;
					if(pWaveHdr->dwBufferLength > nNeedSampleCount * g_tagFormat.nBlockAlign)
					{
						//Get samples
						int nCountLeft = 0;
						int nCountRight = 0;
						short nShortLeft = 0;//size 16 bits
						short nShortPreviousLeft = 0;//size 16 bits
						short nShortPreviousPreviousLeft = 0;//size 16 bits
						short nShortRight = 0;//size 16 bits
						short nShortPreviousRight = 0;//size 16 bits
						short nShortPreviousPreviousRight = 0;//size 16 bits
						int nTimesLeft = 0;
						int nTimesRight = 0;

						int nAverageFreqLeft = 0;
						int nAverageAmplitudeLeft = 0;
						int nAverageFreqRight = 0;
						int nAverageAmplitudeRight = 0;

						int nHighestLeft = 0;
						int nLowestLeft = 0;
						int nHighestRight = 0;
						int nLowestRight = 0;

						DWORD dwUsed = 0;

						while(nCountLeft < nNeedSampleCount && pWaveHdr->dwBufferLength > dwUsed)
						{
							if(nSampleByteSize == 2)
							{
								short* pShort =  (short*)pData;
								//nShort = ntohs(*pShort);
								nShortLeft = *pShort;

								if(g_tagFormat.nChannels == 2)
								{
									short* pShortRight =  pShort + 1;
									//nShort = ntohs(*pShort);
									nShortRight = *pShortRight;
								}
							}
							else
							{
								assert(0);
							}

							if(nShortPreviousLeft > nShortLeft && nShortPreviousLeft > nShortPreviousPreviousLeft)
							{
								nHighestLeft = nShortPreviousLeft;

								//PostMessage(g_hCallbackWnd,123,2,nShortPrevious);//amplitude
								if((nHighestLeft - nLowestLeft) > n16Delta)
								{
									int nFreqLeft = g_tagFormat.nSamplesPerSec / nTimesLeft;
									//PostMessage(g_hCallbackWnd,123,3,nFreq);
									nAverageFreqLeft += nFreqLeft;
									nAverageAmplitudeLeft += nShortPreviousLeft;
									nTimesLeft = 0;//Reset time			
									nCountLeft++;
								}
							}
							else if(nShortPreviousLeft < nShortLeft && nShortPreviousLeft < nShortPreviousPreviousLeft)
							{
								nLowestLeft = nShortPreviousLeft;
								//PostMessage(g_hCallbackWnd,123,1,nShortPrevious);//amplitude
							}
							else
							{
								//PostMessage(g_hCallbackWnd,123,1,nShortPrevious);//amplitude
							}

							nTimesLeft++;
							nShortPreviousPreviousLeft = nShortPreviousLeft;
							nShortPreviousLeft = nShortLeft;

							if(g_tagFormat.nChannels == 2)
							{							
								if(nShortPreviousRight> nShortRight && nShortPreviousRight > nShortPreviousPreviousRight)
								{
									nHighestRight = nShortPreviousRight;

									//PostMessage(g_hCallbackWnd,123,2,nShortPrevious);//amplitude
									if((nHighestRight - nLowestRight) > n16Delta && nTimesRight > 0)
									{
										int nFreqRight = g_tagFormat.nSamplesPerSec / nTimesRight;
										//PostMessage(g_hCallbackWnd,123,3,nFreq);
										nAverageFreqRight += nFreqRight;
										nAverageAmplitudeRight += nShortPreviousRight;
										nTimesRight = 0;//Reset time			
										nCountRight++;
									}
								}
								else if(nShortPreviousRight < nShortRight && nShortPreviousRight < nShortPreviousPreviousRight)
								{
									nLowestRight = nShortPreviousRight;
									//PostMessage(g_hCallbackWnd,123,1,nShortPrevious);//amplitude
								}
								else
								{
									//PostMessage(g_hCallbackWnd,123,1,nShortPrevious);//amplitude
								}
							}

							nTimesRight++;
							nShortPreviousPreviousRight = nShortPreviousRight;
							nShortPreviousRight = nShortRight;

							if(g_tagFormat.nBlockAlign*(nTimesLeft+1) > pWaveHdr->dwBufferLength )
								break;
							pData += g_tagFormat.nBlockAlign;
							dwUsed += g_tagFormat.nBlockAlign;
						};//End of while(nCount < nNeedSampleCount)				

						if(nCountLeft > 0)
						{
							nAverageFreqLeft = nAverageFreqLeft / nCountLeft;
							nAverageAmplitudeLeft = nAverageAmplitudeLeft / nCountLeft;
						}
						if(nCountRight > 0)
						{
							nAverageFreqRight = nAverageFreqRight / nCountRight;
							nAverageAmplitudeRight = nAverageAmplitudeRight / nCountRight;
						}

						if(g_pThis)
						{
							g_pThis->m_nAmplitudeLeft += nAverageAmplitudeLeft;
							g_pThis->m_nFrequenceLeft += nAverageFreqLeft;
							g_pThis->m_nAmplitudeRight += nAverageAmplitudeRight;
							g_pThis->m_nFrequenceRight += nAverageFreqRight;
							g_pThis->m_nSampleCount++;
							if(g_pThis->GetCallbackWnd())
							{
								PostMessage(g_pThis->GetCallbackWnd(),123,0,nAverageFreqLeft);
								PostMessage(g_pThis->GetCallbackWnd(),123,4,nAverageAmplitudeLeft);
							}
						}
					}
					/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //endif 
				}
				delete[] pWaveHdr->lpData;
				pWaveHdr->lpData = NULL;
			}

			if ( !pWaveIn->ResetRequired(pWaveIn) ) 
			{
				if ( !pWaveIn->AddNewHeader(hwi) ) 
				{
					break;
				}
			}
		}
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CWaveIn::CWaveIn(WAVEFORMATEX tagFormat, const CWaveDevice& aDevice) 
:m_waveDevice(aDevice)
,m_hWaveIn(0)
,m_nIndexWaveHdr(NUMWAVEINHDR - 1)
,m_bResetRequired(true)
,m_hCallbackWnd(NULL)
,m_nAmplitudeLeft(0)
,m_nFrequenceLeft(0)
,m_nAmplitudeRight(0)
,m_nFrequenceRight(0)
,m_nSampleCount(0)
{
	g_pThis = this;
	SetWaveFormat(tagFormat);
	InitListOfHeader();
}

//////////////////////////////////////////////////////////////////////
CWaveIn::CWaveIn() 
:m_hWaveIn(0)
,m_bResetRequired(true)
,m_hCallbackWnd(NULL)
,m_nAmplitudeLeft(0)
,m_nFrequenceLeft(0)
,m_nAmplitudeRight(0)
,m_nFrequenceRight(0)
,m_nSampleCount(0)
{
	g_pThis = this;
	InitListOfHeader();
}

//////////////////////////////////////////////////////////////////////
CWaveIn::~CWaveIn()
{
	Close();
	FreeListOfBuffer();
	FreeListOfHeader();
}

//////////////////////////////////////////////////////////////////////
// Initialisation
//////////////////////////////////////////////////////////////////////

void CWaveIn::SetCallbackWnd(HWND hCallbackWnd)
{
	m_hCallbackWnd = hCallbackWnd;
}

HWND CWaveIn::GetCallbackWnd()
{
	return m_hCallbackWnd;
}

int CWaveIn::GetAmplitudeLeft()
{
	int nValue = m_nAmplitudeLeft;
	if(nValue != 0 && m_nSampleCount != 0)
		nValue = m_nAmplitudeLeft/m_nSampleCount;
	return nValue;
}

int CWaveIn::GetFrequenceLeft()
{
	int nValue = m_nFrequenceLeft;
	if(nValue != 0 && m_nSampleCount != 0)
		nValue = m_nFrequenceLeft/m_nSampleCount;
	return nValue;
}

int CWaveIn::GetAmplitudeRight()
{
	int nValue = m_nAmplitudeRight;
	if(nValue != 0 && m_nSampleCount != 0)
		nValue = m_nAmplitudeRight/m_nSampleCount;
	return nValue;
}

int CWaveIn::GetFrequenceRight()
{
	int nValue = m_nFrequenceRight;
	if(nValue != 0 && m_nSampleCount != 0)
		nValue = m_nFrequenceRight/m_nSampleCount;
	return nValue;
}
//////////////////////////////////////////////////////////////////////
void CWaveIn::SetDevice(const CWaveDevice &aDevice)
{
	m_waveDevice = aDevice;
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::SetWaveFormat(WAVEFORMATEX tagFormat)
{
	memcpy(&g_tagFormat,&tagFormat,sizeof(WAVEFORMATEX));
	m_wave.BuildFormat(tagFormat.nChannels, tagFormat.nSamplesPerSec, tagFormat.wBitsPerSample);
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::InitListOfHeader()
{
	for (int i = 0; i < NUMWAVEINHDR; i++) {
		m_tagWaveHdr[i].lpData = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
// Son
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
bool CWaveIn::Close()
{
	if (m_hWaveIn != NULL) {
		if ( !Stop() ) {
			return false;
		}
		if ( IsError( waveInClose(m_hWaveIn)) ) {
			return false;
		}
		m_hWaveIn = 0;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Continue()
{
	if (m_hWaveIn) {
		return !IsError( waveInStart(m_hWaveIn) );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Open()
{
	return !IsError( waveInOpen(&m_hWaveIn, m_waveDevice.GetDevice(), &m_wave.GetFormat(), (DWORD)waveInProc, NULL, CALLBACK_FUNCTION) );
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Pause()
{
	if (m_hWaveIn) {
		return !IsError( waveInStop(m_hWaveIn) );
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Record(UINT nTaille/* = 4096*/)
{
	ASSERT(nTaille > 0);
	ASSERT(m_hWaveIn);

	if ( !Stop() ) {
		return false;
	}
	m_bResetRequired = false;
	FreeListOfBuffer();
	FreeListOfHeader();
	SetWaveFormat( m_wave.GetFormat() );
	m_nIndexWaveHdr = NUMWAVEINHDR - 1;
	m_nBufferSize = nTaille;
	for (int i = 0; i < NUMWAVEINHDR; i++) 
	{
		if ( !AddNewHeader(m_hWaveIn) ) 
		{
			return false;
		}
	}
	if ( IsError(waveInStart(m_hWaveIn)) ) 
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::Stop()
{
	if (m_hWaveIn != NULL) {
		m_bResetRequired = true;
		::Sleep(10);
		if ( IsError(waveInReset(m_hWaveIn)) ) {
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::AddNewBuffer(WAVEHDR *pWaveHdr)
{
	ASSERT(pWaveHdr);

	m_listOfBuffer.AddTail(new CWaveBuffer);
	( (CWaveBuffer*)m_listOfBuffer.GetTail() )->CopyBuffer( pWaveHdr->lpData, \
		pWaveHdr->dwBytesRecorded / m_wave.GetFormat().nBlockAlign, \
		m_wave.GetFormat().nBlockAlign );
	return true;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::AddNewHeader(HWAVEIN hwi)
{
	ASSERT(m_nBufferSize > 0);

	m_nIndexWaveHdr = (m_nIndexWaveHdr == NUMWAVEINHDR - 1) ? 0 : m_nIndexWaveHdr + 1;
	if (m_tagWaveHdr[m_nIndexWaveHdr].lpData == NULL) 
	{
		m_tagWaveHdr[m_nIndexWaveHdr].lpData = new char[m_nBufferSize];
	}
	ZeroMemory(m_tagWaveHdr[m_nIndexWaveHdr].lpData, m_nBufferSize);
	m_tagWaveHdr[m_nIndexWaveHdr].dwBufferLength = m_nBufferSize;
	m_tagWaveHdr[m_nIndexWaveHdr].dwFlags = 0;
	m_tagWaveHdr[m_nIndexWaveHdr].dwUser = (DWORD)(void*)this;
	if ( IsError(waveInPrepareHeader(hwi, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR))) ) 
	{
		return false;
	}
	if ( IsError(waveInAddBuffer(hwi, &m_tagWaveHdr[m_nIndexWaveHdr], sizeof(WAVEHDR))) ) {
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::FreeListOfHeader()
{
	for (int i = 0; i < NUMWAVEINHDR; i++) {
		delete[] m_tagWaveHdr[i].lpData;
		m_tagWaveHdr[i].lpData = NULL;
	}
}

//////////////////////////////////////////////////////////////////////
void CWaveIn::FreeListOfBuffer()
{
	POSITION pos = m_listOfBuffer.GetHeadPosition();
	while (pos) {
		CWaveBuffer* pBuf = (CWaveBuffer*)m_listOfBuffer.GetNext(pos);
		if (pBuf) {
			delete pBuf;
			pBuf = NULL;
		}
	}
	m_listOfBuffer.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
// GET
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
DWORD CWaveIn::GetNumSamples()
{
	DWORD dwTotal = 0L;
	POSITION pos = m_listOfBuffer.GetHeadPosition();
	while (pos) 
	{
		CWaveBuffer* p_waveBuffer = (CWaveBuffer*) m_listOfBuffer.GetNext(pos);
		dwTotal += p_waveBuffer->GetNumSamples();
	}
	return dwTotal;
}

//////////////////////////////////////////////////////////////////////
CString CWaveIn::GetError() const
{
	if (m_nError != MMSYSERR_NOERROR) 
	{
		TCHAR szText[MAXERRORLENGTH + 1] = {0};
		if ( waveInGetErrorText(m_nError, szText, MAXERRORLENGTH) == MMSYSERR_NOERROR ) {
			return szText;
		}
	}
	return _T("");
}

//////////////////////////////////////////////////////////////////////
DWORD CWaveIn::GetPosition()
{
	if (m_hWaveIn) {
		MMTIME mmt;
		mmt.wType = TIME_SAMPLES;
		if ( IsError(waveInGetPosition(m_hWaveIn, &mmt, sizeof(MMTIME))) ) {
			return -1;
		}
		else {
			return mmt.u.sample;
		}
	}
	return -1;
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::IsError(MMRESULT nResult)
{
	m_nError = nResult;
	return (m_nError != MMSYSERR_NOERROR);
}

//////////////////////////////////////////////////////////////////////
bool CWaveIn::IsRecording()
{
	bool bResult = false;
	if (m_nIndexWaveHdr > -1 && m_tagWaveHdr[m_nIndexWaveHdr].dwFlags != 0) 
	{
		bResult |= !(m_tagWaveHdr[m_nIndexWaveHdr].dwFlags & WHDR_DONE == WHDR_DONE);
	}
	return bResult;
}

//////////////////////////////////////////////////////////////////////
CWave CWaveIn::MakeWave()
{
	void* pBuffer = new char[GetNumSamples() * m_wave.GetFormat().nBlockAlign];
	DWORD dwPosInBuffer = 0L;
	POSITION pos = m_listOfBuffer.GetHeadPosition();
	while (pos) 
	{
		CWaveBuffer* p_waveBuffer = (CWaveBuffer*) m_listOfBuffer.GetNext(pos);
		CopyMemory( (char*)pBuffer + dwPosInBuffer, p_waveBuffer->GetBuffer(), p_waveBuffer->GetNumSamples() * p_waveBuffer->GetSampleSize() );
		dwPosInBuffer += p_waveBuffer->GetNumSamples() * p_waveBuffer->GetSampleSize();
	}
	m_wave.SetBuffer( pBuffer, GetNumSamples() );
	return m_wave;
}


//////////////////////////////////////////////////////////////////////
bool CWaveIn::ResetRequired(CWaveIn* pWaveIn)
{
	return m_bResetRequired;
}

