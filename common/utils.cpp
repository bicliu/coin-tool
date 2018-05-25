#include "utils.h"
#include "protocol.h"
#include "net.h"
#include "addrman.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

#include <stdarg.h>
#include <sstream>

using namespace std;

int iDebugRank;
static boost::mutex* mutexDebugLog = NULL;

boost::filesystem::path GetCoinToolDir()
{
    namespace fs = boost::filesystem;
    // Windows < Vista: C:\Documents and Settings\Username\Application Data\CoinTools
    // Windows >= Vista: C:\Users\Username\AppData\Roaming\CoinTools
    // Mac: ~/Library/Application Support/CoinTools
    // Unix: ~/.cointools
#ifdef WIN32
    // Windows
    return GetSpecialFolderPath(CSIDL_APPDATA) / "CoinTools";
#else
    fs::path pathRet;
    char* pszHome = getenv("HOME");
    if (pszHome == NULL || strlen(pszHome) == 0)
        pathRet = fs::path("/");
    else
        pathRet = fs::path(pszHome);
#ifdef MAC_OSX
    // Mac
    return pathRet / "Library/Application Support/CoinTools";
#else
    // Unix
    return pathRet / ".cointools";
#endif
#endif
}

void SetFilePath(const std::string & filename)
{
    mapArgs["-datadir"] = GetCoinToolDir().string();

    boost::filesystem::path pathFile (GetCoinToolDir() / filename);
    mapArgs["-conf"] = pathFile.string();

    cout << "Info: Using config file " << GetConfigFile().string() << endl;
}

static std::string GetTimestampStr(const std::string &str, bool *fStartedNewLine)
{
    string strStamped;

    if (!fLogTimestamps)
        return str;

    if (*fStartedNewLine) {
        int64_t nTimeMicros = GetLogTimeMicros();
        strStamped = DateTimeStrFormat("%Y-%m-%d %H:%M:%S", nTimeMicros/1000000);
        if (fLogTimeMicros)
            strStamped += strprintf(".%06d", nTimeMicros%1000000);
        strStamped += ' ' + str;
    } else
        strStamped = str;

    return strStamped;
}
static std::string GetThreadNameStr(const std::string &str, bool *fStartedNewLine)
{
    string strThreadLogged;

    if (!fLogThreadNames)
        return str;

    std::string strThreadName = GetThreadName();

    if (*fStartedNewLine)
        strThreadLogged = strprintf("%16s | %s", strThreadName.c_str(), str.c_str());
    else
        strThreadLogged = str;

    return strThreadLogged;
}

int PrintStr(const std::string &str)
{
    int ret = 0; // Returns total number of characters written

    ret = fwrite(str.data(), 1, str.size(), stdout);
    fflush(stdout);
    
    return ret;
}

int WriteStr(const std::string &str)
{
    int ret = 0; // Returns total number of characters written
    static bool fStartedNewLine = true;

    std::string strThreadLogged = GetThreadNameStr(str, &fStartedNewLine);
    std::string strTimestamped = GetTimestampStr(strThreadLogged, &fStartedNewLine);

    if (!str.empty() && str[str.size()-1] == '\n')
        fStartedNewLine = true;
    else
        fStartedNewLine = false;

    boost::call_once(&DebugPrintInit, BOOST_ONCE_INIT);
    boost::mutex::scoped_lock scoped_lock(*mutexDebugLog);

    // buffer if we haven't opened the log yet
    if (fileout == NULL) {
        assert(vMsgsBeforeOpenLog);
        ret = strTimestamped.length();
        vMsgsBeforeOpenLog->push_back(strTimestamped);
    }
    else
    {
        // reopen the log file, if requested
        if (fReopenDebugLog) {
            fReopenDebugLog = false;
            boost::filesystem::path pathDebug = GetCoinToolDir() / "debug.log";
            if (freopen(pathDebug.string().c_str(),"a",fileout) != NULL)
                setbuf(fileout, NULL); // unbuffered
        }

        ret = FileWriteStr(strTimestamped, fileout);
    }
    
    return ret;
}

int LogShow(const int debug, const char* format, ...)
{
    char buf[100];
    memset(buf, 0, 100);
    va_list args;
    va_start(args,format);
    vsprintf(buf,format,args);
    va_end(args);

    if(0 == debug)
        return PrintStr(string(buf, buf + strlen(buf)));
    if(iDebugRank == debug)
        return WriteStr(string(buf, buf + strlen(buf)));
    return 0;
}

void showvector(const std::vector<unsigned char>& vch)
{
    int count = 0;
    for(auto c : vch)
    {
        printf("%02x ", (uint8_t)c);
        count++;
        if(count % 8 == 0)
			printf("    ");
		if(count % 16 == 0)
			printf("\n");
    }
    printf("\n");
}

bool showerror(const char* format, ...)
{
    char buf[100];
    memset(buf, 0, 100);
    va_list args;
    va_start(args,format);
    vsprintf(buf,format,args);
    va_end(args);

    PrintStr(std::string("ERROR: ") + string(buf, buf + strlen(buf)) + "\n");
    return false;
}

int showreturn(const char* format, ...)
{
    char buf[100];
    memset(buf, 0, 100);
    va_list args;
    va_start(args,format);
    vsprintf(buf,format,args);
    va_end(args);

    PrintStr(std::string("ERROR: ") + string(buf, buf + strlen(buf)) + "\n");
    return -1;
}

