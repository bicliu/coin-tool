#include "keytool.h"

#include <thread>

using namespace std;


const std::string strMessageCustom = "Ulord Signed Message:\n";

bool MakeNewKey(bool fCompressed)
{
    CKey secret;
    secret.MakeNewKey(fCompressed);

    CPubKey pubkey = secret.GetPubKey();
    
    if(!secret.VerifyPubKey(pubkey))
        return showerror("VerifyPubKey failed %s", HexStr(pubkey).c_str());

    cout << endl << "privkey : " << CBitcoinSecret(secret).ToString() << endl;

    if(fCompressed)
        cout << "compressed pubkey : " << HexStr(pubkey).c_str() << endl;
    else
        cout << "uncompressed pubkey : " << HexStr(pubkey).c_str() << endl;

	cout << "pubkey2 : " << pubkey.GetID().ToString() << endl;
	cout << "address : " << CBitcoinAddress(pubkey.GetID()).ToString() << endl << endl;

    return true;
}

bool CompactSign(const CKey & privkey, const std::string & strMessage, std::vector<unsigned char>& vchSigRet)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    return privkey.SignCompact(ss.GetHash(), vchSigRet);
}

bool CompactVerify(const CPubKey & pubkey, const std::string & strMessage,const std::vector<unsigned char>& vchSig, bool blog)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    CPubKey pubkeyFromSig;
    if(!pubkeyFromSig.RecoverCompact(ss.GetHash(), vchSig)) {
        cout << "Error recovering public key." << endl;
        return false;
    }

    if(pubkeyFromSig.GetID() != pubkey.GetID()) {
        if(blog)
            cout << "Keys don't match : pubkey = " << HexStr(pubkey).c_str() << ", pubkeyFromSig=" << HexStr(pubkeyFromSig).c_str()
                << ", strMessage=" << strMessage << ", vchSig=" << EncodeBase64(&vchSig[0], vchSig.size()) << endl;
        return false;
    }

    return true;
}

bool MsgSign(const CKey & privkey, const std::string & strMessage, std::vector<unsigned char>& vchSig, bool blog)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    if(!privkey.Sign(ss.GetHash(), vchSig))
	{
        if(blog)
            cout << "Error: CheckSign: Sign msg failed! privkey = " << HexStr(privkey).c_str() << endl;
    	return false;
	}
    return true;
}

bool MsgVerify(const CPubKey & pubkey, const std::string & strMessage, const std::vector<unsigned char>& vchSig, bool blog)
{
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    if (!pubkey.Verify(ss.GetHash(), vchSig))
    {
        if(blog)
            cout << "Error: CheckSign: Verify failed! pubkey = " << pubkey.GetID().ToString() << endl;
        return false;
    }
    return true;
}

bool IsPairOfKey(CKey privkey, CPubKey pubkey, std::string msg)
{
    vector<unsigned char> vchSig;
    vector<unsigned char> vchSig1;

    if(!MsgSign(privkey, msg, vchSig1))
        return false;

    if(!MsgVerify(pubkey, msg, vchSig1))
        return false;

    if(!CompactSign(privkey, msg, vchSig))
        return false;

    if(!CompactVerify(pubkey, msg, vchSig))
        return false;

    return true;
}

