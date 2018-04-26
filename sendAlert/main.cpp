#include "utils.h"
#include "alert.h"
#include "net.h"

using namespace std;

int main()
{
    CAlert alert;
	
	CKey prikey;
	CPubKey pubkey;
	string strPrikey = "cPu3SSXjZMzhnqq6mVpNmskdLAFwdcQW3JHgqU6M2s1beTAQWNdW";
	
	ReadFile("alertarg.conf");

    if(mapToolArgs.count("-privkey"))
        strPrikey = mapToolArgs["-privkey"];
    else
        strPrikey = "cPu3SSXjZMzhnqq6mVpNmskdLAFwdcQW3JHgqU6M2s1beTAQWNdW";

    if (mapToolArgs.count("-version"))
		alert.nVersion = atoi(mapToolArgs["-version"]);
	else
		alert.nVersion = PROTOCOL_VERSION;

	if (mapToolArgs.count("-relayuntil"))
		alert.nRelayUntil = atoi64(mapToolArgs["-relayuntil"]);
	else
		alert.nRelayUntil = 4070908800ULL;
	
	if (mapToolArgs.count("-expiration"))
		alert.nExpiration = atoi64(mapToolArgs["-expiration"]);
	else
		alert.nExpiration = 4070908800ULL;

	if (mapToolArgs.count("-id"))
		alert.nID = atoi(mapToolArgs["-id"]);
	else
		alert.nID = 1;

	if (mapToolArgs.count("-cancel"))
		alert.nCancel = atoi(mapToolArgs["-cancel"]);
	else
		alert.nCancel = 0;
	
	//alert.setCancel.insert(alert.nID);
	if (mapToolArgs.count("-setcancel"))
	{
		for(string str : mapMultiToolArgs["-setcancel"])
			alert.setCancel.insert(atoi(str));
	}

	if (mapToolArgs.count("-minver"))
		alert.nMinVer = atoi(mapToolArgs["-minver"]);
	else
		alert.nMinVer = 0;

	if (mapToolArgs.count("-maxver"))
		alert.nMaxVer = atoi(mapToolArgs["-maxver"]);
	else
		alert.nMaxVer = 0x7fffffff;
	
	//alert.setSubVer.insert(FormatSubVersion(CLIENT_NAME, CLIENT_VERSION, std::vector<std::string>()));
	if (mapToolArgs.count("-setsubver"))
	{
		for(string str : mapMultiToolArgs["-setsubver"])
			alert.setSubVer.insert(str);
	}

	if (mapToolArgs.count("-priority"))
		alert.nPriority = atoi(mapToolArgs["-priority"]);
	else
		alert.nPriority = 1;

	if (mapToolArgs.count("-comment"))
		alert.strComment = mapToolArgs["-comment"];
	else
		alert.strComment = "TEST: alert test";

	if (mapToolArgs.count("-statusbar"))
		alert.strStatusBar = mapToolArgs["-statusbar"];
	else
		alert.strStatusBar = "URGENT: Alert key compromised, upgrade required";

	cout << "alert message " << endl << alert.ToString() << endl;
    
    if (mapToolArgs.count("-addnode"))
	{
		for(string strNode : mapMultiToolArgs["-addnode"])
        {
            if(!AddOneNode(strNode))
                cout << "add node failed : " << strNode << endl;
			else
				cout << "add node : " << strNode << endl;
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
