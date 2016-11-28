
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


#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <map>
#include "inifile.h"
#include "tlog.h"
#include "config.h"
#include "configini.h"

using namespace inifile;
using namespace std;

// �����������
// str = eth1:123/udp eth0:234/tcp
int ParseListens(const string &str, std::vector<Listen> &listens)
{
    size_t begin = 0;
    size_t pos;
    size_t pos1;
    std::string ss_listen;
    std::string ss_intf;
    std::string ss_port;
    std::string port;
    std::string type;
    Listen listen;

    while ((pos = str.find(' ', begin)))
    {
        // ����ӿڵ�ַ
        ss_listen = str.substr(begin, pos-begin);
        pos1 = ss_listen.find(':', 0);
        if (pos1 != string::npos)
        {
            listen.intf = ss_listen.substr(0, pos1);
            ss_port = ss_listen.substr(pos1+1);
        }
        else
        {
            listen.intf = "all";
            ss_port = ss_listen;
        }

        // ����˿ڵ�ַ
        pos1 = ss_port.find('/');
        port = ss_port.substr(0, pos1);
        type = ss_port.substr(pos1+1, string::npos);
        if ((type != "udp") && (type != "tcp"))
        {
            printf("\nInvalid listen port type\n");
            return -1;
        }

        listen.type = type;
        listen.port = atoi(port.c_str());

        if (!listen.port)
        {
            printf("\nInvalid listen port\n");
            return -2;
        }

        listens.push_back(listen);

        if (pos == string::npos)
        {
            break;
        }

        begin = pos+1;
    }

    return 0;
}

// ������־����
int32_t LoadLogConf(IniFile &ini, Log &log)
{
    int ret;
    string str;

    if (!ini.hasSection(LOG_GROUP))
    {
        return 0;
    }

    // ��־�ȼ�
    if (ini.hasKey(LOG_GROUP, "Level"))
    {
        ret = ini.getValue(LOG_GROUP, "Level", str);
        if (ret != RET_OK)
        {
            printf("\nLoad LOG Level failed\n");
            return -1;
        }

        if (!strncasecmp(str.c_str(), "DEBUG", 6))
        {
            log.level = tbase::tlog::LOG_DEBUG;
        }
        else if (!strncasecmp(str.c_str(), "TRACE", 6))
        {
            log.level = tbase::tlog::LOG_TRACE;
        }
        else if (!strncasecmp(str.c_str(), "INFO", 5))
        {
            log.level = tbase::tlog::LOG_INFO;
        }
        else if (!strncasecmp(str.c_str(), "ERROR", 6))
        {
            log.level = tbase::tlog::LOG_ERROR;
        }
        else if (!strncasecmp(str.c_str(), "FATAL", 6))
        {
            log.level = tbase::tlog::LOG_FATAL;
        }
        else
        {
            printf("\nInvalid LOG Level\n");
            return -2;
        }
    }

    // ��־�ļ�����
    if (ini.hasKey(LOG_GROUP, "FileMax"))
    {
        ret = ini.getValue(LOG_GROUP, "FileMax", str);
        if (ret != RET_OK)
        {
            printf("\nLoad LOG FileMax failed\n");
            return -3;
        }

        log.maxfilenum = atoi(str.c_str());
    }

    // ��־�ļ���С
    if (ini.hasKey(LOG_GROUP, "FileSize"))
    {
        ret = ini.getValue(LOG_GROUP, "FileSize", str);
        if (ret != RET_OK)
        {
            printf("\nLoad LOG FileSize failed\n");
            return -4;
        }

        log.maxfilesize = atoi(str.c_str());
    }

    return 0;
}

// ��ȡҵ����
// "/msec/s1/s2/bin", ����"s1.s2"
int GetServiceName(string &name)
{
    char *name_pos;
    char *pos;
    char path[256];

    getcwd(path, sizeof(path));
    if (strncmp(path, "/msec/", 6))
    {
        return -1;
    }

    name_pos = path + 6;
    pos = index(name_pos, '/');
    if (NULL == pos)
    {
        return -2;
    }

    *pos = '.';

    pos = index(pos, '/');
    if (NULL == pos)
    {
        return -3;
    }

    *pos++ = '\0';

    if (strncmp(pos, "bin", 4))
    {
        return -4;
    }

    name = name_pos;

    return 0;
}


