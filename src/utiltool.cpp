#include "utiltool.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>

#include <stdio.h>
#include <stdarg.h>
#include <sstream>

using namespace std;

set<int> g_sDebuglist;
set<int> g_sLoglist;
static boost::mutex* g_mutexDebugLog = NULL;
static FILE* g_fLogFile = NULL;
static boost::once_flag g_debugShowInitFlag = BOOST_ONCE_INIT;

static std::string GetTimestampStr(const std::string &str, bool *fStartedNewLine)
{
    string strStamped;

    if (*fStartedNewLine) {
        int64_t nTime = GetTime();
        strStamped = DateTimeStrFormat("%Y-%m-%d %H:%M:%S", nTime);
        if (fLogTimeMicros)
            strStamped += strprintf(".%06d", nTime);
        strStamped += ' ' + str;
    } else
        strStamped = str;

    return strStamped;
}

int PrintStr(const std::string &str)
{
    int ret = 0; // Returns total number of characters written

    ret = fwrite(str.data(), 1, str.size(), stdout);
    fflush(stdout);
    
    return ret;
}

static void DebugShowInit()
{
    assert(mutexDebugLog == NULL);
    mutexDebugLog = new boost::mutex();
    //vMsgsBeforeOpenLog = new list<string>;
}

int WriteStr(const std::string &str)
{
    int ret = 0; // Returns total number of characters written
    static bool fStartedNewLine = true;

    std::string strTimestamped = GetTimestampStr(strThreadLogged, &fStartedNewLine);

    if (!str.empty() && str[str.size()-1] == '\n')
        fStartedNewLine = true;
    else
        fStartedNewLine = false;

    boost::call_once(&DebugShowInit, debugShowInitFlag);
    boost::mutex::scoped_lock scoped_lock(*mutexDebugLog);

    // buffer if we haven't opened the log yet
    if (fileoutput == NULL) {
		boost::filesystem::path pathDebug = GetDataDir() / "debug.log";
	    fileoutput = fopen(pathDebug.string().c_str(), "a");
    	if (fileoutput) setbuf(fileoutput, NULL); // unbuffered
    }

	ret = fwrite(strTimestamped.data(), 1, strTimestamped.size(), fileoutput);
    
    return ret;
}

int LogShow(const int debug, const char* format, ...)
{
    char buf[1000];
    memset(buf, 0, 1000);
    va_list args;
    va_start(args,format);
    vsprintf(buf,format,args);
    va_end(args);

    if(0 == debug)
        return PrintStr(string(buf, buf + strlen(buf)));
    if(g_sDebuglist.find(debug) != g_sDebuglist.end())
        return PrintStr(string(buf, buf + strlen(buf)));
    if(g_sLoglist.find(debug) != g_sLoglist.end())
        return WriteStr(string(buf, buf + strlen(buf)));
    return 0;
}



/** Interpret string as boolean, for argument parsing */
static bool ToolsInterpretBool(const std::string& strValue)
{
    if (strValue.empty())
        return true;
    return (atoi(strValue.c_str()) != 0);
}

/** Turn -noX into -X=0 */
static void ToolsInterpretNegativeSetting(std::string& strKey, std::string& strValue)
{
    if (strKey.length()>3 && strKey[0]=='-' && strKey[1]=='n' && strKey[2]=='o')
    {
        strKey = "-" + strKey.substr(3);
        strValue = ToolsInterpretBool(strValue) ? "0" : "1";
    }
}
