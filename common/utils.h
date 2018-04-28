#ifndef COINTOOL_UTILS_H
#define COINTOOL_UTILS_H

#include "base58.h"
#include "util.h"

//extern std::map<std::string, std::string> mapToolArgs;
//extern std::map<std::string, std::vector<std::string> > mapMultiToolArgs;

/*ouput funcs*/
int PrintStr(const std::string &str);
bool showerror(const char* format, ...);

/*Init funcs*/
void SetFilePath(const std::string & filename);
void SetParams();

/*net funcs*/
bool AddOneNode(const std::string & strNode, bool fConnectToMasternode = false);
void FreeNode();

/*Key funcs*/
bool GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet);
#endif
