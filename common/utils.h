#ifndef COINTOOL_UTILS_H
#define COINTOOL_UTILS_H

#include "base58.h"
#include "util.h"
//#include "net.h"
//#include "addrman.h"

//class CNode;
//extern std::map<std::string, std::string> mapToolArgs;
//extern std::map<std::string, std::vector<std::string> > mapMultiToolArgs;

/*ouput funcs*/
int PrintStr(const std::string &str);
bool showerror(const char* format, ...);
int showreturn(const char* format, ...);
std::string GetRankString(int index);

/*Init funcs*/
void SetFilePath(const std::string & filename);
void SetParams();
bool ReadFile(std::map<std::string, std::string>& mapSettingsRet,
              std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet,
              const std::string & strfile);
bool ReadCurrentFile(std::map<std::string, std::string>& mapSettingsRet,
              std::map<std::string, std::vector<std::string> >& mapMultiSettingsRet,
              const std::string & strfile);

/*net funcs*/
bool AddOneNode(const std::string & strNode, bool fConnectToMasternode = false);
void FreeNode();

/*Key funcs*/
bool GetKeysFromSecret(std::string strSecret, CKey& keyRet, CPubKey& pubkeyRet);

/*pub funcs*/
uint64_t hash_string (const char *str);
constexpr uint64_t hash_string_time(char const* str)
#endif
