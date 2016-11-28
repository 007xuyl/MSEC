
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



#ifndef __WBL_CONFIG_H__
#define __WBL_CONFIG_H__
/*
	���ýӿ���

	Ҫ��֧�ֲ�νṹ,�����\�ָ�,��conf["Main\\ListenPort"]ȡMain·���µ�ListenPort����ֵ
	��Сд����,
	a.name-value������:[]���ʵ�������ֵ,���������,�׳��쳣conf_not_find
			GetPairs����ĳ��path�����е�name-value��,������˳��
			��ͬ·���´���ͬ����������,ȡ��һ��

	b.domain������:GetDomains����ĳ��path�����е�domain������,��˳�����

	c.GetSubPath����ĳ��path�����е�subpath��
*/

#include <string>
#include <map>
#include <vector>
#include <stdexcept>

namespace wbl {

struct conf_load_error: public std::runtime_error{ conf_load_error(const std::string& s):std::runtime_error(s){}};
struct conf_not_find: public std::runtime_error{ conf_not_find(const std::string& s):std::runtime_error(s){}};

/**
 * ������ӿ�
 */
class CConfig
{
protected:
	CConfig(const CConfig&){}
public:
	CConfig(){}
	virtual ~CConfig(){}

public:
	/**
	 * ��������
	 * @throw conf_load_error when Load fail
	 */
	virtual void Load()=0;

	/**
	 * ȡ����,Ҫ��֧�ֲ�νṹ,��'\'����,��conf["Main\\ListenPort"]
	 * @throw conf_not_find when Load fail
	 */
	virtual const std::string& operator [](const std::string& name) const = 0;

	/**
	 * ȡ����,Ҫ��֧�ֲ�νṹ,��'\'����,��conf["Main\\ListenPort"]
	 * @return ������ò�����,����""
	 */
	virtual const std::string& getvalue(const std::string& name,const std::string& defaultvalue="") const = 0;

	/**
	 * ȡpath������name-value��
	 */
	virtual const std::map<std::string,std::string>& GetPairs(const std::string& path) const = 0;
	/**
	 * ȡpath������name-value��single����
	 */
	virtual const std::vector<std::string>& GetDomains(const std::string& path) const = 0;
	/**
	 * ȡpath������subpath��(ֻȡһ��)
	 */
	virtual const std::vector<std::string>& GetSubPath(const std::string& path) const = 0;
};

}
#endif //