/**
 * @brief ��ȡCPU����
 */
int GetCpuNums(void)
{
    static int32_t cpu_num = 0;

    if (cpu_num)
    {
        return cpu_num;
    }

    FILE* fp = fopen("/proc/stat", "r");
    if (!fp) {
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line)-1, fp))
    {
        // cpu[0-9]
        if (strstr(line, "cpu"))
        {
            if (line[3] <= '9' && line[3] >= '0')
            {
                ++cpu_num;
                continue;
            }
        }
        else // ֻ��ע��ǰ�漸�д�cpu����Ϣ
        {
            break;
        }
    }

    if (!cpu_num)
    {
        fclose(fp);
        return 0;
    }

    fclose(fp);

    return cpu_num;
}

// ����ļ��Ƿ����
bool FileExist(const char* path)
{
    if (!path) {
        return false;
    }
    
    int result = access(path, F_OK);
    if (result < 0) {
        return false;
    } else {
        return true;
    } 
}

string ConfigLoader::filename_ = "";

// ����Ĭ������
void ConfigLoader::SetDefaultListen(void)
{
    Listen listen;
    listen.intf = "all";
    listen.port = 7963;
    listen.type = "udp";
    config_.listens.push_back(listen);
    listen.type = "tcp";
    config_.listens.push_back(listen);
}

// ����ҵ��
void ConfigLoader::SetDefaultService(void)
{
    int32_t ret;
    string service_name;

    ret = GetServiceName(service_name);
    if (ret)
    {
        config_.service = "default.default";
    }
    else
    {
        config_.service = service_name;
    }
}

// ��־��������ʼ��
int ConfigLoader::Init(const char *path)
{
    int32_t ret;
    IniFile ini;
    string str;

    if (NULL == path || path[0] == '\0')
    {
        printf("\nInvalid config path\n");
        return -21;
    }

    // ����ļ��Ƿ����
    if (!FileExist(path))
    {
        SetDefaultListen();
        SetDefaultService();
        return 0;
    }

    ConfigLoader::filename_.assign(path);

    // ����ini�����ļ�
    ret = ini.load(path);
    if (ret != RET_OK)
    {
        printf("\nLoad ini config (%s) failed\n", path);
        return -1;
    }

    // �Ƿ����spp����
    if (!ini.hasSection(SPP_GROUP))
    {
        SetDefaultListen();
        SetDefaultService();
        LoadLogConf(ini, config_.log);
        return 0;
    }

    // ��ȡ��������
    if (ini.hasKey(SPP_GROUP, "listen"))
    {
        ret = ini.getValue(SPP_GROUP, "listen", str);
        if (ret != RET_OK)
        {
            printf("\nLoad listens config (%s) failed\n", path);
            return -2;
        }

        ret = ParseListens(str, config_.listens);
        if (ret != RET_OK)
        {
            return -3;
        }
    }
    else
    {
        SetDefaultListen();
    }

    // ��ȡҵ��so�������
    if (ini.hasKey(SPP_GROUP, "module"))
    {
        ret = ini.getValue(SPP_GROUP, "module", config_.module);
        if (ret != RET_OK || config_.module[0] == '\0')
        {
            printf("\nLoad module config (%s) failed\n", path);
            return -4;
        }

        // ������Ǿ���·������Ҫ����ǰ׺
        if (config_.module[0] != '/')
        {
            config_.module = "./" + config_.module;
        }
    }

    #if 0
    if (ini.hasKey(SPP_GROUP, "global"))
    {
        ret = ini.getValue(SPP_GROUP, "global", str);
        if (ret != RET_OK)
        {
            printf("\nLoad module global flag (%s) failed\n", path);
            return -5;
        }

        config_.global = atoi(str.c_str());
    }
    #endif

    // ��ȡ�����ڴ�����
    if (ini.hasKey(SPP_GROUP, "shmsize"))
    {
        ret = ini.getValue(SPP_GROUP, "shmsize", str);
        if (ret != RET_OK)
        {
            printf("\nLoad shmsize (%s) failed\n", path);
            return -6;
        }

        config_.shmsize = atoi(str.c_str());
    }

    // ��ȡҵ�������ļ�·��
    if (ini.hasKey(SPP_GROUP, "conf"))
    {
        ret = ini.getValue(SPP_GROUP, "conf", str);
        if (ret != RET_OK)
        {
            printf("\nLoad shmsize (%s) failed\n", path);
            return -7;
        }

        config_.conf = str;
    }

    // ��ȡ��־����
    ret = LoadLogConf(ini, config_.log);
    if (ret < 0)
    {
        return -8;
    }

    // ��ȡ��������
    if (ini.hasKey(SPP_GROUP, "heartbeat"))
    {
        ret = ini.getValue(SPP_GROUP, "heartbeat", str);
        if (ret != RET_OK)
        {
            printf("\nLoad heartbeat (%s) failed\n", path);
            return -9;
        }

        config_.heartbeat = atoi(str.c_str());
    }

    // ��ȡ����������
    if (ini.hasKey(SPP_GROUP, "reload"))
    {
        ret = ini.getValue(SPP_GROUP, "reload", str);
        if (ret != RET_OK)
        {
            printf("\nLoad reload (%s) failed\n", path);
            return -10;
        }

        config_.reload = atoi(str.c_str());
    }

    // ��ȡ��������
    if (ini.hasKey(SPP_GROUP, "msg_timeout"))
    {
        ret = ini.getValue(SPP_GROUP, "msg_timeout", str);
        if (ret != RET_OK)
        {
            printf("\nLoad msg_timeout (%s) failed\n", path);
            return -11;
        }

        config_.msg_timeout = atoi(str.c_str());
    }

    // ��ȡ����������
    if (ini.hasKey(SPP_GROUP, "procnum"))
    {
        ret = ini.getValue(SPP_GROUP, "procnum", str);
        if (ret != RET_OK)
        {
            printf("\nLoad reload (%s) failed\n", path);
            return -12;
        }

        config_.procnum = atoi(str.c_str());
    }
    else
    {
        // ��ȡ������
        ret = GetCpuNums();
        if (ret > 0)
        {
            config_.procnum = 2*ret;
        }
    }

    // ��ȡҵ����
    ret = GetServiceName(config_.service);
    if (ret < 0)
    {
        config_.service = "default.default";
    }

#if 0
    // ��ȡԤ����nlbҵ������
    if (ini.hasKey(NLB_GROUP, "preload"))
    {
        ret = ini.getValue(NLB_GROUP, "preload", str);
        if (ret != RET_OK)
    }
#endif

    return 0;
}

