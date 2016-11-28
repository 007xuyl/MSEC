
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



#ifndef __WBL_FILE_CONFIG_H__
#define __WBL_FILE_CONFIG_H__

#include "wbl_config.h"

#include <deque>

namespace wbl {

/**
 * ������: CConfig��Fileʵ�ְ汾<br>
 * ������\n��ʾ�س���
 */
class CFileConfig:public CConfig
{
	/**
	 * no implementation
	 */
	CFileConfig(const CFileConfig&);

	CFileConfig& operator=(const CFileConfig&);
public:
	CFileConfig();
	virtual ~CFileConfig();

public:
	/**
	 * ��ʼ��,���������ļ���&Load
	 * @throw conf_load_error when Load fail
	 * @param filename �����ļ���
	 */
	void Init(const std::string& filename) throw(conf_load_error);

	/**
	 * ��������
	 * @throw conf_load_error when Load fail
	 */
	virtual void Load() throw (conf_load_error);

	/**
	 * ȡ����,֧�ֲ�νṹ,��'\'����,��conf["Main\\ListenPort"]
	 * @throw conf_not_find when Load fail
	 */
	virtual const std::string& operator [](const std::string& name) const throw(conf_not_find);
	virtual const std::string& getvalue(const std::string& name,const std::string& defaultvalue="") const;
	/**
	 * ȡpath������name-value��
	 */
	virtual const std::map<std::string,std::string>& GetPairs(const std::string& path) const;
	/**
	 * ȡpath������name-value��single����
	 */
	virtual const std::vector<std::string>& GetDomains(const std::string& path) const;
	/**
	 * ȡpath������subpath��(ֻȡһ��)
	 */
	virtual const std::vector<std::string>& GetSubPath(const std::string& path) const;

protected:
	enum EntryType {
		T_STARTPATH = 0,
		T_STOPPATH = 1,
		T_NULL = 2,
		T_PAIR = 3,
		T_DOMAIN =4,
		T_ERROR = 5
	};

	std::string start_path(const std::string& s);
	std::string stop_path(const std::string& s);
	void decode_pair(const std::string& s,std::string& name,std::string& value);
	std::string trim_comment(const std::string& s); //trimע�ͺ�\n����
	std::string path(const std::deque<std::string>& path);
	std::string parent_path(const std::deque<std::string>& path);
	std::string sub_path(const std::deque<std::string>& path);
	std::string replace_esc(const std::string& s,const std::string& esc,const std::string& repl);
	std::string get_includefile(const std::string& s);
	void LoadSubFile(const std::string& subfile)throw (conf_load_error);

	EntryType entry_type(const std::string& s);
protected:
	std::string _filename;

	std::map<std::string,std::map<std::string,std::string> > _pairs;
	std::map<std::string,std::vector<std::string> > _paths;
	std::map<std::string,std::vector<std::string> > _domains;

	std::map<std::string,std::string> _null_map;
	std::vector<std::string> _null_vector;
};

}
#endif //



