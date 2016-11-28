
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


#include <iostream>
#include "tce.h"

using namespace std;
using namespace tce;

tce::CFileLog g_oFileLog;

#ifdef WIN32
#define SVR_IP "127.0.0.1"
#else
#define SVR_IP "127.0.0.1"
#endif

#define TCP_SVR_IP "127.0.0.1"
#define UDP_SVR_IP "127.0.0.1"
#define TCP_SVR_PORT 30003
#define UDP_SVR_PORT 30003


int g_iTcpCommID = 0;
int g_iUdpCommID = 0;

bool g_bConnectOk = false;
tce::SSession g_stSvrSession;

//���������¼�����OnRead����
int OnRead(tce::SSession& stSession, const unsigned char* pszData, const size_t nSize){


	if ( g_iTcpCommID == stSession.GetCommID() )
	{
		//do tcp data
	} 
	else
	{
		//do udp data
	}


	return 0;
}

//tcp���ӹرմ���OnClose����
void OnClose(tce::SSession& stSession){
//	cout << "OnClose" << endl;

	if ( stSession == g_stSvrSession )
	{
		g_bConnectOk = false;
	}

}

//�������ӳɹ���ʧ�ܴ����ú�����bConnectOk: �������ӳɹ���־
void OnConnect(tce::SSession& stSession, const bool bConnectOk){
//	cout << "OnConnect:" << bConnectOk << endl;
	if ( bConnectOk )
	{
		g_bConnectOk = true;
		g_stSvrSession = stSession;
	}
}

//��ʱ�������ú���//iId����Ϊ	tce::CCommMgr::GetInstance().SetTimer(1, 100, true); ��һ���Ĳ���
void OnTimer(const int iId){
	
	switch( iId )
	{
	case 1:	// connect tcp svr or send data to tcp
		{
			if ( !g_bConnectOk )
			{
				tce::CCommMgr::GetInstance().Connect(g_iTcpCommID, TCP_SVR_IP, TCP_SVR_PORT);
			}
			else
			{
				//send data to svr
				string sTest="test";
				tce::CCommMgr::GetInstance().Write(g_stSvrSession, sTest);
			}
		}
		break;
	case 2:// send data to upd svr
		{
			string sTest = "udp test";
			tce::CCommMgr::GetInstance().WriteTo(g_iUdpCommID, UDP_SVR_IP, UDP_SVR_PORT, sTest);
		}
	    break;
	default:
	    break;
	}
}

//�ײ���󴥷�
void OnError(tce::SSession& stSession, const int iErrNo, const char* pszMsg){	cout << "OnError" << endl;}



int main(void)
{
	g_oFileLog.Init("./test",10*1024);


	//����UDP SVR
	g_iUdpCommID = tce::CCommMgr::GetInstance().CreateSvr(CCommMgr::CT_UDP_SVR, SVR_IP, 30004, 1000000,1000000);
	tce::CCommMgr::GetInstance().SetSvrCallbackFunc(g_iUdpCommID, &OnRead, &OnClose, &OnConnect, &OnError);

	//����TCP SVR
	g_iTcpCommID = tce::CCommMgr::GetInstance().CreateSvr(CCommMgr::CT_TCP_SVR, SVR_IP, 30004, 5000000,5000000);
	tce::CCommMgr::GetInstance().SetSvrDgramType(g_iTcpCommID,CCommMgr::TDT_H2LONGT3);
	tce::CCommMgr::GetInstance().SetSvrCallbackFunc(g_iTcpCommID, &OnRead, &OnClose, &OnConnect, &OnError);


	//�������з������ϵķ���
	tce::CCommMgr::GetInstance().RunAllSvr();

	//���ö�ʱ���ص�����
	tce::CCommMgr::GetInstance().SetTimerCallbackFunc(&OnTimer);

	//����һ����ʱ��
	tce::CCommMgr::GetInstance().SetTimer(1, 1000);
	tce::CCommMgr::GetInstance().SetTimer(2, 1000);


	//����ͨ��ģ��
	tce::CCommMgr::GetInstance().Start();
	return 0;
}