int ConfigLoader::Reload(const char *filename)
{
    return Init(filename);
}

Config& ConfigLoader::GetConfig(void)
{
    return config_;
}

int GetConfig(const string &config_path, const string &session, const string &key, string &val)
{
    int ret;
    IniFile ini;

    // ����ļ��Ƿ����
    if (!FileExist(config_path.c_str()))
    {
        return CONF_RET_FILE_NONEXIST;
    }

    // ����ini�����ļ�
    ret = ini.load(config_path);
    if (ret != RET_OK)
    {
        return CONF_RET_LOAD_ERR;
    }

    // �Ƿ����spp����
    if (ini.hasKey(session, key))
    {
        return CONF_RET_NO_CONFIG;
    }

    ret = ini.getValue(session, key, val);
    if (ret != RET_OK || val[0] == '\0')
    {
        return CONF_RET_NO_CONFIG;
    }

    return CONF_RET_OK;
}

int GetConfig(const string &session, const string &key, string &val)
{
    return GetConfig(ConfigLoader::filename_, session, key, val);
}

#if 0
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "config.h"

bool CheckWorkerConf(WorkerConf& worker)
{
    if ((worker.id <= 0) || (worker.procnum <= 0)
        || (worker.module.size() == 0) || (worker.interval <= 1))
    {
        return false;
    }

    return true;
}

int ParseUnixAddress(const std::string& address, std::string& path, bool& abstract)
{
    size_t pos = address.rfind('-');
    if (pos == std::string::npos) {
        std::cout << "address: " << address << " - not found" << std::endl;
        path = address;
        abstract = true;
        return 0;
    }

    // yaml��������abstract
    path = address.substr(0, pos);
    if (address.substr(pos + 1) == "abstract")
        abstract = false;
    else
        abstract = true;

    return 0;
}

