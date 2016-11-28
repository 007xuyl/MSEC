
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



#ifndef __CONFIGINI_H__
#define __CONFIGINI_H__

#define CONF_RET_OK               0 // ��ȡ���óɹ�
#define CONF_RET_FILE_NONEXIST   -1 // �����ļ�������
#define CONF_RET_NO_CONFIG       -2 // �����ò�����
#define CONF_RET_LOAD_ERR        -3 // ���ü���ʧ��

/**
 * @brief ��ȡINI��ʽ����
 * @info  [USER]        -- session USER
 *        user=bison    -- key:user val:bison
 * @param config_path   [IN]   �����ļ�·���������ò�����ʹ��ϵͳ�������ļ�
 *        session       [IN]   session
 *        key           [IN]   key
 *        val           [OUT]  ֵ
 * @return  CONF_RET_OK              �ɹ�
 *          CONF_RET_FILE_NONEXIST   �����ļ�������
 *          CONF_RET_NO_CONFIG       �����ò�����
 *          CONF_RET_LOAD_ERR        ���ü���ʧ��
 */
int GetConfig(const string &config_path, const string &session, const string &key, string &val);
int GetConfig(const string &session, const string &key, string &val);


#endif

