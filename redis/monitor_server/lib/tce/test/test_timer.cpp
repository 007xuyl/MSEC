
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

//��ʱ�������ú���//iId����Ϊ	tce::CCommMgr::GetInstance().SetTimer(1, 100, true); ��һ���Ĳ���
void OnTimer(const int iId){
//	timeval tvCur;
//	gettimeofday(&tvCur, 0);
//	uint64_t ui64CurTime = tvCur.tv_sec*1000+tvCur.tv_usec/1000;

	switch( iId )
	{
	case 1:
		{
			cout << "timer id=" << iId << "; time=" << tce::GetTickCount() << endl;
		}
		break;
	case 2:
		{
			cout << "timer id=" << iId << "; time=" << tce::GetTickCount() << endl;
		}
	    break;
	default:
	    break;
	}
}


int main(void)
{
	//���ö�ʱ���ص�����
	tce::CCommMgr::GetInstance().SetTimerCallbackFunc(&OnTimer);

	//����һ����ʱ��
	tce::CCommMgr::GetInstance().SetTimer(1, 100);
	tce::CCommMgr::GetInstance().SetTimer(2, 1000);


	//����ͨ��ģ��
	tce::CCommMgr::GetInstance().Start();
	return 0;
}