bool CheckKey()
{
    string msg;
    vector<CKey> vSecret;
    vector<CPubKey> vPublic;
    if(mapArgs.count("-privkey"))
    {
        for(string str : mapMultiArgs["-privkey"])
        {
            CKey privkey;
            CPubKey retpubkey;
            if(!GetKeysFromSecret(str, privkey, retpubkey))
            {
                cout << "Error: privkey <" << str << "> getpubkey failed!" << endl;
                continue;
            }
            vSecret.push_back(privkey);
        }
    }
    else
        return showerror("File without privkey, add privkey= frist!");

    if(mapArgs.count("-pubkey"))
    {
        for(string str : mapMultiArgs["-pubkey"])
        {
            CPubKey pubkey(ParseHex(str));
            vPublic.push_back(pubkey);
        }
    }
    else
        return showerror("File without pubkey, add pubkey= frist!");
    
    if(mapArgs.count("-message"))
        msg = mapArgs["-message"];
    else
        return showerror("File without message, add message= frist!");
    
    vector<unsigned char> vchSig;
    for(CKey secret : vSecret)
    {
        bool bIsPair = false;
		cout << "private key <" << CBitcoinSecret(secret).ToString() << ">" << endl << "{" << endl;
        for(vector<CPubKey>::iterator iter = vPublic.begin(); iter != vPublic.end(); )
        {
            if(IsPairOfKey(secret, *iter, msg))
            {
                bIsPair = true;
				cout << "    publickey <" << HexStr(*iter).c_str() << "> address " << CBitcoinAddress((*iter).GetID()).ToString()  << endl;
				iter = vPublic.erase(iter);
            }
			else
				iter++;
        }
		cout << "}" << endl;
        CPubKey retPub = secret.GetPubKey();
        if(!bIsPair)
            cout << "get publickey <" << HexStr(retPub).c_str() << "> address " << CBitcoinAddress(retPub.GetID()).ToString()  << endl << endl;
    }

	if(vPublic.size() != 0)
	{
		cout << endl << "Get address from pubkey:" << endl;
		for(CPubKey publickey : vPublic)
			cout << "publickey <" << HexStr(publickey).c_str() << "> get address " << CBitcoinAddress(publickey.GetID()).ToString()  << endl;
	}

    return true;
}

void PrivKeyHelp()
{
    cout << "Command \"privkey\" example :" << endl << endl
        << "privkey privatekey" << endl << endl;
}
void PrivKey(int argc, char* argv[])
{
    if(argc < cmdindex+2)
    {
        PrivKeyHelp();
        return;
    }
    string strprivkey = argv[cmdindex+1];
    CKey privkey;
    CPubKey retpubkey;
    if(!GetKeysFromSecret(strprivkey, privkey, retpubkey))
    {
        cout << "Error: privkey <" << strprivkey << "> getpubkey failed!" << endl;
        return;
    }
    ToolLog(0, "private key    %s\n{\n\tpublic key    %s\n\taddress       %s\n}\n",
                CBitcoinSecret(privkey).ToString().c_str(), HexStr(retpubkey).c_str(), CBitcoinAddress(retpubkey.GetID()).ToString().c_str());
    return;
}

void SignMsgHelp()
{
    cout << "Command \"signmsg\" example :" << endl << endl
        << "signmsg privatekey \"message\"" << endl << endl;
}

void SignMsg(int argc, char* argv[])
{
    if(argc < cmdindex+3)
    {
        SignMsgHelp();
        return;
    }
    string strprivkey = argv[cmdindex+1]; 
    string strMessage = argv[cmdindex+2];
    std::vector<unsigned char> vchSig;

    CKey privkey;
    CPubKey retpubkey;
    if(!GetKeysFromSecret(strprivkey, privkey, retpubkey))
    {
        cout << "Error: privkey <" << strprivkey << "> getpubkey failed!" << endl;
        return;
    }

    if(!MsgSign(privkey, strMessage, vchSig))
	{
        cout << "Error: Sign msg failed! privkey = " << CBitcoinSecret(privkey).ToString() << endl;
        return;
    }

    cout << "Base64Code: " << EncodeBase64(&vchSig[0], vchSig.size()) << endl;
    return;
}

void SignVerifyHelp()
{
    cout << "Command \"verifymsg\" example :" << endl << endl
        << "verifymsg publickey \"message\" \"signature\"" << endl << endl;
}

void SignVerify(int argc, char* argv[])
{
    if(argc < cmdindex+4)
    {
        SignVerifyHelp();
        return;
    }
    string strSig = argv[cmdindex+3];
    
    std::vector<unsigned char> vchSig(DecodeBase64(strSig.c_str()));
    CPubKey pubkey(ParseHex(argv[cmdindex+1]));

    if (!MsgVerify(pubkey, argv[cmdindex+2], vchSig))
    {
        cout << "Error: Verify failed! pubkey = " << HexStr(pubkey).c_str() << endl;
        return;
    }

    cout << "Verify Success !" << endl;
    return;
}

void CompactSignHelp()
{
    cout << "Command \"compactsign\" example :" << endl << endl
        << "compactsign privatekey \"message\"" << endl << endl;
}

