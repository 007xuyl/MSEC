
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


#ifndef _TC_CLIENTSOCKET_H__
#define _TC_CLIENTSOCKET_H__

#include "tc_socket.h"

namespace taf
{
/*************************************TC_ClientSocket**************************************/

/**
* @brief �ͻ���socket��ز�������
*/
class TC_ClientSocket
{
public:

    /**
    *  @brief ���캯��
	 */
	TC_ClientSocket() : _port(0),_timeout(3000) {}

    /**
     * @brief ��������
     */
	virtual ~TC_ClientSocket(){}

    /**
    * @brief ���캯��
    * @param sIP      ������IP
	* @param iPort    �˿�, portΪ0ʱ:��ʾ�����׽��ִ�ʱipΪ�ļ�·��
    * @param iTimeout ��ʱʱ��, ����
	*/
	TC_ClientSocket(const string &sIp, int iPort, int iTimeout) { init(sIp, iPort, iTimeout); }

    /**
    * @brief ��ʼ������
    * @param sIP      ������IP
	* @param iPort    �˿�, portΪ0ʱ:��ʾ�����׽��ִ�ʱipΪ�ļ�·��
    * @param iTimeout ��ʱʱ��, ����
	*/
	void init(const string &sIp, int iPort, int iTimeout)
    {
        _socket.close();
        _ip         = sIp;
        _port       = iPort;
        _timeout    = iTimeout;
    }

    /**
    * @brief ���͵�������
    * @param sSendBuffer ����buffer
    * @param iSendLen    ����buffer�ĳ���
    * @return            int 0 �ɹ�,<0 ʧ��
    */
    virtual int send(const char *sSendBuffer, size_t iSendLen) = 0;

    /**
    * @brief �ӷ��������ز�����iRecvLen���ֽ�
    * @param sRecvBuffer ����buffer
	* @param iRecvLen    ָ�����ն��ٸ��ַ��ŷ���,����������ݵĳ���
    * @return            int 0 �ɹ�,<0 ʧ��
    */
    virtual int recv(char *sRecvBuffer, size_t &iRecvLen) = 0;

    /**
    * @brief  ���巢�͵Ĵ���
    */
    enum
    {
        EM_SUCCESS  = 0,      	/** EM_SUCCESS:���ͳɹ�*/
		EM_SEND     = -1,		/** EM_SEND:���ʹ���*/
		EM_SELECT   = -2,	    /** EM_SELECT:select ����*/
		EM_TIMEOUT  = -3,		/** EM_TIMEOUT:select��ʱ*/
		EM_RECV     = -4,		/** EM_RECV: ���ܴ���*/
		EM_CLOSE    = -5,		/**EM_CLOSE: �����������ر�*/
		EM_CONNECT  = -6,		/** EM_CONNECT : ����������ʧ��*/
		EM_SOCKET   = -7		/**EM_SOCKET : SOCKET��ʼ��ʧ��*/
    };

protected:
    /**
     * �׽��־��
     */
	TC_Socket 	_socket;

    /**
     * ip���ļ�·��
     */
	string		_ip;

    /**
     * �˿ڻ�-1:��ʾ�Ǳ����׽���
     */
	int     	_port;

    /**
     * ��ʱʱ��, ����
     */
	int			_timeout;
};

/**
 * @brief TCP�ͻ���Socket
 * ���߳�ʹ�õ�ʱ�򣬲��ö��߳�ͬʱsend/recv��С�Ĵ�����
 */
class TC_TCPClient : public TC_ClientSocket
{
public:
    /**
    * @brief  ���캯��
	 */
	TC_TCPClient(){}

    /**
    * @brief  ���캯��
    * @param sIp       ������Ip
    * @param iPort     �˿�
    * @param iTimeout  ��ʱʱ��, ����
	*/
	TC_TCPClient(const string &sIp, int iPort, int iTimeout) : TC_ClientSocket(sIp, iPort, iTimeout)
    {
    }

    /**
    * @brief  ���͵�������
    * @param sSendBuffer  ����buffer
    * @param iSendLen     ����buffer�ĳ���
    * @return             int 0 �ɹ�,<0 ʧ��
    */
    int send(const char *sSendBuffer, size_t iSendLen);

    /**
    * @brief  �ӷ��������ز�����iRecvLen���ֽ�
    * @param sRecvBuffer ����buffer
	* @param iRecvLen    ָ�����ն��ٸ��ַ��ŷ���,����������ݵĳ���
    * @return            int 0 �ɹ�,<0 ʧ��
    */
    int recv(char *sRecvBuffer, size_t &iRecvLen);

    /**
	*  @brief �ӷ�����ֱ��������(ע������Ƿ��������صĽ�����,
	*         �������м�ķ��� ) ֻ����ͬ������
    * @param sRecvBuffer ����buffer, �����ָ���
    * @param sSep        �ָ���
    * @return            int 0 �ɹ�,<0 ʧ��
    */
    int recvBySep(string &sRecvBuffer, const string &sSep);

    /**
     * @brief ���յ��������ر�����Ϊֹ
     * @param recvBuffer
     *
     * @return int 0 �ɹ�,<0 ʧ��
     */
    int recvAll(string &sRecvBuffer);

    /**
     * @brief  �ӷ���������iRecvLen���ֽ�
     * @param sRecvBuffer, sRecvBuffer��buffer���ȱ�����ڵ���iRecvLen
	 * @param iRecvLen
     * @return int 0 �ɹ�,<0 ʧ��
     */
    int recvLength(char *sRecvBuffer, size_t iRecvLen);

