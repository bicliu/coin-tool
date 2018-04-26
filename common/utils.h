#ifndef COINTOOL_UTILS_H
#define COINTOOL_UTILS_H

#include "base58.h"
#include "util.h"
#include "net.h"

//extern std::map<std::string, std::string> mapToolArgs;
//extern std::map<std::string, std::vector<std::string> > mapMultiToolArgs;

void SetFilePath(const std::string & filename);
bool AddOneNode(const std::string & strNode, bool fConnectToMasternode = false);
void FreeNode(CNode* pnode);
bool GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet);
void SetParams();

#endif
