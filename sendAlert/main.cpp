#include "utils.h"
#include "alert.h"
#include "net.h"

using namespace std;

int main()
{
    CAlert alert;
	
	CKey prikey;
	CPubKey pubkey;
	string strPrikey = "cRZyST127HW1wyKZUFgr5hykxKR7YJCcVS4nSF17A2N2nwLiLXtN";
	map<string, string> mapAlertArgs;
	map<string, vector<string> > mapMultiAlertArgs;

	ReadFile(mapAlertArgs, mapMultiAlertArgs, "alertarg.conf");

    if(mapAlertArgs.count("-privkey"))
        strPrikey = mapAlertArgs["-privkey"];
    else
        strPrikey = "cRZyST127HW1wyKZUFgr5hykxKR7YJCcVS4nSF17A2N2nwLiLXtN";

    if (mapAlertArgs.count("-version"))
		alert.nVersion = atoi(mapAlertArgs["-version"]);
	else
		alert.nVersion = PROTOCOL_VERSION;

	if (mapAlertArgs.count("-relayuntil"))
		alert.nRelayUntil = atoi64(mapAlertArgs["-relayuntil"]);
	else
		alert.nRelayUntil = 4070908800ULL;
	
	if (mapAlertArgs.count("-expiration"))
		alert.nExpiration = atoi64(mapAlertArgs["-expiration"]);
	else
		alert.nExpiration = 4070908800ULL;

	if (mapAlertArgs.count("-id"))
		alert.nID = atoi(mapAlertArgs["-id"]);
	else
		alert.nID = 1;

	if (mapAlertArgs.count("-cancel"))
		alert.nCancel = atoi(mapAlertArgs["-cancel"]);
	else
		alert.nCancel = 0;
	
	//alert.setCancel.insert(alert.nID);
	if (mapAlertArgs.count("-setcancel"))
	{
		for(string str : mapMultiAlertArgs["-setcancel"])
			alert.setCancel.insert(atoi(str));
	}

	if (mapAlertArgs.count("-minver"))
		alert.nMinVer = atoi(mapAlertArgs["-minver"]);
	else
		alert.nMinVer = 0;

	if (mapAlertArgs.count("-maxver"))
		alert.nMaxVer = atoi(mapAlertArgs["-maxver"]);
	else
		alert.nMaxVer = 0x7fffffff;
	
	//alert.setSubVer.insert(FormatSubVersion(CLIENT_NAME, CLIENT_VERSION, std::vector<std::string>()));
	if (mapAlertArgs.count("-setsubver"))
	{
		for(string str : mapMultiAlertArgs["-setsubver"])
			alert.setSubVer.insert(str);
	}

	if (mapAlertArgs.count("-priority"))
		alert.nPriority = atoi(mapAlertArgs["-priority"]);
	else
		alert.nPriority = 1;

	if (mapAlertArgs.count("-comment"))
		alert.strComment = mapAlertArgs["-comment"];
	else
		alert.strComment = "TEST: alert test";

	if (mapAlertArgs.count("-statusbar"))
		alert.strStatusBar = mapAlertArgs["-statusbar"];
	else
		alert.strStatusBar = "URGENT: Alert key compromised, upgrade required";
    
    if (mapAlertArgs.count("-node"))
	{
		for(string strNode : mapMultiAlertArgs["-node"])
        {
            if(!AddOneNode(strNode))
                cout << "add node failed : " << strNode << endl;
        }
	}
    else
    {
        cout << "add one node by config node=10.175.0.100:9888 frist in config file" << endl;
        return 0;
    }

    // Now serialize the data
    CDataStream sMsg(alert.vchMsg, SER_NETWORK, PROTOCOL_VERSION);
    sMsg << (CUnsignedAlert)alert;

	//for (std::vector<unsigned char>::const_iterator iter = sMsg.begin(); iter != sMsg.end(); iter++)
	for(unsigned int i = 0; i < sMsg.size(); i++)
    {
    	alert.vchMsg.push_back((unsigned char)sMsg[i]);
	}

	if(!GetKeysFromSecret(strPrikey, prikey, pubkey))
	{
        cout << "Error: getKeys failed by privkey = " << strPrikey << endl;
    	return 0;
	}
    cout << "pubkey from prikey " << pubkey.GetID().ToString() << endl;

	/*if(!prikey.SignCompact(Hash(alert.vchMsg.begin(), alert.vchMsg.end()), alert.vchSig))
	{
		obj.push_back(Pair("msg", "SignCompact msg failed!"));
	}
	CPubKey pubkeyFromSig;
    if(!pubkeyFromSig.RecoverCompact(Hash(alert.vchMsg.begin(), alert.vchMsg.end()), alert.vchSig)) {
        obj.push_back(Pair("msg", "Recover pubkey failed!"));
        return obj;
    }
    cout << "pubkey from recover " << pubkeyFromSig.GetID().ToString() << endl;*/

	
	if(!prikey.Sign(Hash(alert.vchMsg.begin(), alert.vchMsg.end()), alert.vchSig))
	{
        cout << "Error: Sign msg failed!" << endl;
    	return 0;
	}
   
    CPubKey paramkey(Params().AlertKey());
    cout << "pubkey from params " << paramkey.GetID().ToString() << endl;
	if(!alert.CheckSignature(Params().AlertKey()))
	{
        cout << "Error: Check signature failed!" << endl;
    	return 0;
	}

    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
    {
    	if(!alert.RelayTo(pnode))
            cout << "Send failed to Node " << pnode->addr.ToString() << endl;
    	else
			cout << "Send success to Node " << pnode->addr.ToString() << endl;
    }

    return 0;
}
