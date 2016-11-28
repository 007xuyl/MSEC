
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


/******************************************************************************

 FILENAME:	LinkTable.h

  DESCRIPTION:	����ʽ�ڴ�ṹ�ӿ�����

  �������ȡ�����ݴ洢��̬���鲿�֣��ڴ�����������֯���ϲ����

 ******************************************************************************/

#ifndef _LINK_TABLE_H_
#define _LINK_TABLE_H_

#include <stdint.h>
#include <string>

using namespace std;

// һ��key�������Ŀ����������ڴ�����
#define MAX_BLOCK_COUNT 10000

// Ԥ���ճ����ռ��256K
#define LT_MAX_RECYCLE_POOL_SIZE   (256UL*1024/8)
#define LT_DEF_RECYCLE_POOL_SIZE   100
// Ԥ�������ݽṹ��չ�Ŀռ�
#define LT_MAX_RESERVE_LEN      (256UL*1024)
#define LT_MAX_USRDATA_LEN      ((512UL*1024)-4)

// �ռ䲻���ʱ���Ƿ���Ҫ����Ԥ���ճ��е�Ԫ��������
//#define EMPTY_RECYCLE_ON_OUT_OF_SPACE

typedef unsigned long long ull64_t;

typedef struct
{
    ull64_t ddwNext : 48;
    ull64_t ddwLengthInBlock : 16; // data length in the block, the block is empty if this is 0
    uint8_t bufData[0];
}LtBlock;


typedef struct
{
    ull64_t ddwAllBlockCount;
    ull64_t ddwFreeBlockCount;
    ull64_t ddwBlockSize;
    ull64_t ddwLastEmpty; // ˳�����ʱ�������Ϊ�յ�����
    ull64_t ddwFirstFreePos; // All free blocks are linked together by ddwNext
    ull64_t ddwRecycleIndex; // Current index to addwRecyclePool
    uint32_t dwRecyclePoolSize; // Ԥ���ճصĴ�С�������ã�������LT_MAX_RECYCLE_POOL_SIZE
    ull64_t addwRecyclePool[LT_MAX_RECYCLE_POOL_SIZE]; // All blocks to be deleted are put here for delayed deletion
    uint8_t  sReserved2[LT_MAX_RESERVE_LEN];  // Reserved for future extension
    uint32_t dwUserDataLen;
    uint8_t  sUserData[LT_MAX_USRDATA_LEN];  // Reserved for application use
}LinkTableHead;


class LinkTable
{
private:
	volatile LinkTableHead *header;
	void *blocks;

	char errmsg[1024];

public:
	LinkTable():header(NULL), blocks(NULL) { errmsg[0] = 0; }
	~LinkTable() {}

	ull64_t EvalBufferSize(ull64_t uBlockCount, ull64_t uBlockSize) { return sizeof(LinkTableHead) + (uBlockCount * uBlockSize); }
	ull64_t EvalBlockCount(ull64_t uBuffSize, ull64_t uBlockSize) { return (uBuffSize - sizeof(LinkTableHead)) / uBlockSize; }

	//��ʼ����ʽ���û������Լ�����LinkTableHead��Block�����buffer
	//���Ե���EvalBufferSize���ݿ��С�Ϳ�������������Ҫ��buffer��С
	int Init(void *pBuffer, ull64_t ddwBufferSize, ull64_t ddwBlockCount, ull64_t ddwBlockSize);
	int InitExisting(void *pBuffer, ull64_t ddwBufferSize, ull64_t ddwBlockCount, ull64_t ddwBlockSize);

	//����ddwPosָ����û�����
	int GetData(ull64_t ddwPos, void *sDataBuf, int &iDataLen);

	//�������ݣ����ddwPosִ�е�������Ч��ɾ����������
	//ִ�гɹ�ʱ��ddwPos �����²��������λ��
	//  ���� -100 ��ʾ�ռ䲻��
	int SetData(ull64_t &ddwPos, const void *sDataBuf, int iDataLen);

	//���ĳ����ddwPos��ʼ������
	int EraseData(ull64_t ddwPos);

	// �����û��ı�Ԥ���ճصĴ�С��sz������1��LT_MAX_RECYCLE_POOL_SIZE֮��
	int SetRecyclePoolSize(int sz);

	// ����ͷ��Ԥ�����û��ռ�
	int GetHeaderData(void *pBuff, int iSize);
	int SetHeaderData(const void *pData, int iLen);

	//����
	const char *GetErrorMsg() { return errmsg; }
	int PrintInfo(const string &prefix = "");

	// ����0~100����ʾ��ǰʹ�ðٷֱ�
	int GetUsage() { return header? (100 - ((header->ddwFreeBlockCount * 100) / header->ddwAllBlockCount)) : 0; }

private:
	int FreeBlock(ull64_t ddwStartPos); // ���������ŵ�free������
	int RecycleBlock(ull64_t ddwStartPos); // ������ճ�
	void EmptyRecyclePool(); // ��ջ��ճأ����ճ����������Ԫ��
	int GetEmptyBlock(int iCount, ull64_t &ddwStartPos); // ��ȡiCount���սڵ㣬��������ʼλ��
};

#endif
