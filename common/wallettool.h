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

void SignMsgHelp();
void SignMsg(const std::string & strprivkey,const std::string strMessage);

void SignVerifyHelp();
void SignVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig);

void CompactSignHelp();
void CompactSign(const std::string & strprivkey, std::string strMessage);

void CompactVerifyHelp();
void CompactVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig);

void NewAddressHelp();
void NewAddress(bool bCompressed, int num);
#endif