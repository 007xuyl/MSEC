
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


#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <signal.h>
#include <string>
#include <vector>
#include <map>
#include "inifile.h"
#include "tlog.h"

using namespace tbase::tlog;
using namespace std;

#define SPP_GROUP "SRPC"
#define NLB_GROUP "NLB"
#define LOG_GROUP "LOG"

// ��־����
struct Log
{
    int level;       // ��־�ȼ�
    int type;        // ��־����
    int maxfilesize; // ��־�ļ���С
    int maxfilenum;  // ��־�ļ�����

    Log():level(tbase::tlog::LOG_ERROR), type(tbase::tlog::LOG_TYPE_CYCLE), maxfilesize(10240000), maxfilenum(10) {}
};

// ������ַ
struct Listen
{
    std::string type;       // ������ַ����  TCP/UDP
    std::string intf;       // �ӿڵ�ַ      eth0/eth1...
    int         port;       // �����˿�
    int         oob;        // �Ƿ��oob

    Listen(): port(-1), oob(0) {}
};

// ����������Ϣ
struct Config
{
    std::vector<Listen> listens;    // ������ַ
    //std::vector<std::string> nlb_preload;
    std::string         service;    // ҵ����
    std::string         module;     // ҵ��so
    std::string         conf;       // ҵ�������ļ�·��
    Log  log;                       // ��־
    int  timeout;                   // �������ӳ�ʱʱ�� ��λ: ��
    int  msg_timeout;               // ��Ϣ���ر����¼� ��λ: ms
    int  global;                    // dlopen ��־ RTLD_GLOBAL
    int  procnum;                   // ������
    int  shmsize;                   // �����ڴ��С
    int  heartbeat;                 // ����ʱ��
    int  reload;                    // �ȼ��ر��

    Config(): module("./msec.so"), timeout(60), msg_timeout(800), global(1), procnum(4), shmsize(16), heartbeat(60), reload(0) {}
};

// ���ö�ȡ��
class ConfigLoader
{
public:
    ConfigLoader()  {};
    ~ConfigLoader() {};

    void SetDefaultListen(void);
    void SetDefaultService(void);
    int Init(const char *filename);
    int Reload(const char *filename);
    Config &GetConfig(void);

private:
    Config      config_;

public:
    static string filename_;
};


#if 0
// ȫ������
struct Global
{
    std::vector<Listen> listens;  // ������ַ
    std::string         service;  // ҵ����
    int  timeout;    // �������ӳ�ʱʱ�� ��λ: ��
    int  TOS;        // �Ƿ��TOS
    bool udpclose;   // udp�ذ��رձ�־

    Global(): timeout(60), TOS(0), udpclose(true) {}
};

// controller����
struct CtrlConf
{
    Log  flog;       // ��־
};

// proxy����
struct ProxyConf
{
    int interval;       // �ϱ�����ʱ����
    int global;         // dlopen ��־ RTLD_GLOBAL
    int maxconn;        // ���������
    int maxpkg;
    int sendcache_limit;
    Log flog;           // �����־
    Log log;            // ҵ����־

    std::string module; // ҵ��so
    std::string result;
    std::string conf;   // ҵ��˽�������ļ�·��
    std::string type;   // 

    ProxyConf(): interval(20), global(0), maxconn(100000), sendcache_limit(-1) {}
};

// worker����
struct WorkerConf
{
    int id;         // worker��ID
    int procnum;    // ������
    int interval;   // �ϱ�����ʱ����
    int reload;     // ���������
    int global;     // dlopen ��־ RTLD_GLOBAL
    int shmsize;    // �����ڴ��С ��λ:MB
    int timeout;    // ����ʱ�� ��λ:ms
    int TOS;        // ��˽����Ƿ��TOS
    int exitsignal; // �˳��ź�

    Log flog;       // �����־
    Log log;        // ҵ����־

    std::string module;
    std::string result;
    std::string conf;
    std::string type;

    WorkerConf(): id(-1), procnum(0), interval(20), reload(0), global(0), shmsize(16), timeout(800), exitsignal(SIGUSR1) {}
};

typedef std::map<int, WorkerConf> WorkersConf;

struct Config
{
    Global      global;
    CtrlConf    ctrl;
    ProxyConf   proxy;
    WorkersConf workers;
};

class ConfigLoader
{
public:
    ConfigLoader()  {};
    ~ConfigLoader() {};

    int Init(const char *filename);
    int Reload(void);

public:
    std::string filename_;
    Config      config_;
};

#endif


#endif

