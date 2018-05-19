#include "utils.h"
#include "rewardtool.h"
#include "alerttool.h"
#include "wallettool.h"
#include "genesis.h"

#include <iomanip>

using namespace std;

typedef void (*FP_CMD)(int, char)

static boost::scoped_ptr<ECCVerifyHandle> globalVerifyHandle;
map <string, FP_CMD> mapCommand;

void RegisterCmd()
{
	mapCommand.insert(make_pair("help", &ShowCmds));
	mapCommand.insert(make_pair("signmsg", &SignMsg));
	mapCommand.insert(make_pair("verifymsg", &SignMsg));
	mapCommand.insert(make_pair("compactsign", &SignMsg));
	mapCommand.insert(make_pair("compactverify", &SignMsg));
	mapCommand.insert(make_pair("newaddress", &SignMsg));
	mapCommand.insert(make_pair("sendalert", &SignMsg));
	mapCommand.insert(make_pair("gettime", &SignMsg));
	mapCommand.insert(make_pair("blockreward", &SignMsg));
	mapCommand.insert(make_pair("rewardstatistics", &SignMsg));
	mapCommand.insert(make_pair("rewardexample", &SignMsg));
	mapCommand.insert(make_pair("genesis", &SignMsg));
}

void ShowCmds(int argc, char* argv[])
{
	cout << "********* commands *********" << endl;
	for(map<string, FP_CMD>::iterator iter = mapCommand.begin(); iter != mapCommand.end(); iter++)
		cout << iter->first << endl;
	cout << "********* commands *********" << endl;
}

int main(int argc, char* argv[])
{
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();
	globalVerifyHandle.reset(new ECCVerifyHandle());

	RegisterCmd();
	
	// Sanity check no need
    //if (!ECC_InitSanityCheck())
    //    return showreturn(("ECC_InitSanityCheck sanity check failed. Shutting down."));

	if(argc < 2)
	{
		ShowCmds(argc, argv);
		return -1;
	}

	string strcmd = string(argv[cmdindex]);

	map<string, FP_CMD>::iterator iterCMD = mapCommand.find(strcmd);

	if(iter != mapCommand.end())
		iterCMD->second(argc, argv);
	else
		cout << "Error: unknown method " << strcmd << endl;
	/*if("signmsg" == strcmd)
		SignMsg(argc, argv);
	else if("verifymsg" == strcmd)
		SignVerify(argc, argv);
	else if("compactsign" == strcmd)
		CompactSignHandle(argc, argv);
	else if("compactverify" == strcmd)
		CompactVerifyHandle(argc, argv);
	else if("newaddress" == strcmd)
		NewAddress(argc, argv);
	else if("sendalert" == strcmd)
		SendAlertHandle(argc, argv);
	else if("gettime" == strcmd)
		GetTimeHandle(argc, argv);
	else if("blockreward" == strcmd)
		BlockReward(argc, argv);
	else if("rewardstatistics" == strcmd)
		RewardStatisticsHandle(argc, argv);
	else if("rewardexample" == strcmd)
		RewardExample(argc, argv);
	else if("genesis" == strcmd)
		GenesisLookFor(argc, argv);
	else
		cout << "Error: unknown method " << strcmd << endl;*/

    return 0;
}

