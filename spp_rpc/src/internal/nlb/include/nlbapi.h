
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


#ifndef _NLBAPI_H_
#define _NLBAPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    NLB_PORT_TYPE_UDP  = 1,
    NLB_PORT_TYPE_TCP  = 2,
    NLB_PORT_TYPE_ALL  = 3,
}NLB_PORT_TYPE;

/* ����·����Ϣ */
struct routeid
{
    uint32_t ip;        // IPV4��ַ : �����ֽ���
    uint16_t port;      // �˿�     : �����ֽ���
    NLB_PORT_TYPE type; // �˿�����
};

/* API������ */
enum {
    NLB_ERR_INVALID_PARA    = -1,  // ������Ч
    NLB_ERR_NO_ROUTE        = -2,  // ����·��ʧ��
    NLB_ERR_NO_AGENT        = -3,  // û�а�װAGENT
    NLB_ERR_NO_ROUTEDATA    = -4,  // û�и�ҵ���·����Ϣ
    NLB_ERR_NO_STATISTICS   = -5,  // û�и�ҵ���·��ͳ����Ϣ
    NLB_ERR_NO_HISTORY      = -6,  // û����ʷ·����Ϣ
    NLB_ERR_LOCK_CONFLICT   = -7,  // ����ͻ
    NLB_ERR_INVALID_MAGIC   = -8,  // ROUTEID magic��Ч
    NLB_ERR_NO_SERVER       = -9,  // û�����������
    NLB_ERR_CREATE_SOCKET_FAIL = -10, // ������AGENTͨ�ŵ�socketʧ��
    NLB_ERR_SEND_FAIL          = -11, // ����·������ʧ��
    NLB_ERR_RECV_FAIL          = -12, // ����·������ʧ��
    NLB_ERR_INVALID_RSP        = -13, // ·������ظ�������Ч
    NLB_ERR_AGENT_ERR          = -14, // Agent�ظ�·������ʧ��
};

/**
 * @brief ͨ��ҵ������ȡ·����Ϣ
 * @para  name:  ���������ҵ�����ַ���  "Login.ptlogin"
 * @      route: ���������·����Ϣ(ip��ַ���˿ڣ��˿�����)
 * @return  0: �ɹ�  others: ʧ��
 */
int32_t getroutebyname(const char *name, struct routeid *route);

/**
 * @brief ����·��ͳ������
 * @info  ÿ���շ���������Ҫ���ɹ����ʱ�����ݸ��µ�ͳ������
 * @para  name:  ���������ҵ�����ַ���  "Login.ptlogin"
 *        ip:    ���������IPV4��ַ
 *        failed:���������>1:ʧ�ܴ��� 0->�ɹ�
 *        cost:  ���������ʱ��
 */
int32_t updateroute(const char *name, uint32_t ip, int32_t failed, int32_t cost);

#ifdef __cplusplus
}
#endif


#endif

