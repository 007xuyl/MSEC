
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
 *  @filename mt_monitor.h
 */
 
#ifndef __MT_MONITOR_H__
#define __MT_MONITOR_H__

#define MONITOR_MT_EPOLL_FD_ERR     "frm.mt fd error"               // ΢�߳�fd�Ƿ�
#define MONITOR_MT_EPOLL_ERR        "frm.mt epoll error"            // epoll����
#define MONITOR_MT_POLL_EMPTY       "frm.mt pool empty"             // ΢�̳߳ؿ���
#define MONITOR_MT_OVERLOAD         "frm.mt overload"               // ΢�̸߳����ﵽ����
#define MONITOR_MT_POLL_INIT_FAIL   "frm.mt pool init failed"       // ΢�̳߳س�ʼ��ʧ��
#define MONITOR_MT_HEAP_ERROR       "frm.mt timer heap error"       // ΢�̶߳�ʱ���Ѵ���
#define MONITOR_MT_POOL_SIZE        "frm.mt pool size"              // ΢�̳߳ش�С
#define MONITOR_MT_SOCKET_ERR       "frm.mt create socket failed"   // 320842
#define MONITOR_MT_SEND_ERR         "frm.mt send failed"            // ����ʧ��
#define MONITOR_MT_RECV_FAIL        "frm.mt recv failed"            // ����ʧ��
#define MONITOR_MT_CONNECT_FAIL     "frm.mt connect failed"         // ����ʧ��
#define MONITOR_MT_SESSION_EXPIRE   "frm.mt session expired"        // udp session��ʱ

#endif

 
