#include "utils.h"
#include "protocol.h"
#include "net.h"
#include "addrman.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

using namespace std;

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

    cout << "Using config file " << GetConfigFile().string() << endl;
}

bool AddOneNode(const string & strNode, bool fConnectToMasternode)
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

void SetParams()
{
    if(GetBoolArg("-testnet", false))
        SelectParams(CBaseChainParams::TESTNET);
    else
        SelectParams(CBaseChainParams::MAIN);
}
