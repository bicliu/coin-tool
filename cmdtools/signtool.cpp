#include "signtool.h"
#include "wallet/wallet.h"

using namespace std;

const std::string strMessageCustom = "Ulord Signed Message:\n";

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

void SignMsgHelp()
{
    cout << "Command \"signmsg\" example :" << endl << endl
        << "signmsg privatekey \"message\"" << endl << endl;
}

void SignMsg(const std::string & strprivkey,const std::string strMessage)
{
    std::vector<unsigned char> vchSig;

    CKey privkey;
    CPubKey retpubkey;
    if(!GetKeysFromSecret(strprivkey, privkey, retpubkey))
    {
        cout << "Error: privkey <" << strprivkey << "> getpubkey failed!" << endl;
        return;
    }

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;
    uint256 msgHash = ss.GetHash();

    if(!privkey.Sign(msgHash, vchSig))
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

void SignVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig)
{
    std::vector<unsigned char> vchSig(DecodeBase64(strSig.c_str()));
    CPubKey pubkey(ParseHex(strpubkey));

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;
    uint256 msgHash = ss.GetHash();

    if (!pubkey.Verify(msgHash, vchSig))
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

void CompactSign(const std::string & strprivkey, std::string strMessage)
{
    std::vector<unsigned char> vchSig;

    CKey privkey;
    CPubKey retpubkey;
    if(!GetKeysFromSecret(strprivkey, privkey, retpubkey))
    {
        cout << "Error: privkey <" << strprivkey << "> getpubkey failed!" << endl;
        return;
    }

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;

    if(!privkey.SignCompact(ss.GetHash(), vchSig))
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

void CompactVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig)
{
    std::vector<unsigned char> vchSig(DecodeBase64(strSig.c_str()));
    CPubKey pubkey(ParseHex(strpubkey));

    CHashWriter ss(SER_GETHASH, 0);
    ss << strMessageCustom;
    ss << strMessage;
    //uint256 msgHash = ss.GetHash();

    CPubKey pubkeyFromSig;
    if(!pubkeyFromSig.RecoverCompact(ss.GetHash(), vchSig)) {
        cout << "Error: recovering public key failed." << endl;
        return;
    }

    if(pubkeyFromSig.GetID() != pubkey.GetID()) {
        cout << "Error: Keys don't match : pubkey = " << HexStr(pubkey).c_str() << ", pubkeyFromSig=" << HexStr(pubkeyFromSig).c_str()
            << ", strMessage=" << strMessage << ", vchSig=" << EncodeBase64(&vchSig[0], vchSig.size()) << endl;
        return;
    }

    cout << "Verify Success !" << endl;
    return;
}