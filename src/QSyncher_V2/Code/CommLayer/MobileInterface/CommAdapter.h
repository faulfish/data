#ifndef _BENQ_MOBILEPHONE_COMM_ADAPTER_H_
#define _BENQ_MOBILEPHONE_COMM_ADAPTER_H_

#pragma once

#include "CommInterface.h"

class CCommProxy;

class CCommAdapter
{
public:
	CCommAdapter();
	CCommAdapter( CCommProxy* pCommProxy );
public:
	~CCommAdapter();

private:
  CommInterfaceList m_vpCommInterface;
  CCommProxy*       m_pCommProxy;

public:
  void SetProxy( CCommProxy* pCommProxy);
  void LockAllInterface( BOOL bLock );
  BOOL AdaptAllinterface();
  BOOL CreateInterface( CommInterface eInterface, CCommInterface* &pInterface );
  void DeleteInterface( CCommInterface *pInterface );
  BOOL DeleteAllinterface();
  int  GetRefCount( CommInterface eInterface );
};

#endif