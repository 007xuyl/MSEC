
/**
 * Tencent is pleased to support the open source community by making MSEC available.
 *
 * Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the GNU General Public License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License. You may 
 * obtain a copy of the License at
 *
 *     https://opensource.org/licenses/GPL-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the 
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions
 * and limitations under the License.
 */


#ifndef __TCE_COMM_MGR_H__
#define __TCE_COMM_MGR_H__

#include "tce_singleton.h"
#include "tce_udp_server.h"
#include "tce_tcp_server.h"
#include "fifo_buffer.h"
#include "tce_timer.h"
#include "tce_utils.h"
#include "function.h"
#include "tce_event.h"
#include <string>
#include <map>
#include <deque>
#include <set>

#ifndef NULL
#define NULL 0
#endif

namespace tce{

class CCommMgr
	:private CNonCopyAble
{
	struct SEventInfo{
		int32_t iID;
	};
public:
	typedef int32_t (* ONREADFUNC)(SSession&, const unsigned char*, const size_t);
	typedef void (* ONCLOSEFUNC)(SSession&);
	typedef void (* ONCONNECTFUNC)(SSession&, const bool);
	typedef void (* ONERRORFUNC)(SSession&, const int32_t iErrCode, const char* pszErrMsg);
	typedef void (* ONTIMERFUNC)(const int32_t);
	typedef void (* CALLBACKFUNC)(void);

	typedef std::map<int32_t, CEvent*> MAP_EVENT;
	
public:
	enum COMM_TYPE{
		CT_ERROR,
		CT_UDP_SVR,
		CT_TCP_SVR,
		CT_HTTP_SVR,
	};

	//�ײ�TCP�ְ���ʽ
	enum TCP_DGRAM_TYPE{
		TDT_H2SHORTT3,						//��һ�ֽ�Ϊ0x02�����µ�2�ֽ�Ϊ�������ĳ��ȣ����һ�ֽ�Ϊ0x03
		TDT_H5SHORTT3,						//��һ�ֽ�Ϊ0x05�����µ�2�ֽ�Ϊ�������ĳ��ȣ����һ�ֽ�Ϊ0x03
		TDT_H2LONGT3,						//��һ�ֽ�Ϊ0x02�����µ�4�ֽ�Ϊ�������ĳ��ȣ����һ�ֽ�Ϊ0x03
		TDT_HTTP,							//��HTTPЭ��ְ�
		TDT_LONGLEN_EXCLUDE_ITSELF,			//��ʼ4�ֽ�Ϊ�������ĳ���,Packet's total length (excludes itself)
		TDT_LONGLEN_INCLUDE_ITSELF,			//��ʼ4�ֽ�Ϊ�������ĳ���,Packet's total length (includes itself)
		TDT_H5_VER_SHORT_T3,				//ȺͼƬ��������ֵ�Э���ʽ ����һ�ֽ�Ϊ0x05�����µ�2�ֽ�Ϊ�汾�ţ����µ�2�ֽ�Ϊ�������ĳ��ȣ����һ�ֽ�Ϊ0x03��
		TDT_EMPTY,							//û�г��ȱ�ʶ��ֻҪ�����ݾ����ϲ㷢��
		TDT_HTTP_RESPONSE_PART,				//����httpЭ���Ӧ������һ������֤����������httpЭ��ͷ������ֻҪ�����ݾ����ϲ㷢��. for http proxy client
		TDT_ASN,					//����ASNЭ���ʽ. add by kylehuang 2013-06-20. for cmem.
	};

	enum SOCKET_MANAGE_TYPE{
		SMT_ARRAY,				//һ��ʹ�÷�ʽ(���߳�ģʽ�������ٶȻ���һЩ)
		SMT_ARRAY_FAST,			//�������ڷ�������������ʹ�ã�˫�߳�ģʽ�������ٶȻ�ȽϿ죩
	};
	enum {
		MAX_COMM_SIZE = 16,
	};
private:
	struct SCommConfig{
		SCommConfig()
			:bRun(false)
			,sIp("127.0.0.1")
			,wPort(80)
			,nInBufferSize(10*1024*1024)
			,nOutBufferSize(10*1024*1024)
			,nMaxFdInSize(4*1024*1024)
			,nMaxFdOutSize(4*1024*1024)
			,nMaxMallocMem(100*1024*1024)
			,nMaxClient(10000)
			,nOverTime(180)
			,nDelayStartTime(0)
			,nMallocItemSize(4*1024)
			,nCommType(CT_ERROR)
			,nTcpDgramType(TDT_H2SHORTT3)
			,nSockManageType(SMT_ARRAY)
			,poInBuffer(NULL)
			,poOutBuffer(NULL)
			,poSvr(NULL)
			,pOnReadFunc(NULL)
			,pOnCloseFunc(NULL)
			,pOnConnectFunc(NULL)
			,pOnErrorFunc(NULL)
		{
		}

		bool bRun;
		std::string sIp;
		uint16_t wPort;
		size_t nInBufferSize;
		size_t nOutBufferSize;
		size_t nMaxFdInSize;
		size_t nMaxFdOutSize;
		size_t nMaxMallocMem;
		size_t nMaxClient;
		time_t nOverTime;
		time_t nDelayStartTime;
		size_t nMallocItemSize;
		COMM_TYPE nCommType;
		TCP_DGRAM_TYPE nTcpDgramType;
		SOCKET_MANAGE_TYPE nSockManageType;
		
		CFIFOBuffer* poInBuffer;
		CFIFOBuffer* poOutBuffer;
		CServerBase* poSvr;
		ONREADFUNC pOnReadFunc;
		ONCLOSEFUNC pOnCloseFunc;
		ONCONNECTFUNC pOnConnectFunc;
		ONERRORFUNC pOnErrorFunc;

	};
	struct SRunComm{
		SRunComm(){	memset(this, 0, sizeof(SRunComm));	}
		CFIFOBuffer* poInBuffer;
		SCommConfig* pstCommConfig;
		int32_t 	iCommID;
	};

	typedef std::map<int32_t, SCommConfig> MAP_COMM_CONFIG;
	typedef std::vector<SCommConfig*> VEC_COMM_CONFIG;
	typedef std::set<int32_t> SET_COMMID;
	typedef std::deque<int32_t> DEQ_COMMID;

public:
	~CCommMgr();
	static CCommMgr& GetInstance(){
		if (NULL == m_pInstance){
			m_pInstance = new CCommMgr;
		}
		return *m_pInstance;
	}

	//void Release(){
	//	if (NULL != m_pInstance){
	//		delete m_pInstance;
	//		m_pInstance = NULL;
	//	}
	//}

	int32_t CreateSvr(const COMM_TYPE nCommType, const std::string& sBindIp, const uint16_t wBindPort, const size_t nInBufferSize=10*1024*1024, const size_t nOutBufferSize=10*1024*1024);
	bool SetSvrDgramType(const int32_t iCommID, const TCP_DGRAM_TYPE nTcpDgramType=TDT_H2LONGT3);
	bool SetSvrClientOpt(const int32_t iCommID,
						const size_t nMaxClient=10000,
						const time_t nOverTime=180,
						const size_t nMaxFdInSize=4*1024*1024,
						const size_t nMaxFdOutSize=4*1024*1024,
						const size_t nMaxMallocMem=100*1024*1024,
						const size_t nMallocItemSize=4*1024);
	bool SetSvrDelayStart(const int32_t iCommID, const size_t nDelayStartTime=0);

	bool SetSvrSockManageType(const int32_t iCommID, const SOCKET_MANAGE_TYPE nSockManageType=SMT_ARRAY);

	bool SetSvrCallbackFunc(const int32_t iCommID, ONREADFUNC pOnReadFunc, ONCLOSEFUNC pOnCloseFunc=NULL, ONCONNECTFUNC pOnConnectFunc=NULL, ONERRORFUNC pOnErrorFunc=NULL);

	bool SetTimerCallbackFunc(ONTIMERFUNC pOnTimerFunc);

	void Stop();

	bool RunAllSvr();

	bool RunSvr(const int32_t iCommID);

	bool StopSvr(const int32_t iCommID);
	bool StopAllSvr();
	bool CloseSvr(const int32_t iCommID);
	bool StopTimer();

	bool SetTimer(CALLBACKFUNC pFunc,		//��ʱ���ص�����
		const int32_t iElapse,			//����
		const bool bAutoDel=false)	//�Ƿ��Զ�ɾ����ʱ��
	{
		return m_oTimer.Add(pFunc, iElapse, bAutoDel);
	}
	bool KillTimer(CALLBACKFUNC pFunc) //��ʱ��
	{
		return m_oTimer.Del(pFunc);
	}
	bool ResetTimer(CALLBACKFUNC pFunc)	//���趨ʱ��
	{
		return m_oTimer.Reset(pFunc);
	}

	bool SetTimer(const int32_t iId,		//��ʱ��ID
				const int32_t iElapse,			//����
				const bool bAutoDel=false)	//�Ƿ��Զ�ɾ����ʱ��
	{
		return m_oTimer.Add(iId, iElapse, bAutoDel);
	}
	bool KillTimer(const int32_t iId) //��ʱ��ID
	{
		return m_oTimer.Del(iId);
	}
	bool ResetTimer(const int32_t iId)	//���趨ʱ��
	{
		return m_oTimer.Reset(iId);
	}

	bool Start();

	bool StartByThread();

	bool Write(SSession& stSession,const char* pszData1, const size_t nSize1,const char* pszData2, const size_t nSize2);
	bool Write(SSession& stSession,const char* pszData, const size_t nSize);
	inline bool Write(SSession& stSession,const unsigned char* pszData, const size_t nSize)	{		return this->Write(stSession, (const char*)pszData, nSize);	}
	inline bool Write(SSession& stSession, const std::string& sData){		return this->Write(stSession, sData.data(), sData.size());	}


	bool WriteTo(const int32_t iCommID, const std::string& sIp, const uint16_t wPort, const unsigned char* pszData, const size_t nSize);
	inline bool WriteTo(const int32_t iCommID, const std::string& sIp, const uint16_t wPort, const char* pszData, const size_t nSize){
		return this->WriteTo(iCommID, sIp, wPort, reinterpret_cast<const unsigned char*>(pszData), nSize);
	}
	inline bool WriteTo(const int32_t iCommID, const std::string& sIp, const uint16_t wPort, const std::string& sData){
		return this->WriteTo(iCommID, sIp, wPort, sData.data(), sData.size());
	}

	bool Close(SSession& stSession, const time_t nCloseWaitTime=0);

	bool Connect(const int32_t iCommID, const std::string& sIp, const uint16_t wPort, const int64_t nParam1=0, const void* pParam2=0);
	bool Connect(const int32_t iCommID, const uint32_t dwIp, const uint16_t wPort, const int64_t nParam1=0, const void* pParam2=0);


	int32_t AddEvent(CEvent* poEvent);

	const char* GetErrMsg() const {	return m_szErrMsg;	}
private:
//	bool StopTimer();	//ֹͣ��ʱ��

	bool RunTimer();	//������ʱ��

	bool CreateTCPSvr();

private:
	CCommMgr();

	int32_t OnRead(SCommConfig& stCommConfig, SSession& stSession, const unsigned char* pszData, const size_t nSize);

	inline void CallBackErrFunc(const int32_t iErrCode, const char* pszErrMsg){
		if ( NULL != m_pOnErrorFunc )
		{
			SSession stSession;
			m_pOnErrorFunc(stSession, iErrCode, pszErrMsg);
		}
		else
		{
			printf("[tce_error]errcode=%d, msg=%s", iErrCode, pszErrMsg);
		}
	}
private:
	static CCommMgr* m_pInstance;

//	MAP_COMM_CONFIG m_mapNoRunSvrs;
//	MAP_COMM_CONFIG m_mapRunSvrs;

	SCommConfig* m_arComms[MAX_COMM_SIZE];

	SET_COMMID m_setRunCommIDs;
	DEQ_COMMID m_deqIdleCommIDs;
	
	SRunComm m_arrRunComms[MAX_COMM_SIZE];
	

	char m_szErrMsg[1024];

	int32_t m_iCommMaxId;
	int32_t m_iUdpMaxCommID;
	int32_t m_iTcpMaxCommID;
	
	CTimer<int64_t> m_oTimer;

	ONTIMERFUNC m_pOnTimerFunc;
	ONERRORFUNC m_pOnErrorFunc;
	bool m_bStart;

	MAP_EVENT m_mapEvents;
};

};


#endif
