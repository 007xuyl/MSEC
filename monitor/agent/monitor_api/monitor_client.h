
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


#ifndef __MONITOR_API_H__
#define __MONITOR_API_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
	
	//��ʼ��API
	//	 FileName: mmapѭ�������ļ�
	int Monitor_Init(const char* FileName);
	
	
	// �ۼ��ϱ�API
	//	ServiceName: ������
	//	AttrName: ������
	//	Value: �ۼ��ϱ����ϱ�ֵ
	int Monitor_Add(const char* ServiceName, const char* AttrName, uint32_t Value);
	
	// �����ϱ�API��ÿ����ֻȡ��һ�θ����ϱ�������
	//	ServiceName: ������
	//	AttrName: ������
	//	Value: �����ϱ����ϱ�ֵ
	// 	�ر�ע��:ҵ�����ͬһ���ϱ����������б�ֻ֤����һ���ϱ���ʽ��
	//                             �л�Add/Set�ϱ�ͬһ�����Կ��ܻᵼ��ͳ���쳣
	int Monitor_Set(const char* ServiceName, const char* AttrName, uint32_t Value);
#ifdef __cplusplus
}
#endif

#endif
