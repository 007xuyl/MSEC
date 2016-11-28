
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
 *  @filename mt_sys_call.cpp
 *  @info  ΢�̷߳�װϵͳapi, ͬ������΢�߳�API��ʵ���첽����
 */

#include "mt_api.h"
#include "mt_capi.h"

using namespace NS_MICRO_THREAD;


/**
 * @brief ΢�߳�����sleep�ӿ�, ��λms
 * @info  ҵ����Ҫ�����ó�CPUʱʹ��
 */
void mtc_sleep(int ms)
{
    mt_sleep(ms);
}

/**
 * @brief ΢�̻߳�ȡϵͳʱ�䣬��λms
 */
unsigned long long mtc_time_ms(void)
{
    return mt_time_ms();
}

/**
 * @brief ���õ�ǰ΢�̵߳�˽�б���
 * @info  ֻ����ָ�룬�ڴ���Ҫҵ�����
 */
void mtc_set_private(void *data)
{
    mt_set_msg_private(data);
}

/**
 * @brief  ��ȡ��ǰ΢�̵߳�˽�б���
 * @return ˽�б���ָ��
 */
void* mtc_get_private(void)
{
    return mt_get_msg_private();
}

/**
 * @brief  ΢�߳̿�ܳ�ʼ��
 * @info   ҵ��ʹ��spp������΢�̣߳���Ҫ���øú�����ʼ����ܣ�
 *         ʹ��spp��ֱ�ӵ���SyncFrame�Ŀ�ܳ�ʼ����������
 * @return <0 ��ʼ��ʧ��  0 ��ʼ���ɹ�
 */
int mtc_init_frame(void)
{
    if (mt_init_frame())
    {
        return 0;
    }

    return -1;
}

/**
 * @brief ����΢�̶߳���ջ�ռ��С
 * @info  �Ǳ������ã�Ĭ�ϴ�СΪ128K
 */
void mtc_set_stack_size(unsigned int bytes)
{
    mt_set_stack_size(bytes);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� recvfrom
 * @param fd ϵͳsocket��Ϣ
 * @param buf ������Ϣ������ָ��
 * @param len ������Ϣ����������
 * @param from ��Դ��ַ��ָ��
 * @param fromlen ��Դ��ַ�Ľṹ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ճ���, <0 ʧ��
 */
int mtc_recvfrom(int fd, void *buf, int len, int flags, struct sockaddr *from, socklen_t *fromlen, int timeout)
{
    return mtc_recvfrom(fd, buf, len, flags, from, fromlen, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� sendto
 * @param fd ϵͳsocket��Ϣ
 * @param msg �����͵���Ϣָ��
 * @param len �����͵���Ϣ����
 * @param to Ŀ�ĵ�ַ��ָ��
 * @param tolen Ŀ�ĵ�ַ�Ľṹ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
int mtc_sendto(int fd, const void *msg, int len, int flags, const struct sockaddr *to, int tolen, int timeout)
{
    return mt_sendto(fd, msg, len, flags, to, tolen, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� connect
 * @param fd ϵͳsocket��Ϣ
 * @param addr ָ��server��Ŀ�ĵ�ַ
 * @param addrlen ��ַ�ĳ���
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
int mtc_connect(int fd, const struct sockaddr *addr, int addrlen, int timeout)
{
    return mt_connect(fd, addr, addrlen, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� accept
 * @param fd �����׽���
 * @param addr �ͻ��˵�ַ
 * @param addrlen ��ַ�ĳ���
 * @param timeout ��ȴ�ʱ��, ����
 * @return >=0 accept��socket������, <0 ʧ��
 */
int mtc_accept(int fd, struct sockaddr *addr, socklen_t *addrlen, int timeout)
{
    return mt_accept(fd, addr, addrlen, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� read
 * @param fd ϵͳsocket��Ϣ
 * @param buf ������Ϣ������ָ��
 * @param nbyte ������Ϣ����������
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ճ���, <0 ʧ��
 */
ssize_t mtc_read(int fd, void *buf, size_t nbyte, int timeout)
{
    return mt_read(fd, buf, nbyte, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� write
 * @param fd ϵͳsocket��Ϣ
 * @param buf �����͵���Ϣָ��
 * @param nbyte �����͵���Ϣ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
ssize_t mtc_write(int fd, const void *buf, size_t nbyte, int timeout)
{
    return mt_write(fd, buf, nbyte, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� recv
 * @param fd ϵͳsocket��Ϣ
 * @param buf ������Ϣ������ָ��
 * @param len ������Ϣ����������
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ճ���, <0 ʧ��
 */
ssize_t mtc_recv(int fd, void *buf, int len, int flags, int timeout)
{
    return mt_recv(fd, buf, len, flags, timeout);
}

/**
 * @brief ΢�̰߳�����ϵͳIO���� send
 * @param fd ϵͳsocket��Ϣ
 * @param buf �����͵���Ϣָ��
 * @param nbyte �����͵���Ϣ����
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 �ɹ����ͳ���, <0 ʧ��
 */
ssize_t mtc_send(int fd, const void *buf, size_t nbyte, int flags, int timeout)
{
    return mt_send(fd, buf, nbyte, flags, timeout);
}

/**
 * @brief ΢�̵߳ȴ�epoll�¼��İ�������
 * @param fd ϵͳsocket��Ϣ
 * @param events �ȴ����¼� IN/OUT
 * @param timeout ��ȴ�ʱ��, ����
 * @return >0 ������¼�, <0 ʧ��
 */
int mtc_wait_events(int fd, int events, int timeout)
{
    return mt_wait_events(fd, events, timeout);
}


/**
 * @brief ����΢�߳�
 * @param entry   ��ں���ָ�룬���ͼ�ThreadStart
 * @param args    ��ں�������
 * @return   0 �����ɹ�  <0 ����ʧ��
 */
int mtc_start_thread(void* entry, void* args)
{
    if (mt_start_thread(entry, args)) {
        return 0;
    }

    return -1;
}

/**
 * @brief ��TCP���ӳ��л�ȡ���ӣ����û�У����´���
 */
void *mtc_get_keep_conn(struct sockaddr_in* dst, int *sock)
{
    return mt_get_keep_conn(dst, sock);
}

/**
 * @brief  �ͷ�TCP���ӵ����ӳ�
 * @param  conn   mt_get_keep_conn���ص����Ӷ���ָ��
 * @param  force  1  -> ǿ���ͷ����ӣ�����������ӳ�
 * @param         0  -> �ͷŵ����ӳأ����ӳ����ˣ��ر�����
 */
void mtc_free_keep_conn(void *conn, int force)
{
    mt_free_keep_conn(conn, (bool)force);
}



