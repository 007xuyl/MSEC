
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

//���������¼�����OnRead����
int OnRead(tce::SSession& stSession, const unsigned char* pszData, const size_t iSize){

	//http ����
	tce::CHttpParser oParser;
	if ( oParser.Decode((char*)pszData, iSize) )
	{
		cout << "URI:" << oParser.GetURI() << endl;

		if ( strcmp(oParser.GetURI(), "/cgi-bin/test") == 0 )
		{
			if ( oParser.GetIfModifiedSinceTime() + 60 < time(NULL) )	//oParser.GetIfModifiedSinceTime() ��ȡIEҳ���޸�ʱ��
			{
				tce::CHttpResponse oResp;
				oResp.Begin();
				oResp.SetStatusCode(304);
				oResp.End();
				tce::CCommMgr::GetInstance().Write(stSession, oResp.GetData(), oResp.GetDataLen());
				return 0;
			}

			//response
			tce::CHttpResponse oResp;
			oResp.Begin();
			oResp.SetLastModified(time(NULL));			//����ҳ���޸�ʱ��
			oResp.SetCacheControl("max-age=10");	//����ҳ�治���ʷ�����ʱ�䣨��λ�룩
			oResp << "test123456";
			oResp.End();
			tce::CCommMgr::GetInstance().Write(stSession, oResp.GetData(), oResp.GetDataLen());
		} 
		else if ( strcmp(oParser.GetURI(), "/cgi-bin/test1") == 0 )
		{
			//response
			tce::CHttpResponse oResp;
			oResp.Begin();
			oResp << "haha";
			oResp.End();
			tce::CCommMgr::GetInstance().Write(stSession, oResp.GetData(), oResp.GetDataLen());
		}
		else
		{
			//response
			tce::CHttpResponse oResp;
			oResp.Begin();
			oResp << "no support request:" << oParser.GetURI();
			oResp.End();
			tce::CCommMgr::GetInstance().Write(stSession, oResp.GetData(), oResp.GetDataLen());
		}
	}
	else
	{
		cout << "error:" << oParser.GetErrMsg() << endl;
	}

	return 0;
}

//tcp���ӹرմ���OnClose����
void OnClose(tce::SSession& stSession){
//	cout << "OnClose" << endl;
}

//�������ӳɹ���ʧ�ܴ����ú�����bConnectOk: �������ӳɹ���־
void OnConnect(tce::SSession& stSession, const bool bConnectOk){
//	cout << "OnConnect:" << bConnectOk << endl;
}

//��ʱ�������ú���//iId����Ϊ	tce::CCommMgr::GetInstance().SetTimer(1, 100, true); ��һ���Ĳ���
void OnTimer(const int iId){

}

//�ײ���󴥷�
void OnError(tce::SSession& stSession, const int iErrNo, const char* pszErrMsg){	cout << "OnError" << endl;}



int main(void)
{
	g_oFileLog.Init("./test",10*1024);

	//����HTTP SVR
	int iCommID = tce::CCommMgr::GetInstance().CreateSvr(CCommMgr::CT_HTTP_SVR, SVR_IP, 30005, 1000000,1000000);
	tce::CCommMgr::GetInstance().SetSvrCallbackFunc(iCommID, &OnRead, &OnClose, &OnConnect, &OnError);


	//�������з������ϵķ���
	tce::CCommMgr::GetInstance().RunAllSvr();

	//���ö�ʱ���ص�����
	tce::CCommMgr::GetInstance().SetTimerCallbackFunc(&OnTimer);

	//����һ����ʱ��
	tce::CCommMgr::GetInstance().SetTimer(1, 100, true);


	//����ͨ��ģ��
	tce::CCommMgr::GetInstance().Start();
	return 0;
}

