#include "utils.h"
#include "wallet/wallet.h"

using namespace std;

extern CWallet* pwalletMain;
/*extern secp256k1_context* secp256k1_context_sign;


bool Get2TypePubKey(const CKey & secret, CPubKey & result_compressed, CPubKey & result_uncompressed)
{
    if(!secret.IsValid())
        return showerror("Get2TypePubKey:secret.fValid");
    secp256k1_pubkey pubkey;
    size_t clen = 65;

    int ret = secp256k1_ec_pubkey_create(secp256k1_context_sign, &pubkey, secret.begin());
    if(!ret)
        return showerror("Get2TypePubKey:secp256k1_ec_pubkey_create return %d", ret);
    secp256k1_ec_pubkey_serialize(secp256k1_context_sign, (unsigned char*)result_compressed.begin(), &clen, &pubkey, SECP256K1_EC_COMPRESSED);
    if(result_compressed.size() != clen)
        return showerror("get invalid compressed pubkey length = %d", result_compressed.size());
    if(!result_compressed.IsValid())
        return showerror("get invalid compressed pubkey");

    secp256k1_ec_pubkey_serialize(secp256k1_context_sign, (unsigned char*)result_uncompressed.begin(), &clen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
    if(result_uncompressed.size() != clen)
        return showerror("get invalid compressed pubkey length = %d", result_uncompressed.size());
    if(!result_uncompressed.IsValid())
        return showerror("get invalid compressed pubkey");

    return true;
}

bool MakeNewKey()
{
    bool fCompressed = pwalletMain->CanSupportFeature(FEATURE_COMPRPUBKEY); // default to compressed public keys if we want 0.6.0 wallets

    CKey secret;
    secret.MakeNewKey(fCompressed);

    // Compressed public keys were introduced in version 0.6.0
    if (fCompressed)
        pwalletMain->SetMinVersion(FEATURE_COMPRPUBKEY);

    //CPubKey pubkey = secret.GetPubKey();
    CPubKey pubkeys;
    CPubKey pubkeyl;

    if(!Get2TypePubKey(secret, pubkeys, pubkeyl))
        return false;
    
    if(!secret.VerifyPubKey(pubkeys))
        return showerror("VerifyPubKey failed %s", HexStr(pubkeys).c_str());

    if(!secret.VerifyPubKey(pubkeyl))
        return showerror("VerifyPubKey failed %s", HexStr(pubkeys).c_str());

    cout << "privkey : " << HexStr(secret).c_str() << endl << "pubkey short : " << HexStr(pubkeys).c_str() << endl << "pubkey long : " << HexStr(pubkeyl).c_str() << endl;

    return true;
}*/

bool MakeNewKey()
{
	AssertLockHeld(pwalletMain->cs_wallet); // mapKeyMetadata

    bool fCompressed = pwalletMain->CanSupportFeature(FEATURE_COMPRPUBKEY); // default to compressed public keys if we want 0.6.0 wallets

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