void InitSys()
{
    if(GetBoolArg("-testnet", false))
	{
        SelectParams(CBaseChainParams::TESTNET);
		printf("Info: select TEST net!\n");
	}
    else
	{
        SelectParams(CBaseChainParams::MAIN);
		printf("Info: select MAIN net!\n");
	}
    
    if (mapArgs.count("-debug"))
		iDebugRank = atoi(mapArgs["-debug"]);
	else
		iDebugRank = 0;

    mutexDebugLog = new boost::mutex();
}

/** Interpret string as boolean, for argument parsing */
static bool ToolsInterpretBool(const std::string& strValue)
{
    if (strValue.empty())
        return true;
    return (atoi(strValue) != 0);
}

/** Turn -noX into -X=0 */
static void ToolsInterpretNegativeSetting(std::string& strKey, std::string& strValue)
{
    if (strKey.length()>3 && strKey[0]=='-' && strKey[1]=='n' && strKey[2]=='o')
    {
        strKey = "-" + strKey.substr(3);
        strValue = ToolsInterpretBool(strValue) ? "0" : "1";
    }
}

boost::filesystem::path GetFile(const std::string & filename)
{
    boost::filesystem::path pathFile(filename);
    if (!pathFile.is_complete())
        pathFile = GetCoinToolDir() / pathFile;

    return pathFile;
}

bool ReadFile(std::map<std::string, std::string>& mapSettingsRet,
              std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet,
              const std::string & strfile)
{
    boost::filesystem::ifstream streamFile(GetFile(strfile));
    if (!streamFile.good())
		return showerror("Open file <%s> failed!", GetFile(strfile).string().c_str());

    set<string> setOptions;
    setOptions.insert("*");

    for (boost::program_options::detail::config_file_iterator it(streamFile, setOptions), end; it != end; ++it)
    {
        // Don't overwrite existing settings so command line settings override ulord.conf
        string strKey = string("-") + it->string_key;
        string strValue = it->value[0];
        ToolsInterpretNegativeSetting(strKey, strValue);
        if (mapSettingsRet.count(strKey) == 0)
            mapSettingsRet[strKey] = strValue;
        mapMultiSettingsRet[strKey].push_back(strValue);
    }
	return true;
}

bool ReadCurrentFile(std::map<std::string, std::string>& mapSettingsRet,
              std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet,
              const std::string & strfile)
{
    boost::filesystem::path pathFile(strfile);
    boost::filesystem::ifstream streamFile(pathFile);
    if (!streamFile.good())
		return showerror("Open file <%s> failed!", strfile.c_str());

    set<string> setOptions;
    setOptions.insert("*");

    for (boost::program_options::detail::config_file_iterator it(streamFile, setOptions), end; it != end; ++it)
    {
        // Don't overwrite existing settings so command line settings override ulord.conf
        string strKey = string("-") + it->string_key;
        string strValue = it->value[0];
        ToolsInterpretNegativeSetting(strKey, strValue);
        if (mapSettingsRet.count(strKey) == 0)
            mapSettingsRet[strKey] = strValue;
        mapMultiSettingsRet[strKey].push_back(strValue);
    }
	return true;
}

std::string GetRankString(int index)
{
    switch(index)
    {
        case 1:
            return "1st";
        case 2:
            return "2nd";
        case 3:
            return "3rd";
        default:
        {
            if(index < 1)
                return "";
            else
            {
                ostringstream oss;
                oss << index << "th";
                return oss.str();
            }
        }
    }
}

bool AddOneNode(const std::string & strNode, bool fConnectToMasternode)
{
	//CAddress addr;
	//return OpenNetworkConnection(addr, NULL, strNode.c_str());
    CService destaddr = CService(strNode);
    bool proxyConnectionFailed = false;
    SOCKET hSocket;
    if(ConnectSocket(destaddr, hSocket, DEFAULT_CONNECT_TIMEOUT, &proxyConnectionFailed))
    {
        if (!IsSelectableSocket(hSocket)) {
            cout << "Cannot create connection: non-selectable socket created (fd >= FD_SETSIZE ?)" << endl;
            CloseSocket(hSocket);
            return false;
        }
        addrman.Attempt(destaddr);

        // Add node
        CNode* pnode = new CNode(hSocket, (CAddress)destaddr, "", false, true);

        pnode->nTimeConnected = GetTime();
		pnode->nVersion = PROTOCOL_VERSION;
        if(fConnectToMasternode) {
            pnode->AddRef();
            pnode->fMasternode = true;
        }

        LOCK(cs_vNodes);
        vNodes.push_back(pnode);
        return true;
    }
    cout << "ConnectSocket failed!" << endl;
    return false;
}

void FreeNode()
{
    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
    {
        pnode->fDisconnect = true;
        CloseSocket(pnode->hSocket);
        //vNodes->erase(pnode);
        delete pnode;
    }
    vNodes.clear();
}

bool GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet)
{
    CBitcoinSecret vchSecret;

    if(!vchSecret.SetString(strSecret)) return false;

    keyRet = vchSecret.GetKey();
    pubkeyRet = keyRet.GetPubKey();

    return true;
}

bool atob(const std::string & str)
{
    if("true" == str)
        return true;
    return false;
}

