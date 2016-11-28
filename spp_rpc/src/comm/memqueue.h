
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


#ifndef __MEMQUEUE_H__
#define __MEMQUEUE_H__

#include <stdint.h>

typedef struct _tag_mem_queue_header
{
    volatile int32_t    version;            /* �汾��       */
    volatile int32_t    free_list;          /* ��������     */
    volatile int32_t    block_num;          /* ����         */
    volatile int32_t    unused;             /* ���ÿ���     */
    volatile int32_t    block_size;         /* ���С       */
    volatile int32_t    block_data_size;    /* �����ݴ�С   */
    volatile int32_t    write_times;        /* д����       */
    volatile int32_t    read_times;         /* ������       */
    volatile int32_t    msg_cnt;            /* ��ǰ��Ϣ��   */
    volatile int32_t    nomem;              /* û�п����   */
}mem_queue_header_t;

typedef struct _tag_mem_block_queue
{
    volatile int32_t    read_pos;           /* ���ж�ָ�� */
    volatile int32_t    write_pos;          /* ����дָ�� */
    volatile int32_t    size;               /* ���д�С   */
    volatile int32_t    reserved[13];
    volatile int32_t    queue[0];           /* ��������   */
}mem_block_queue_t;

typedef struct _tag_mem_block
{
    volatile int32_t    next;               /* ����ָ�� */
    volatile int32_t    index;              /* ���±�   */
    volatile int32_t    total_len;          /* �����������ܴ�С */
    volatile int32_t    data_len;           /* ��ǰ�����ݴ�С   */
    volatile int32_t    reserved[4];
    char                data[0];            /* ������ʼδ֪     */
}mem_block_t;

typedef struct _tag_mem_queue_desc
{
    mem_queue_header_t *header;
    mem_block_queue_t * queue;
    mem_block_t *       blocks;
}mem_queue_desc_t;

/**
 * @brief   �������г�ʼ��
 * @return  0       �ɹ�
 *          ����    ʧ��
 * @param   desc        ��������������
 *          vaddr       ���������ڴ��ַ
 *          mem_size    �������д�С
 *          data_size   ʹ�������ݴ�С��ͨ����ֵ��ȷ�Ͽ��С
 * @info    ����ͬʱ����̳�ʼ��
 */
int32_t mem_queue_init(mem_queue_desc_t *desc, void *vaddr, int32_t mem_size, int32_t data_size);

/**
 * @brief   ������������нӿں���
 * @return  >=0     ���а�����
 *          <0      ʧ��
 * @param   desc        ��������������
 *          data        ����
 *          len         ���ݳ���
 */
int32_t mem_queue_push(mem_queue_desc_t *desc, char *data, int32_t len);

/**
 * @brief   ������������нӿں���
 * @return  >=0     ���а�����
 *          <0      ʧ��
 * @param   desc        ��������������
 *          iov         iovec��ַ
 *          iovlen      iov����
 */
int32_t mem_queue_pushv(mem_queue_desc_t *desc, struct iovec *iov, int32_t iovlen);

/**
 * @brief   �������г����нӿں���
 * @return  0       û������
 *          >0      �����а�����
 *          <0      ʧ��
 * @param   desc        ��������������
 *          data        ����
 * @info    data��Ҫ������free
 */
int32_t mem_queue_pop(mem_queue_desc_t *desc, char **data);

/**
 * @brief   �������г����нӿں���
 * @return  0       û������
 *          >0      �����а�����
 *          <0      ʧ��
 * @param   desc        ��������������
 *          buff        ����
 *          len         buff����
 */
int32_t mem_queue_pop_nm(mem_queue_desc_t *desc, char *buff, int32_t len);

#endif