int ParseIPAddress(const std::string& address, std::string& type, std::string& intf, int& port, int& oob)
{
    size_t pos = address.rfind(':');
    if (pos == std::string::npos) {
        std::cout << "address: " << address << " : not found" << std::endl;
        return -1;
    }

    intf  = address.substr(0, pos);

    size_t slash_pos = address.find('/');
    if (slash_pos == std::string::npos) {
        std::cout << "address: " << address << " / not found" << std::endl;
        return -1;
    }

    port = atoi(address.substr(pos + 1, slash_pos - pos - 1).c_str());

    size_t shortPos = address.find('-');
    if (shortPos == std::string::npos) {
        oob = 0;
        type = address.substr(slash_pos + 1);
    } else {
        type = address.substr(slash_pos + 1, shortPos - slash_pos - 1);
        oob = 0;
        if(address.substr(shortPos + 1) == "oob")
            oob = 1;
    }

    return 0;
}

int ParseListenAddress(const std::string &address, Listen &listen)
{
    int ret;

    if(address[0] == '/') { // unix path 
        std::string path;
        bool abstract;

        ParseUnixAddress(address, path, abstract);

        listen.abstract = abstract;
        listen.type     = "unix";
        listen.path     = path;
    } else {
        std::string type;
        std::string intf;
        int port;
        int oob;
        ret = ParseIPAddress(address, type, intf, port, oob);
        if(ret < 0)
        {
            return -1;
        }

        listen.type = type;
        listen.intf = intf;
        listen.port = port;
        listen.oob  = oob;
    }

    return 0;
}

void operator >> (const YAML::Node& node, Listen& listen)
{
    std::string address;
    const YAML::Node *pConf = node.FindValue("address");
    if(NULL != pConf) {
        *pConf >> address;
    }

    ParseListenAddress(address, listen);
}

void operator >> (const YAML::Node& node, Log& log)
{
    const YAML::Node *pConf = NULL;
    pConf = node.FindValue("level");
    if(NULL != pConf) {
        *pConf >> log.level;
    }

    pConf = node.FindValue("type");
    if(NULL != pConf) {
        *pConf >> log.type;
    }

    pConf = node.FindValue("maxfilesize");
    if(NULL != pConf) {
        *pConf >> log.maxfilesize;
    }

    pConf = node.FindValue("maxfilenum");
    if(NULL != pConf) {
        *pConf >> log.maxfilenum;
    }
}

void operator >> (const YAML::Node& node, Global& global)
{
    const YAML::Node *pConf = node.FindValue("listen");
    if (NULL == pConf) {
        throw "no listen address!";
    }

    const YAML::Node& listensNode = node["listen"];
    if (listensNode.size() == 0) {
        throw "no listen address!";
    }

    for (unsigned i = 0; i < listensNode.size(); i++) {
        Listen listen;
        listensNode[i] >> listen;
        global.listens.push_back(listen);
    }

    pConf = node.FindValue("service");
    if (NULL == pConf) {
        throw "no service name config!";
    }

    *pConf >> global.service;

    pConf = node.FindValue("timeout");
    if (NULL != pConf) {
        *pConf >> global.timeout;
    }

    pConf = node.FindValue("udpclose");
    if (NULL != pConf) {
        *pConf >> global.udpclose;
    }

    pConf = node.FindValue("TOS");
    if (NULL != pConf) {
        *pConf >> global.TOS;
    }

}

void operator >> (const YAML::Node& node, CtrlConf& ctrl)
{
    const YAML::Node *pConf = node.FindValue("flog");
    if (NULL != pConf) {
        node["flog"] >> ctrl.flog;
    }
}

