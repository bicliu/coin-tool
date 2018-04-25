#ifndef COINTOOL_UTILS_H
#define COINTOOL_UTILS_H

#include "base58.h"
#include "util.h"

void ReadFile(std::map<std::string, std::string>& mapSettingsRet,
              std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet,
              const std::string & strfile);
bool AddOneNode(const std::string & strNode);
bool GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet);
#endif
