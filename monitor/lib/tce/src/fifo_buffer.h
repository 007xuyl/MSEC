
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


#ifndef __TCE_FIFO_BUFFER_H__
#define __TCE_FIFO_BUFFER_H__

#include <string>

#include "tce_singleton.h"
#include "tce_utils.h"
#include <string>

namespace tce{

class CFIFOBuffer :
	private CNonCopyAble
{
	typedef CFIFOBuffer this_type;
	enum {MIN_BUF_LEN=10*1024};

	enum{HEAD_LEN=sizeof(int32_t)};

public:
	enum RETURN_TYPE{
		BUF_OK=0,
		BUF_FULL=1,
		BUF_EMPTY=2,
		BUF_NO_ENOUGH=3,		//�����buffer����
		BUF_ERR=4,
	};


	CFIFOBuffer(void);
	~CFIFOBuffer(void);

	bool Init(const int32_t iBufLen);	//��ʼ��
	const char* GetErrMsg() const {	return m_sErrMsg.c_str();	}

	//ֱ�ӿ��Զ�ȡ
	RETURN_TYPE Read(unsigned char* pszBuf, int32_t& iBufLen);
	RETURN_TYPE Read(std::string& sBuf);

	//////////////////////////////////////////////////////////////////////////
	//���ʹ�ã��죩
	//{//for example
	//	if(ReadNext() == CFIFOBuffer::BUF_OK)
	//	{
	//		int32_t iBufLen = GetCurDataLen();
	//		char* pszData = GetCurData();
	//		MoveNext();
	//	}
	//}
	int32_t GetCurDataLen() const {		return m_iCurDataLen;	}
	const unsigned char* GetCurData() const {	return (m_pszDataBuf+m_iHeadPos);	}
	RETURN_TYPE ReadNext();
	void MoveNext()	{	m_iHeadPos += m_iCurDataLen;	}
	//////////////////////////////////////////////////////////////////////////

	RETURN_TYPE Write(const unsigned char* pszData1, const int32_t iDataSize1, const unsigned char* pszData2, const int32_t iDataSize2);
	inline RETURN_TYPE Write(const char* pszData1, const int32_t iDataSize1, const char* pszData2, const int32_t iDataSize2){
		return Write(reinterpret_cast<const unsigned char*>(pszData1), iDataSize1, reinterpret_cast<const unsigned char*>(pszData2), iDataSize2);
	}

	RETURN_TYPE Write(const unsigned char* pszData1, const int32_t iDataSize1, const unsigned char* pszData2, const int32_t iDataSize2, const unsigned char* pszData3, const int32_t iDataSize3);
	inline RETURN_TYPE Write(const char* pszData1, const int32_t iDataSize1, const char* pszData2, const int32_t iDataSize2, const char* pszData3, const int32_t iDataSize3){
		return Write(reinterpret_cast<const unsigned char*>(pszData1), iDataSize1, reinterpret_cast<const unsigned char*>(pszData2), iDataSize2, reinterpret_cast<const unsigned char*>(pszData3), iDataSize3);
	}

	RETURN_TYPE Write(const unsigned char* pszBuf, const int32_t iBufLen);
	inline RETURN_TYPE Write(const char* pszBuf, const int32_t iBufLen)	{		return Write(reinterpret_cast<const unsigned char*>(pszBuf), iBufLen);	}
	inline RETURN_TYPE Write(const std::string& sBuf)	{	return Write(sBuf.data(), static_cast<int32_t>(sBuf.size()));	}

	size_t GetSize() const {	return (size_t)m_iBufferLen;	}
private:
	void Reset();	//�������
private:
	std::string m_sErrMsg;

	unsigned char* m_pszDataBuf;	//����bufferָ��
	int32_t m_iBufferLen;	//����buffer�ĳ���

	volatile int32_t m_iHeadPos;		//�ѱ������ݵ�ͷ��λ��
	volatile int32_t m_iTailPos;   		//�ѱ������ݵ�β��λ��

	int32_t m_iCurDataLen;		//��ǰ���ݳ���

	int32_t m_iResetCount;				//reset ����
	int32_t m_iReadCount;				//read ����
	int32_t m_iWriteCount;				//write ����
};

};

#endif 
