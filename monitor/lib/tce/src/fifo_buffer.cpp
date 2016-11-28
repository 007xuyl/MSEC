
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


#include "fifo_buffer.h"
#include <cassert>

namespace tce{


CFIFOBuffer::CFIFOBuffer(void)
:m_pszDataBuf(NULL)
,m_iBufferLen(0)
,m_iHeadPos(0)
,m_iTailPos(0)
,m_iCurDataLen(0)
,m_iResetCount(0)
,m_iReadCount(0)
,m_iWriteCount(0)
{
}

CFIFOBuffer::~CFIFOBuffer(void)
{
	delete m_pszDataBuf;
}

void CFIFOBuffer::Reset()
{
	m_iHeadPos = 0;
	m_iTailPos = 0;
	m_iCurDataLen = 0;
	++m_iResetCount;
}

bool CFIFOBuffer::Init(const int32_t iBufLen)
{
	bool bOk = false;
	if ( iBufLen < MIN_BUF_LEN )
	{
		m_iBufferLen = MIN_BUF_LEN;
	}
	else
	{
		m_iBufferLen = iBufLen;
	}

	m_pszDataBuf = new unsigned char[m_iBufferLen];
	if (NULL != m_pszDataBuf)
	{
		bOk = true;
	}
	else
	{
		m_sErrMsg = "memory no enough.";
		bOk = false;
		m_iBufferLen = 0;
	}

	return bOk;
}


CFIFOBuffer::RETURN_TYPE CFIFOBuffer::ReadNext()
{
	CFIFOBuffer::RETURN_TYPE nRe = BUF_OK;

	assert(m_iHeadPos>=0 && m_iHeadPos<=m_iBufferLen);
	assert(m_iTailPos>=0 && m_iTailPos<=m_iBufferLen);

	int32_t iTmpTailPos = m_iTailPos;

	if (m_iHeadPos < iTmpTailPos)
	{
		//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
		if (m_iHeadPos+HEAD_LEN > iTmpTailPos) 
		{
			//reset buffer
			char szErrMsg[1024];
			xsnprintf(szErrMsg, sizeof(szErrMsg),  "data error:(m_iHeadPos<%lu>+4 > dwTailPos<%lu>)(file:%s,line:%d)",m_iHeadPos,iTmpTailPos, __FILE__, __LINE__);
			m_sErrMsg = szErrMsg;
			assert(0);
			this->Reset();
			return BUF_ERR; 
		}

		//��ȡ���ݳ�����Ϣ
		m_iCurDataLen = *(int32_t *)(m_pszDataBuf+m_iHeadPos);

		//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
		if (m_iHeadPos+HEAD_LEN+m_iCurDataLen > iTmpTailPos)
		{
			char szErrMsg[1024];
			xsnprintf(szErrMsg, sizeof(szErrMsg),  "data error:(m_iHeadPos<%lu>+sizeof(SDataHead)<%u>+4 > dwTailPos<%lu>)(file:%s,line:%d).", m_iHeadPos, m_iCurDataLen, iTmpTailPos, __FILE__, __LINE__);
			m_sErrMsg = szErrMsg;
			assert(0);
			this->Reset();
			return BUF_ERR; 
		}

		//��ȡ����������Ϣ
		m_iHeadPos+= HEAD_LEN;
	}
	else if (m_iHeadPos > iTmpTailPos) 
	{
		if (m_iHeadPos+HEAD_LEN <= m_iBufferLen)
		{
			//��ȡ����ͷ��Ϣ
			m_iCurDataLen = *(int32_t *)(m_pszDataBuf+m_iHeadPos);
			if ( m_iCurDataLen + m_iHeadPos + HEAD_LEN > m_iBufferLen )
//			if(EMPTY_DATA == stDataHead.ucDataType)
			{//�ѵ�bufferβ��,�ӿ�ʼ������
				m_iHeadPos = 0;
				//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
				if (m_iHeadPos == iTmpTailPos)
				{
					char szErrMsg[1024];
					xsnprintf(szErrMsg, sizeof(szErrMsg),  "data error:(m_iHeadPos<%d>+4 > dwTailPos<%d>)(file:%s,line:%d)",m_iHeadPos,iTmpTailPos, __FILE__, __LINE__);
					m_sErrMsg = szErrMsg;
					assert(0);
					this->Reset();
					return BUF_ERR;
				}
				else if (m_iHeadPos+HEAD_LEN > iTmpTailPos) 
				{
					//reset buffer
					char szErrMsg[1024];
					xsnprintf(szErrMsg, sizeof(szErrMsg), "data error:(m_iHeadPos<%d>+4 > dwTailPos<%d>)(file:%s,line:%d)",m_iHeadPos,iTmpTailPos, __FILE__, __LINE__);
					m_sErrMsg = szErrMsg;
					assert(0);
					this->Reset();
					return BUF_ERR;
				}

				//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
				if (m_iHeadPos+m_iCurDataLen > iTmpTailPos)
				{
					char szErrMsg[1024];
					xsnprintf(szErrMsg, sizeof(szErrMsg), "data error:(m_iHeadPos<%lu>+DataLen<%u> > dwTailPos<%lu>)(file:%s,line:%d).",m_iHeadPos, m_iCurDataLen, iTmpTailPos, __FILE__, __LINE__);
					m_sErrMsg = szErrMsg;
					assert(0);
					this->Reset();
					return BUF_ERR;
				}
			}
			else
			{
				//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
				if (m_iHeadPos+HEAD_LEN+m_iCurDataLen > m_iBufferLen)
				{
					char szErrMsg[1024];
					xsnprintf(szErrMsg, sizeof(szErrMsg), "data error:<dwTailPos=%d>(m_iHeadPos<%d>+4+wDataLen<%u> > m_iBufferLen<%lu>)(file:%s,line:%d).",iTmpTailPos, m_iHeadPos,m_iCurDataLen,m_iBufferLen, __FILE__, __LINE__);
					m_sErrMsg = szErrMsg;
					assert(0);
					this->Reset();
					return BUF_ERR;
				}		

				//��ȡ����������Ϣ
				m_iHeadPos += HEAD_LEN ;
			}
		}
		else //�ѵ�bufferβ��,�ӿ�ʼ������
		{
			m_iHeadPos = 0;
			//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
			if (m_iHeadPos == iTmpTailPos)
			{
				return BUF_EMPTY;
			}
			else if (m_iHeadPos+HEAD_LEN > iTmpTailPos) 
			{
				//reset buffer
				char szErrMsg[1024];
				xsnprintf(szErrMsg, sizeof(szErrMsg), "data error:(m_iHeadPos<%lu>+4> dwTailPos<%lu>)(file:%s,line:%d)",m_iHeadPos, iTmpTailPos, __FILE__, __LINE__);
				m_sErrMsg = szErrMsg;
				assert(0);
				this->Reset();
				return BUF_ERR;
			}

			//��ȡ����ͷ��Ϣ
			m_iCurDataLen = *(int32_t *)(m_pszDataBuf+m_iHeadPos);

			//�ж����ݳ��ȵĺϷ���, ����Ƿ����������,����
			if (m_iHeadPos+HEAD_LEN+m_iCurDataLen > iTmpTailPos)
			{
				char szErrMsg[1024];
				xsnprintf(szErrMsg, sizeof(szErrMsg), "data error:(m_iHeadPos<%lu>+4+wDataLen<%u> > dwTailPos<%lu>)(file:%s,line:%d).",m_iHeadPos, m_iCurDataLen, iTmpTailPos, __FILE__, __LINE__);
				m_sErrMsg = szErrMsg;
				assert(0);
				this->Reset();
				return BUF_ERR;
			}

			//��ȡ����������Ϣ
			m_iHeadPos += HEAD_LEN;
		}
	}
	else//if (m_iHeadPos == dwTailPos)
	{
		//û������
		nRe = BUF_EMPTY;
	}

	if ( nRe == BUF_OK )
	{
		++m_iReadCount;
	}

	return nRe;
}



CFIFOBuffer::RETURN_TYPE CFIFOBuffer::Read(unsigned char* pszBuf, int32_t& iBufLen)
{
	CFIFOBuffer::RETURN_TYPE nRe = ReadNext();
	if ( CFIFOBuffer::BUF_OK == nRe )
	{
		if ( GetCurDataLen() <= iBufLen )
		{
			memcpy(pszBuf, GetCurData(), GetCurDataLen());
		}
		else
		{
			nRe = CFIFOBuffer::BUF_NO_ENOUGH;
		}
		MoveNext();
	}
	return nRe;
}

CFIFOBuffer::RETURN_TYPE CFIFOBuffer::Read(std::string& sBuf)
{
	CFIFOBuffer::RETURN_TYPE nRe = ReadNext();
	if ( CFIFOBuffer::BUF_OK == nRe )
	{
		sBuf.assign(reinterpret_cast<const char*>(GetCurData()), GetCurDataLen());
		MoveNext();
	}

	return nRe;
}



CFIFOBuffer::RETURN_TYPE CFIFOBuffer::Write(const unsigned char* pszBuf, const int32_t iBufLen)
{

	assert(m_iHeadPos>=0 && m_iHeadPos<=m_iBufferLen);
	assert(m_iTailPos>=0 && m_iTailPos<=m_iBufferLen);

	int32_t iTmpHeadPos = m_iHeadPos;

	CFIFOBuffer::RETURN_TYPE nRe = BUF_OK;

	//��ȡ��������ͷ���������ݵ�λ��
	if (iTmpHeadPos <= m_iTailPos)
	{
		if (m_iTailPos+HEAD_LEN+iBufLen <= m_iBufferLen)
		{
			//��д
			//��������ͷ����������
			memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN, pszBuf, iBufLen);
			m_iTailPos += HEAD_LEN+iBufLen;	
		}
		else
		{
			//�ѵ�buffer��β��,ֻ�ܱ�������ͷ
			if (m_iTailPos+HEAD_LEN <= m_iBufferLen) 
			{
				if (iBufLen < iTmpHeadPos )
				{
					memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
					memcpy(m_pszDataBuf, pszBuf, iBufLen);
					m_iTailPos = iBufLen;	
				}
				else
				{
					nRe = BUF_FULL;
				}
			}
			else
			{
				if (iBufLen+HEAD_LEN < iTmpHeadPos) 
				{
					memcpy(m_pszDataBuf, &iBufLen, HEAD_LEN);
					memcpy(m_pszDataBuf+HEAD_LEN, pszBuf, iBufLen);
					m_iTailPos = HEAD_LEN+iBufLen;	
				}
				else
				{
					nRe = BUF_FULL;
				}
			}
		}
	}
	else
	{
		if (m_iTailPos+HEAD_LEN+iBufLen < iTmpHeadPos)
		{
			//��д
			//��������ͷ����������
			memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN, pszBuf, iBufLen);
			m_iTailPos += HEAD_LEN+iBufLen;	
		}
		else //buffer����
		{
			nRe = BUF_FULL;
		}
	}

	if ( nRe == BUF_OK )
	{
		++m_iWriteCount;
	}


	return nRe;
}

