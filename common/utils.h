#ifndef COINTOOL_UTILS_H
#define COINTOOL_UTILS_H

#include "base58.h"
#include "util.h"
//#include "net.h"
//#include "addrman.h"

//class CNode;
//extern std::map<std::string, std::string> mapToolArgs;
//extern std::map<std::string, std::vector<std::string> > mapMultiToolArgs;
const int cmdindex = 1;

/*ouput funcs*/
int PrintStr(const std::string &str);
bool showerror(const char* format, ...);
int showreturn(const char* format, ...);
std::string GetRankString(int index);
int ToolLog(const int debug, const char* format, ...);

/*Init funcs*/
void SetFilePath(const std::string & filename);
void InitSys();
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
bool atob(const std::string & str);



// templated version of my_equal so it could work with both char and wchar_t
template<typename charT>
struct my_equal {
    my_equal( const std::locale& loc ) : loc_(loc) {}
    bool operator()(charT ch1, charT ch2) {
        return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
    }
private:
    const std::locale& loc_;
};

// find substring (case insensitive)
template<typename T>
int ci_find_substr( const T& str1, const T& str2, const std::locale& loc = std::locale() )
{
    typename T::const_iterator it = std::search( str1.begin(), str1.end(),
        str2.begin(), str2.end(), my_equal<typename T::value_type>(loc) );
    if ( it != str1.end() ) return it - str1.begin();
    else return -1; // not found
}

#endif
