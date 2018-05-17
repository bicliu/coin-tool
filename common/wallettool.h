#ifndef COINTOOL_WALLET_H
#define COINTOOL_WALLET_H
#include "utils.h"

bool MakeNewKey(bool fCompressed);

bool CompactSign(std::string strMessage, std::vector<unsigned char>& vchSigRet, CKey privkey);
bool CompactVerify(CPubKey pubkey, const std::vector<unsigned char>& vchSig, std::string strMessage);
bool CheckSign(const CKey privkey,const CPubKey pubkey, const std::vector<unsigned char> vchMsg);

bool CheckKey();
bool IsPairOfKey(CKey privkey, CPubKey pubkey, std::string msg);

#endif