CFIFOBuffer::RETURN_TYPE CFIFOBuffer::Write(const unsigned char* pszData1, const int32_t iDataSize1, const unsigned char* pszData2, const int32_t iDataSize2)
{

	assert(m_iHeadPos>=0 && m_iHeadPos<=m_iBufferLen);
	assert(m_iTailPos>=0 && m_iTailPos<=m_iBufferLen);

	int32_t iTmpHeadPos = m_iHeadPos;

	CFIFOBuffer::RETURN_TYPE nRe = BUF_OK;
	int32_t iBufLen = iDataSize1+iDataSize2;

	//��ȡ��������ͷ���������ݵ�λ��
	if (iTmpHeadPos <= m_iTailPos)
	{
		if (m_iTailPos+HEAD_LEN+iBufLen <= m_iBufferLen)
		{
			//��д
			//��������ͷ����������
			memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN, pszData1, iDataSize1);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN+iDataSize1, pszData2, iDataSize2);
			m_iTailPos += HEAD_LEN+iBufLen;	
		}
		else
		{
			//�ѵ�buffer��β��,ֻ�ܱ�������ͷ
			if (m_iTailPos+HEAD_LEN <= m_iBufferLen) 
			{
				if (iBufLen < iTmpHeadPos )
				{
					memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
					memcpy(m_pszDataBuf, pszData1, iDataSize1);
					memcpy(m_pszDataBuf+iDataSize1, pszData2, iDataSize2);
					m_iTailPos = iBufLen;	
				}
				else
				{
					nRe = BUF_FULL;
				}
			}
			else
			{
				if ( HEAD_LEN+iBufLen < iTmpHeadPos ) 
				{
					memcpy(m_pszDataBuf, &iBufLen, HEAD_LEN);
					memcpy(m_pszDataBuf+HEAD_LEN, pszData1, iDataSize1);
					memcpy(m_pszDataBuf+HEAD_LEN+iDataSize1, pszData2, iDataSize2);
					m_iTailPos = HEAD_LEN+iBufLen;	
				}
				else
				{
					nRe = BUF_FULL;
				}
			}
		}
	}
	else
	{
		if (m_iTailPos+HEAD_LEN+iBufLen < iTmpHeadPos)
		{
			//��д
			//��������ͷ����������
			memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN, pszData1, iDataSize1);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN+iDataSize1, pszData2, iDataSize2);
			m_iTailPos += HEAD_LEN+iBufLen;	
		}
		else //buffer����
		{
			nRe = BUF_FULL;
		}
	}

	if ( nRe == BUF_OK )
	{
		++m_iWriteCount;
	}


	return nRe;
}


