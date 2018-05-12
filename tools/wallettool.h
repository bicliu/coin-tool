#ifndef COINTOOL_WALLET_H
#define COINTOOL_WALLET_H

bool MakeNewKey(bool fCompressed);

bool CompactSign(std::string strMessage, std::vector<unsigned char>& vchSigRet, CKey privkey);
bool CompactVerify(CPubKey pubkey, const std::vector<unsigned char>& vchSig, std::string strMessage);
bool CheckSign(const CKey privkey,const CPubKey pubkey, const std::vector<unsigned char> vchMsg);

bool CheckKey();

#endif