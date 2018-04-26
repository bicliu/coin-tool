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
	
	SetFilePath("alertarg.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();
	ECCVerifyHandle globalVerifyHandle;

    if(mapArgs.count("-privkey"))
        strPrikey = mapArgs["-privkey"];
    else
        strPrikey = "cPu3SSXjZMzhnqq6mVpNmskdLAFwdcQW3JHgqU6M2s1beTAQWNdW";

    if (mapArgs.count("-version"))
		alert.nVersion = atoi(mapArgs["-version"]);
	else
		alert.nVersion = PROTOCOL_VERSION;

	if (mapArgs.count("-relayuntil"))
		alert.nRelayUntil = atoi64(mapArgs["-relayuntil"]);
	else
		alert.nRelayUntil = 4070908800ULL;
	
	if (mapArgs.count("-expiration"))
		alert.nExpiration = atoi64(mapArgs["-expiration"]);
	else
		alert.nExpiration = 4070908800ULL;

	if (mapArgs.count("-id"))
		alert.nID = atoi(mapArgs["-id"]);
	else
		alert.nID = 1;

	if (mapArgs.count("-cancel"))
		alert.nCancel = atoi(mapArgs["-cancel"]);
	else
		alert.nCancel = 0;
	
	//alert.setCancel.insert(alert.nID);
	if (mapArgs.count("-setcancel"))
	{
		for(string str : mapMultiArgs["-setcancel"])
			alert.setCancel.insert(atoi(str));
	}

	if (mapArgs.count("-minver"))
		alert.nMinVer = atoi(mapArgs["-minver"]);
	else
		alert.nMinVer = 0;

	if (mapArgs.count("-maxver"))
		alert.nMaxVer = atoi(mapArgs["-maxver"]);
	else
		alert.nMaxVer = 0x7fffffff;
	
	//alert.setSubVer.insert(FormatSubVersion(CLIENT_NAME, CLIENT_VERSION, std::vector<std::string>()));
	if (mapArgs.count("-setsubver"))
	{
		for(string str : mapMultiArgs["-setsubver"])
			alert.setSubVer.insert(str);
	}

	if (mapArgs.count("-priority"))
		alert.nPriority = atoi(mapArgs["-priority"]);
	else
		alert.nPriority = 1;

	if (mapArgs.count("-comment"))
		alert.strComment = mapArgs["-comment"];
	else
		alert.strComment = "TEST: alert test";

	if (mapArgs.count("-statusbar"))
		alert.strStatusBar = mapArgs["-statusbar"];
	else
		alert.strStatusBar = "URGENT: Alert key compromised, upgrade required";

	cout << "alert message " << endl << alert.ToString() << endl;
    
    if (mapArgs.count("-addnode"))
	{
		for(string strNode : mapMultiArgs["-addnode"])
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

	int iSuccess = 0;

    LOCK(cs_vNodes);
    BOOST_FOREACH(CNode* pnode, vNodes)
    {
    	if(!alert.RelayTo(pnode))
            cout << "Send failed to Node " << pnode->addr.ToString() << endl;
    	else
		{
			cout << "Send success to Node " << pnode->addr.ToString() << endl;
			iSuccess++;
		}
    }

	cout << "send alert finished. total send to " << vNodes.size() << " nodes. " << iSuccess << " successes!" endl;

	MilliSleep(10000);
	FreeNode();

    return 0;
}
