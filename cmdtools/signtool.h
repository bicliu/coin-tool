#ifndef COINTOOL_SIGN_H
#define COINTOOL_SIGN_H
#include "utils.h"

void SignMsg(const std::string & strprivkey,const std::string strMessage);
void SignVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig);
void CompactSign(const std::string & strprivkey, std::string strMessage);
void CompactVerify(const std::string & strpubkey,const std::string & strMessage, const std::string & strSig);

void SignMsgHelp();
void SignVerifyHelp();
void CompactSignHelp();
void CompactVerifyHelp();

#endif