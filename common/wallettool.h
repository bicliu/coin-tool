#ifndef COINTOOL_WALLET_H
#define COINTOOL_WALLET_H
#include "utils.h"

bool MakeNewKey(bool fCompressed);

bool CompactSign(std::string strMessage, std::vector<unsigned char>& vchSigRet, CKey privkey);
bool CompactVerify(CPubKey pubkey, const std::vector<unsigned char>& vchSig, std::string strMessage);

bool CheckKey();
bool IsPairOfKey(CKey privkey, CPubKey pubkey, std::string msg);

bool MsgSign(const CKey & privkey, const std::string & strMessage, std::vector<unsigned char>& vchSig);
bool MsgVerify(const CPubKey & pubkey, const std::string & strMessage, const std::vector<unsigned char>& vchSig);

void SignMsg(const std::string & strprivkey,const std::string strMessage);
void SignVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig);
void CompactSign(const std::string & strprivkey, std::string strMessage);
void CompactVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig);

void SignMsgHelp();
void SignVerifyHelp();
void CompactSignHelp();
void CompactVerifyHelp();
#endif