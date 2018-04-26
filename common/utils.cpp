#include "utils.h"
#include "protocol.h"
#include "net.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std;

//map<string, string> mapToolArgs;
//map<string, vector<string> > mapMultiToolArgs;

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

    boost::filesystem::path = GetCoinToolDir() / filename
    mapArgs["-conf"] = path.string();

    printf("Using config file %s\n", GetConfigFile().string());
}
#if 0
boost::filesystem::path GetFile(const std::string & filename)
{
    boost::filesystem::path pathConfigFile(filename);
    if (!pathConfigFile.is_complete())
        pathConfigFile = GetCoinToolDir() / pathConfigFile;

    return pathConfigFile;
}

/** Interpret string as boolean, for argument parsing */
static bool InterpretBool(const std::string& strValue)
{
    if (strValue.empty())
        return true;
    return (atoi(strValue) != 0);
}
/** Turn -noX into -X=0 */
static void InterpretNegativeSetting(std::string& strKey, std::string& strValue)
{
    if (strKey.length()>3 && strKey[0]=='-' && strKey[1]=='n' && strKey[2]=='o')
    {
        strKey = "-" + strKey.substr(3);
        strValue = InterpretBool(strValue) ? "0" : "1";
    }
}

void ReadFile(const std::string & strfile)
{
    boost::filesystem::ifstream streamFile(GetFile(strfile));
    if (!streamFile.good()){
        // Create empty ulord.conf if it does not excist
        FILE* configFile = fopen(GetFile(strfile).c_str(), "a");
        if (configFile != NULL)
            fclose(configFile);
        return; // Nothing to read, so just return
    }

    set<string> setOptions;
    setOptions.insert("*");

    for (boost::program_options::detail::config_file_iterator it(streamFile, setOptions), end; it != end; ++it)
    {
        // Don't overwrite existing settings so command line settings override ulord.conf
        string strKey = string("-") + it->string_key;
        string strValue = it->value[0];
        InterpretNegativeSetting(strKey, strValue);
        if (mapArgs.count(strKey) == 0)
            mapArgs[strKey] = strValue;
        mapMultiArgs[strKey].push_back(strValue);
    }
}
#endif
bool AddOneNode(const string & strNode)
{
	CAddress addr;
	return OpenNetworkConnection(addr, NULL, strNode.c_str());
}

bool GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet)
{
    CBitcoinSecret vchSecret;

    if(!vchSecret.SetString(strSecret)) return false;

    keyRet = vchSecret.GetKey();
    pubkeyRet = keyRet.GetPubKey();

    return true;
}

void SetParams()
{
    if(GetBoolArg("-testnet", false))
        SelectParams(CBaseChainParams::TESTNET);
    else
        SelectParams(CBaseChainParams::MAIN);
}