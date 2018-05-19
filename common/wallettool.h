#ifndef COINTOOL_WALLET_H
#define COINTOOL_WALLET_H
#include "utils.h"

bool MakeNewKey(bool fCompressed);

bool CompactSign(const CKey & privkey, const std::string & strMessage, std::vector<unsigned char>& vchSigRet);
bool CompactVerify(const CPubKey & pubkey, const std::string & strMessage,const std::vector<unsigned char>& vchSig, bool blog = false);

bool CheckKey();
bool IsPairOfKey(CKey privkey, CPubKey pubkey, std::string msg);

bool MsgSign(const CKey & privkey, const std::string & strMessage, std::vector<unsigned char>& vchSig, bool blog = false);
bool MsgVerify(const CPubKey & pubkey, const std::string & strMessage, const std::vector<unsigned char>& vchSig, bool blog = false);

void SignMsg(int argc, char* argv[]);
void SignVerify(int argc, char* argv[]);
void CompactSignHandle(int argc, char* argv[]);
void CompactVerifyHandle(int argc, char* argv[]);
void NewAddress(int argc, char* argv[]);
#endif