
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


/**
 *  @file SyncFrame.h
 *  @info ͬ���߳��߼���ܴ���
 *  @time 20130515
 **/

#ifndef __NEW_SYNC_FRAME_EX_H__
#define __NEW_SYNC_FRAME_EX_H__
#include <serverbase.h>

namespace SPP_SYNCFRAME {

class CSyncMsg;
using namespace spp::comm;

#define SF_LOG(lvl, fmt, args...)                                           \
do {                                                                        \
    register CServerBase *base = CSyncFrame::Instance()->GetServerBase();   \
    if ((base) && (lvl >= base->log_.log_level(-1))) {                      \
        base->log_.LOG_P_ALL(lvl, fmt"\n", ##args);                         \
    }                                                                       \
} while (0)


/**
 * @brief ΢�߳�ͬ����ܶ���
 */
class CSyncFrame
{
    public:
        static bool _init_flag;        ///< ��ʼ��״̬���
        static int  _iNtfySock;        ///< socket���͵�֪ͨ���         
        
		unsigned int _uMsgNo;           ///��Ϣʵ����

    public:
        static CSyncFrame* Instance (void);
        static void Destroy (void);

        CSyncFrame();
        ~CSyncFrame();

        /**
         * ��ʼ���첽��ܣ��ɲ����worker���̵�spp_handle_init�����
         *
         * @param pServBase CServerBase����ָ��
         * @param max_thread_num ��󲢷��߳���, �ɿ��Ƽ�������ڴ�ռ��
         * @param min_thread_num ��С�����߳���, �ɿ����̳߳صĿ����߳���Ŀ
         *
         * @return 0: �ɹ��� ������ʧ��
         *
         */
        int InitFrame(CServerBase *pServBase, int max_thread_num = 50000, int min_thread_num = 200);

        /**
         * ���������ɲ����spp_handle_process���������
         *
         * @param pMsg CMsgBase���������ָ�룬��ź�������ص����ݣ��ö�����Ҫ�����new�ķ�ʽ���䣬�ͷ��ɿ�ܸ���
         *
         * @return 0: �ɹ��� ������ʧ��
         *
         */ 
        int Process(CSyncMsg *pMsg);

        /**
         * ��ȡCServerBase����ָ�룬�ö����ṩ��־��ͳ�ƹ���
         * �궨��FRAME_LOG��Ϊ�˼���־��¼�ġ�
         *
         * @return CServerBase����ָ��
         **/
        CServerBase* GetServerBase()     {
            return _pServBase;
        }

        
        /**
         *  ���߳�ѭ��, ÿ���л�������, �ó�CPU, ���ȿ�ܺ��epoll
         */
        void WaitNotifyFd(int utime);

        /**
         *  ΢�߳�������־, SPPע���ڲ��ӿ�
         */
        bool GetMtFlag();

        /**
         *  ���߳�ѭ��, ����ǰ���Ƿ�������, ����epoll�ȴ�����
         */
        void HandleSwitch(bool block);

        /**
         *  ��ȡ��ǰ���������
         */
        CSyncMsg* GetCurrentMsg();
		
        /**
         *  ��鵱ǰջ�Ƿ�ȫ
         */
		bool CheckCurrentStack(long lESP);

		/**
         *	��ȡ��Ϣ��
         */
         int GetThreadNum();

        /**
         * ������ID
         */
        void SetGroupId(int id);

		/**
         *	���ߣ��л���
         */
		void sleep(int ms);

    protected:
        static CSyncFrame *_s_instance;     ///< ȫ�ֵ������
        CServerBase *_pServBase;            ///< SPP���������    
        int _iGroupId;                      ///< SPP WORKER��id
        int _iNtfyFd;                       ///< �����ڴ�ӳ��Ĺܵ����

};


}

#endif
