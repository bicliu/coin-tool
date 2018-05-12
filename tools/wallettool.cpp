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

    cout << "privkey : " << HexStr(secret).c_str() << endl << "pubkey short : " << HexStr(pubkey).c_str() << endl;

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
        cout << "Keys don't match : pubkey = " << pubkey.GetID().ToString() << ", pubkeyFromSig=" << pubkeyFromSig.GetID().ToString()
            << ", strMessage=" << strMessage << ", vchSig=" << EncodeBase64(&vchSig[0], vchSig.size()) << endl;
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

    std::vector<unsigned char> vchSig;
    if(!privkey.Sign(ss.GetHash(), vchSig))
	{
        cout << "Error: Sign msg failed! privkey = " << HexStr(privkey).c_str() << endl;
    	return false;
	}
    if (!pubkey.Verify(ss.GetHash(), vchSig))
    {
        cout << "Error: Verify failed! pubkey = " << pubkey.GetID().ToString() << endl;
        return false;
    }
    return true;
}

bool CheckKey()
{
    string strPrikey;
    string strPubkey;
    if(mapArgs.count("-privkey"))
        strPrikey = mapArgs["-privkey"];
    else
        return showerror("File without privkey, add privkey= frist!");

    if(mapArgs.count("-pubkey"))
        strPubkey = mapArgs["-pubkey"];
    else
        return showerror("File without privkey, add pubkey= frist!");
    
    if(!mapArgs.count("-message"))
        return showerror("File without privkey, add message= frist!");
    
    CKey prikey;
	CPubKey pubkey(ParseHex(strPubkey));
    CPubKey retpubkey;
    vector<unsigned char> vchSig;
	
    if(!GetKeysFromSecret(strPrikey, prikey, retpubkey))
        return showerror("CheckKey get privkey failed!");

    //cout << "Info for retpubkey " << retpubkey.GetID().ToString() << endl;

    if(!CheckSign(prikey, pubkey, mapArgs["-message"]))
        return false;

    if(!CompactSign(mapArgs["-message"], vchSig, prikey))
        return false;

    if(!CompactVerify(retpubkey, vchSig, mapArgs["-message"]))
        return false;

    cout << "check key success!" << endl;
    return true;
}