#include "wallet/wallet.h"

#include <secp256k1.h>
#include <secp256k1_recovery.h>

extern CWallet* pwalletMain;
extern secp256k1_context* secp256k1_context_sign;


bool Get2TypePubKey(const CKey & secret)
{
    if(!secret.IsValid())
        return showerror("Get2TypePubKey:secret.fValid");
    secp256k1_pubkey pubkey;
    size_t clen = 65;
    CPubKey result_compressed;
    CPubKey result_uncompressed;
    int ret = secp256k1_ec_pubkey_create(secp256k1_context_sign, &pubkey, secret.begin());
    if(!ret)
        return showerror("Get2TypePubKey:secp256k1_ec_pubkey_create return %d", ret);
    secp256k1_ec_pubkey_serialize(secp256k1_context_sign, (unsigned char*)result_compressed.begin(), &clen, &pubkey, SECP256K1_EC_COMPRESSED);
    assert(result_compressed.size() == clen);
    assert(result_compressed.IsValid());

    secp256k1_ec_pubkey_serialize(secp256k1_context_sign, (unsigned char*)result_uncompressed.begin(), &clen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
    assert(result_uncompressed.size() == clen);
    assert(result_uncompressed.IsValid());

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

    CPubKey pubkey = secret.GetPubKey();
    if(!secret.VerifyPubKey(pubkey))
        return false;

    return true;
}