    /**
    * @brief  ���͵�������, �ӷ��������ز�����iRecvLen���ֽ�
    * @param sSendBuffer  ����buffer
    * @param iSendLen     ����buffer�ĳ���
    * @param sRecvBuffer  ����buffer
	* @param iRecvLen     ����buffer�ĳ���ָ��[in/out],
	*   			      ����ʱ��ʾ����buffer�Ĵ�С,����ʱ��ʾ�����˶��ٸ��ַ�
    * @return             int 0 �ɹ�,<0 ʧ��
    */
	int sendRecv(const char* sSendBuffer, size_t iSendLen, char *sRecvBuffer, size_t &iRecvLen);

    /**
    * @brief  ���͵�������, ���ȴ�������ֱ����β�ַ�, ������β�ַ�
	* sSep�����Ƿ��������صĽ�����,�������м�ķ��ţ�ֻ����ͬ������
	* (һ�ν���һ�����ȵ�buffer,���ĩβ��sSep�򷵻�,
	* ��������ȴ�����)
    *
    * @param sSendBuffer  ����buffer
    * @param iSendLen     ����buffer�ĳ���
    * @param sRecvBuffer  ����buffer
    * @param sSep         ��β�ַ�
    * @return             int 0 �ɹ�,<0 ʧ��
    */
	int sendRecvBySep(const char* sSendBuffer, size_t iSendLen, string &sRecvBuffer, const string &sSep);

    /**
    * @brief  ���͵�������, ���ȴ�������ֱ����β�ַ�(\r\n), ����\r\n
    * ע������Ƿ��������صĽ�����,�������м�ķ���
	* ֻ����ͬ������
    * (һ�ν���һ�����ȵ�buffer,���ĩβ��\r\n�򷵻�,��������ȴ�����)
    *
    * @param sSendBuffer  ����buffer
    * @param iSendLen     ����buffer�ĳ���
    * @param sRecvBuffer  ����buffer
    * @param sSep         ��β�ַ�
    * @return             int 0 �ɹ�,<0 ʧ��
    */
	int sendRecvLine(const char* sSendBuffer, size_t iSendLen, string &sRecvBuffer);

    /**
     * @brief  ���͵�������, ����ֱ���������ر�����Ϊֹ
     * ��ʱ�������ر����Ӳ���Ϊ����
     * @param sSendBuffer
     * @param iSendLen
     * @param sRecvBuffer
     *
     * @return int
     */
    int sendRecvAll(const char* sSendBuffer, size_t iSendLen, string &sRecvBuffer);

protected:
    /**
     * @brief  ��ȡsocket
     *
     * @return int
     */
    int checkSocket();
};

/*************************************TC_TCPClient**************************************/
 /**
  * @brief  ���߳�ʹ�õ�ʱ�򣬲��ö��߳�ͬʱsend/recv��С�Ĵ���
  */
class TC_UDPClient : public TC_ClientSocket
{
public:
    /**
    * @brief  ���캯��
	 */
	TC_UDPClient(){};

    /**
    * @brief  ���캯��
    * @param sIp       ������IP
    * @param iPort     �˿�
    * @param iTimeout  ��ʱʱ��, ����
	*/
	TC_UDPClient(const string &sIp, int iPort, int iTimeout) : TC_ClientSocket(sIp, iPort, iTimeout)
    {
    }

    /**
     * @brief  ��������
     * @param sSendBuffer ����buffer
     * @param iSendLen    ����buffer�ĳ���
     *
     * @return            int 0 �ɹ�,<0 ʧ��
     */
    int send(const char *sSendBuffer, size_t iSendLen);

    /**
     * @brief  ��������
     * @param sRecvBuffer  ����buffer
	 * @param iRecvLen     ����/����ֶ�
     * @return             int 0 �ɹ�,<0 ʧ��
     */
    int recv(char *sRecvBuffer, size_t &iRecvLen);

    /**
     * @brief  ��������, ������Զ�̵Ķ˿ں�ip
     * @param sRecvBuffer ����buffer
     * @param iRecvLen    ����/����ֶ�
     * @param sRemoteIp   ����ֶ�, Զ�̵�ip
     * @param iRemotePort ����ֶ�, Զ�̶˿�
     *
     * @return int 0 �ɹ�,<0 ʧ��
     */
    int recv(char *sRecvBuffer, size_t &iRecvLen, string &sRemoteIp, uint16_t &iRemotePort);

    /**
     * @brief  ���Ͳ���������
     * @param sSendBuffer ����buffer
     * @param iSendLen    ����buffer�ĳ���
     * @param sRecvBuffer ����/����ֶ�
     * @param iRecvLen    ����/����ֶ�
     *
     * @return int 0 �ɹ�,<0 ʧ��
     */
    int sendRecv(const char *sSendBuffer, size_t iSendLen, char *sRecvBuffer, size_t &iRecvLen);

    /**
     * @brief  ���Ͳ���������, ͬʱ��ȡԶ�̵�ip�Ͷ˿�
     * @param sSendBuffer  ����buffer
     * @param iSendLen     ����buffer�ĳ���
     * @param sRecvBuffer  ����/����ֶ�
     * @param iRecvLen     ����/����ֶ�
     * @param sRemoteIp    ����ֶ�, Զ�̵�ip
     * @param iRemotePort  ����ֶ�, Զ�̶˿�
     *
     * @return int 0 �ɹ�,<0 ʧ��
     */
    int sendRecv(const char *sSendBuffer, size_t iSendLen, char *sRecvBuffer, size_t &iRecvLen, string &sRemoteIp, uint16_t &iRemotePort);

protected:
    /**
     * @brief  ��ȡsocket
     *
     * @return TC_Socket&
     */
    int checkSocket();
};

}

#endif
