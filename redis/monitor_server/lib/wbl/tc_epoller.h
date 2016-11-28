
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


#ifndef	__TC_EPOLLER_H_
#define __TC_EPOLLER_H_

#include <sys/epoll.h>
#include <cassert>

namespace taf
{
/////////////////////////////////////////////////
/** 
 * @file  tc_epoller.h 
 * @brief  epoll������װ�� 
 *  
 */
/////////////////////////////////////////////////
 
/**
 * @brief epoller�����࣬�Ѿ�Ĭ�ϲ�����EPOLLET��ʽ������ 
 */
class TC_Epoller
{
public:

	/**
	 * @brief ���캯��. 
	 *  
     * @param bEt Ĭ����ETģʽ����״̬�����仯��ʱ��Ż��֪ͨ
	 */
	TC_Epoller(bool bEt = true);

	/**
     * @brief ��������.
	 */
	~TC_Epoller();

	/**
	 * @brief ����epoll���. 
	 *  
     * @param max_connections epoll������Ҫ֧�ֵ����������
	 */
	void create(int max_connections);

	/**
	 * @brief ��Ӽ������. 
	 *  
     * @param fd    ���
     * @param data  ����������, ���Ժ�����epoll_event�л�ȡ��
     * @param event ��Ҫ�������¼�EPOLLIN|EPOLLOUT
     *              
	 */
	void add(int fd, long long data, __uint32_t event);

	/**
	 * @brief �޸ľ���¼�. 
	 *  
     * @param fd    ���
     * @param data  ����������, ���Ժ�����epoll_event�л�ȡ��
     * @param event ��Ҫ�������¼�EPOLLIN|EPOLLOUT
	 */
	void mod(int fd, long long data, __uint32_t event);

	/**
	 * @brief ɾ������¼�. 
	 *  
     * @param fd    ���
     * @param data  ����������, ���Ժ�����epoll_event�л�ȡ��
     * @param event ��Ҫ�������¼�EPOLLIN|EPOLLOUT
	 */
	void del(int fd, long long data, __uint32_t event);

	/**
	 * @brief �ȴ�ʱ��. 
	 *  
	 * @param millsecond ���� 
     * @return int       ���¼������ľ����
	 */
	int wait(int millsecond);

	/**
     * @brief ��ȡ���������¼�.
	 *
	 * @return struct epoll_event&���������¼�
	 */
	struct epoll_event& get(int i) { assert(_pevs != 0); return _pevs[i]; }

protected:

	/**
	 * @brief ����epoll����EPOLL��Ϊ��Ե����EPOLLETģʽ 
     * @param fd    �������create����ʱ����ֵ
     * @param data  ����������, ���Ժ�����epoll_event�л�ȡ��
     * @param event ��Ҫ�������¼�
	 * @param op    EPOLL_CTL_ADD�� ע���µ�fd��epfd�У� 
	 * 			    EPOLL_CTL_MOD���޸��Ѿ�ע���fd�ļ����¼��� 
	 * 			    EPOLL_CTL_DEL����epfd��ɾ��һ��fd�� 
	 *  
	 */
	void ctrl(int fd, long long data, __uint32_t events, int op);

protected:

    /**
     * 	epoll
     */
    int _iEpollfd;

	/**
     * ���������
	 */
	int	_max_connections;

	/**
     * �¼���
	 */
	struct epoll_event *_pevs;

    /**
     * �Ƿ���ETģʽ
     */
    bool _et;
};

}
#endif