void operator >> (const YAML::Node& node, ProxyConf& proxy)
{
    const YAML::Node *pConf = node.FindValue("flog");
    if (NULL != pConf) {
        node["flog"] >> proxy.flog;
    }

    pConf = node.FindValue("log");
    if (NULL != pConf) {
        node["log"] >> proxy.log;
    }

    pConf = node.FindValue("interval");
    if (NULL != pConf) {
        node["interval"] >> proxy.interval;
    }

    pConf = node.FindValue("global");
    if (NULL != pConf) {
        node["global"] >> proxy.global;
    }

    pConf = node.FindValue("sendcache_limit");
    if (NULL != pConf) {
        node["sendcache_limit"] >> proxy.sendcache_limit;
    }

    pConf = node.FindValue("module");
    if (NULL == pConf) {
        throw "no proxy module config!";
    }

    node["module"] >> proxy.module;

    pConf = node.FindValue("conf");
    if (NULL != pConf) {
        node["conf"] >> proxy.conf;
    }

    pConf = node.FindValue("result");
    if (NULL != pConf) {
        node["result"] >> proxy.result;
    }

    pConf = node.FindValue("type");
    if (NULL != pConf) {
        node["type"] >> proxy.type;
    }

    pConf = node.FindValue("maxconn");
    if (NULL != pConf) {
        node["maxconn"] >> proxy.maxconn;
    }

    pConf = node.FindValue("maxpkg");
    if (NULL != pConf) {
        node["maxpkg"] >> proxy.maxpkg;
    }
}

void operator >> (const YAML::Node& node, WorkerConf& worker)
{
    const YAML::Node *pConf = node.FindValue("id");
    if (NULL == pConf) {
        return;
    }

    *pConf >> worker.id;

    pConf = node.FindValue("module");
    if (NULL == pConf) {
        return;
    }

    *pConf >> worker.module;

    pConf = node.FindValue("procnum");
    if (NULL == pConf) {
        return;
    }

    *pConf >> worker.procnum;

    
    pConf = node.FindValue("global");
    if (NULL != pConf) {
        node["global"] >> worker.global;
    }

    pConf = node.FindValue("interval");
    if (NULL != pConf) {
        node["interval"] >> worker.interval;
    }

    pConf = node.FindValue("reload");
    if(NULL != pConf) {
        node["reload"] >> worker.reload;
    }

    pConf = node.FindValue("conf");
    if(NULL != pConf) {
        *pConf >> worker.conf;
    }

    pConf = node.FindValue("flog");
    if(NULL != pConf) {
        *pConf >> worker.flog;
    }

    pConf = node.FindValue("log");
    if(NULL != pConf) {
        *pConf >> worker.log;
    }

    pConf = node.FindValue("shmsize");
    if(NULL != pConf) {
        *pConf >> worker.shmsize;
    }

    pConf = node.FindValue("timeout");
    if(NULL != pConf) {
        *pConf >> worker.timeout;
    }
}

void operator >> (const YAML::Node& node, WorkersConf& workers)
{
    for(unsigned i = 0; i < node.size(); i++) {
        WorkerConf worker;
        node[i] >> worker;

        if (!CheckWorkerConf(worker) || (workers.find(worker.id) != workers.end())) {
            throw "worker config error!";
        }

        workers.insert(WorkersConf::value_type(worker.id, worker));
    }

    if (workers.size() == 0) {
        throw "no worker config!";
    }
}

void operator >> (const YAML::Node& node, Config& config)
{
    const YAML::Node *pConf = node.FindValue("global");
    if (NULL == pConf) {
        throw "no global config!";
    }

    node["global"] >> config.global;

    pConf = node.FindValue("controller");
    if (NULL != pConf) {
        node["controller"] >> config.ctrl;
    }

    pConf = node.FindValue("proxy");
    if (NULL == pConf) {
        throw "no proxy config!";
    }

    node["proxy"] >> config.proxy;

    pConf = node.FindValue("worker");
    if (NULL == pConf) {
        throw "no worker config!";
    }

    node["worker"] >> config.workers;
}


int ConfigLoader::Init(const char *filename)
{
    try{
        filename_ = filename;

        std::ifstream fin(filename, std::ifstream::in);
        if(!fin.is_open()) {
            std::cout << "[ERROR]" << filename_ << " not exist." << std::endl;
            return -1;
        }

        YAML::Parser parser(fin);
        YAML::Node   doc;
        parser.GetNextDocument(doc);
        doc >> config_;

        fin.close();

    } catch(YAML::ParserException & e) {
        std::cerr << "[ERROR]" << filename << " parse exception:" << e.what() << std::endl;
        return -2;
    } catch (const char * err) {
        std::cerr << "[ERROR]" << "configuration file(" << filename << ")format error:" << err << std::endl;
        return -3;
    }

    return 0;
}

int ConfigLoader::Reload()
{
    return 0;
}

#endif