CFIFOBuffer::RETURN_TYPE CFIFOBuffer::Write(const unsigned char* pszData1, const int32_t iDataSize1, const unsigned char* pszData2, const int32_t iDataSize2, const unsigned char* pszData3, const int32_t iDataSize3)
{

	assert(m_iHeadPos>=0 && m_iHeadPos<=m_iBufferLen);
	assert(m_iTailPos>=0 && m_iTailPos<=m_iBufferLen);

	int32_t iTmpHeadPos = m_iHeadPos;

	CFIFOBuffer::RETURN_TYPE nRe = BUF_OK;
	int32_t iBufLen = iDataSize1+iDataSize2+iDataSize3;

	//��ȡ��������ͷ���������ݵ�λ��
	if (iTmpHeadPos <= m_iTailPos)
	{
		if (m_iTailPos+HEAD_LEN+iBufLen <= m_iBufferLen)
		{
			//��д
			//��������ͷ����������
			memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN, pszData1, iDataSize1);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN+iDataSize1, pszData2, iDataSize2);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN+iDataSize1+iDataSize2, pszData3, iDataSize3);
			m_iTailPos += HEAD_LEN+iBufLen;	
		}
		else
		{
			//�ѵ�buffer��β��,ֻ�ܱ�������ͷ
			if (m_iTailPos+HEAD_LEN <= m_iBufferLen) 
			{
				if (iBufLen < iTmpHeadPos )
				{
					memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
					memcpy(m_pszDataBuf, pszData1, iDataSize1);
					memcpy(m_pszDataBuf+iDataSize1, pszData2, iDataSize2);
					memcpy(m_pszDataBuf+iDataSize1+iDataSize2, pszData3, iDataSize3);
					m_iTailPos = iBufLen;	
				}
				else
				{
					nRe = BUF_FULL;
				}
			}
			else
			{
				if ( HEAD_LEN+iBufLen < iTmpHeadPos ) 
				{
					memcpy(m_pszDataBuf, &iBufLen, HEAD_LEN);
					memcpy(m_pszDataBuf+HEAD_LEN, pszData1, iDataSize1);
					memcpy(m_pszDataBuf+HEAD_LEN+iDataSize1, pszData2, iDataSize2);
					memcpy(m_pszDataBuf+HEAD_LEN+iDataSize1+iDataSize2, pszData3, iDataSize3);
					m_iTailPos = HEAD_LEN+iBufLen;	
				}
				else
				{
					nRe = BUF_FULL;
				}
			}
		}
	}
	else
	{
		if (m_iTailPos+HEAD_LEN+iBufLen < iTmpHeadPos)
		{
			//��д
			//��������ͷ����������
			memcpy(m_pszDataBuf+m_iTailPos, &iBufLen, HEAD_LEN);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN, pszData1, iDataSize1);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN+iDataSize1, pszData2, iDataSize2);
			memcpy(m_pszDataBuf+m_iTailPos+HEAD_LEN+iDataSize1+iDataSize2, pszData3, iDataSize3);
			m_iTailPos += HEAD_LEN+iBufLen;	
		}
		else //buffer����
		{
			nRe = BUF_FULL;
		}
	}

	if ( nRe == BUF_OK )
	{
		++m_iWriteCount;
	}


	return nRe;
}





};