void CompactSignHandle(int argc, char* argv[])
{
    if(argc < cmdindex+3)
    {
        CompactSignHelp();
        return;
    }
	string strprivkey = argv[cmdindex+1];
    string strMessage = argv[cmdindex+2];
    std::vector<unsigned char> vchSig;

    CKey privkey;
    CPubKey retpubkey;
    if(!GetKeysFromSecret(strprivkey, privkey, retpubkey))
    {
        cout << "Error: privkey <" << strprivkey << "> getpubkey failed!" << endl;
        return;
    }

    if(!CompactSign(privkey, strMessage, vchSig))
    {
        cout << "Error: Sign msg failed! privkey = " << CBitcoinSecret(privkey).ToString() << endl;
        return;
    }

    cout << "Base64Code: " << EncodeBase64(&vchSig[0], vchSig.size()) << endl;
    return;
}

void CompactVerifyHelp()
{
    cout << "Command \"compactverify\" example :" << endl << endl
        << "compactverify publickey \"message\" \"signature\"" << endl << endl;
}

void CompactVerifyHandle(int argc, char* argv[])
{
    if(argc < cmdindex+4)
    {
        CompactVerifyHelp();
        return;
    }
    string strSig = argv[cmdindex+3];

    std::vector<unsigned char> vchSig(DecodeBase64(strSig.c_str()));
    CPubKey pubkey(ParseHex(argv[cmdindex+1]));

    if(!CompactVerify(pubkey, argv[cmdindex+2], vchSig, true)) return;

    cout << "Verify Success !" << endl;
    return;
}

void NewAddressHelp()
{
    cout << "Command \"newaddress\" example :" << endl << endl
        << "newaddress Ifcompressed number" << endl << endl;
}

void NewAddress(int argc, char* argv[])
{
    if(argc < cmdindex+3)
    {
        NewAddressHelp();
        return;
    }

    int num = atoi(argv[cmdindex+2]);

    for (int i = 0; i < num; i++)
        MakeNewKey(atob(argv[cmdindex+1]));
}

void FindAddressHelp()
{
    cout << "Command \"findaddress\" example :" << endl << endl
        << "findaddress IsCase \"target\" ..." << endl
		<< "findaddress true \"example\""<< endl;
}

void _getaddress(const vector <string> * vTarget,const int & index, const bool & bIsCase, int * result)
{
	//cout << "thread " << index << " start" << endl;
	CKey secret;
	CPubKey pubkey;
	string addrPro;
	uint32_t imatch = 0;
    while(0 == *result)
    {
        secret.MakeNewKey(true);
        pubkey = secret.GetPubKey();
        
        if(!secret.VerifyPubKey(pubkey))
        {
            cout << "VerifyPubKey failed " << HexStr(pubkey).c_str();
            return;
        }

        addrPro = CBitcoinAddress(pubkey.GetID()).ToString();
        imatch = 0;
        for(auto var : (*vTarget))
        {
			bool bfind = !bIsCase ? (-1 != ci_find_substr(addrPro, var)) : (addrPro.find(var) != string::npos);
            if(bfind)
            {
				imatch++;
				if(vTarget->size() == imatch)
				{
                	cout << endl << "Task " << index << " get target." << endl
						<< "privkey : " << CBitcoinSecret(secret).ToString() << endl
                		<< "address : " << addrPro << endl << endl;
                	(*result)++;
                	return;
				}
            }
			else
				break;
        }
    }
}

void FindAddress(int argc, char* argv[])
{
    if(argc < cmdindex+3)
    {
        FindAddressHelp();
        return;
    }

	bool fIsCase = atob(argv[cmdindex + 1]);
    vector <string> vTarget;
    for(int i = cmdindex+2; i < argc; i++)
        vTarget.push_back(argv[i]);

    cout << "Looking for ... ..." << endl;

    std::vector<std::thread> threads;
	int icpu = std::min(GetNumCores(), 100);
    int iresult = 0;
	cout << "Get " << icpu << " cpus to use" << endl;

    for (int i = 0; i < icpu; ++i)
    {
        threads.push_back(std::thread(_getaddress, &vTarget, i, fIsCase,  &iresult));
    }
    for (auto &t : threads)
    {
        t.join();
    }
    cout << endl << "Congratulations! Find an expectant address" << endl;

    return;
}
