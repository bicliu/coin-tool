#include "utils.h"
#include "wallet/wallet.h"

using namespace std;

extern CWallet* pwalletMain;

string strMessageCustom = "TEST: sign test";

bool MakeNewKey(bool fCompressed)
{
    pwalletMain = new CWallet();
	AssertLockHeld(pwalletMain->cs_wallet); // mapKeyMetadata

    CKey secret;
    secret.MakeNewKey(fCompressed);

    // Compressed public keys were introduced in version 0.6.0
    if (fCompressed)
        pwalletMain->SetMinVersion(FEATURE_COMPRPUBKEY);

    CPubKey pubkey = secret.GetPubKey();
    
    if(!secret.VerifyPubKey(pubkey))
        return showerror("VerifyPubKey failed %s", HexStr(pubkey).c_str());

    cout << endl << "privkey : " << CBitcoinSecret(secret).ToString() << endl;

    if(fCompressed)
        cout << "compressed pubkey : " << HexStr(pubkey).c_str() << endl;
    else
        cout << "uncompressed pubkey : " << HexStr(pubkey).c_str() << endl;

	cout << "address : " << CBitcoinAddress(pubkey.GetID()).ToString() << endl << endl;

    return true;
}

bool CompactSign(std::string strMessage, std::vector<unsigned char>& vchSigRet, CKey privkey)
{
    if (mapArgs.count("-magicstr"))
		strMessageCustom = mapArgs["-magicstr"];

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    return privkey.SignCompact(ss.GetHash(), vchSigRet);
}

bool CompactVerify(CPubKey pubkey, const std::vector<unsigned char>& vchSig, std::string strMessage)
{
    if (mapArgs.count("-magicstr"))
		strMessageCustom = mapArgs["-magicstr"];
        
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    CPubKey pubkeyFromSig;
    if(!pubkeyFromSig.RecoverCompact(ss.GetHash(), vchSig)) {
        cout << "Error recovering public key." << endl;
        return false;
    }

    if(pubkeyFromSig.GetID() != pubkey.GetID()) {
        /*cout << "Keys don't match : pubkey = " << pubkey.GetID().ToString() << ", pubkeyFromSig=" << pubkeyFromSig.GetID().ToString()
            << ", strMessage=" << strMessage << ", vchSig=" << EncodeBase64(&vchSig[0], vchSig.size()) << endl;*/
        return false;
    }

    return true;
}

bool CheckSign(const CKey privkey,const CPubKey pubkey, const std::string strMessage)
{
    if (mapArgs.count("-magicstr"))
		strMessageCustom = mapArgs["-magicstr"];
        
    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;
    uint256 msgHash = ss.GetHash();

    std::vector<unsigned char> vchSig;
    if(!privkey.Sign(msgHash, vchSig))
	{
        //cout << "Error: CheckSign: Sign msg failed! privkey = " << HexStr(privkey).c_str() << endl;
    	return false;
	}
    if (!pubkey.Verify(msgHash, vchSig))
    {
        //cout << "Error: CheckSign: Verify failed! pubkey = " << pubkey.GetID().ToString() << endl;
        return false;
    }
    return true;
}

bool IsPairOfKey(CKey privkey, CPubKey pubkey, std::string msg)
{
    vector<unsigned char> vchSig;

    if(!CheckSign(privkey, pubkey, msg))
        return false;

    if(!CompactSign(msg, vchSig, privkey))
        return false;

    if(!CompactVerify(pubkey, vchSig, msg))
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
		cout << "private key <" << CBitcoinSecret(secret).ToString() << ">" << endl << "{" << endl;
        for(CPubKey publickey : vPublic)
        {
            if(IsPairOfKey(secret, publickey, msg))
            {
				cout << "    publickey <" << HexStr(publickey).c_str() << "> address " << CBitcoinAddress(publickey.GetID()).ToString()  << endl;
            }
        }
		cout << "}" << endl;
    }

    return true;
}

