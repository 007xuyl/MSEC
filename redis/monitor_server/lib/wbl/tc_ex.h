
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


#ifndef __TC_EX_H
#define __TC_EX_H

#include <stdexcept>
using namespace std;

namespace taf
{
/////////////////////////////////////////////////
/** 
* @file  tc_ex.h 
* @brief �쳣�� 
*/           
/////////////////////////////////////////////////

/**
* @brief �쳣��.
*/
class TC_Exception : public exception
{
public:
    /**
	 * @brief ���캯�����ṩ��һ�����Դ���errno�Ĺ��캯���� 
	 *  
	 *  	  �쳣�׳�ʱֱ�ӻ�ȡ�Ĵ�����Ϣ
	 *  
	 * @param buffer �쳣�ĸ澯��Ϣ 
     */
	explicit TC_Exception(const string &buffer);

    /**
	 * @brief ���캯��,�ṩ��һ�����Դ���errno�Ĺ��캯���� 
	 *  
	 *  	  �쳣�׳�ʱֱ�ӻ�ȡ�Ĵ�����Ϣ
	 *  
     * @param buffer �쳣�ĸ澯��Ϣ 
     * @param err    ������, ����strerror��ȡ������Ϣ
     */
	TC_Exception(const string &buffer, int err);

    /**
     * @brief ��������
     */
    virtual ~TC_Exception() throw();

    /**
     * @brief ������Ϣ.
     *
     * @return const char*
     */
    virtual const char* what() const throw();

    /**
     * @brief ��ȡ������
     * 
     * @return �ɹ���ȡ����0
     */
    int getErrCode() { return _code; }

private:
    void getBacktrace();

private:
    /**
	 * �쳣�������Ϣ
     */
    string  _buffer;

	/**
	 * ������
     */
    int     _code;

};

}
#endif

