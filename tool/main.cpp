#include "utils.h"
#include "rewardtool.h"
#include "alerttool.h"
#include "keytool.h"
#include "genesis.h"

#include <iomanip>

using namespace std;

typedef void (*FP_CMD)(int, char**);

static boost::scoped_ptr<ECCVerifyHandle> globalVerifyHandle;
map <string, FP_CMD> mapCommand;

void ShowCmds(int argc, char* argv[])
{
    cout << "********* commands *********" << endl;
    for(map<string, FP_CMD>::iterator iter = mapCommand.begin(); iter != mapCommand.end(); iter++)
        cout << iter->first << endl;
    cout << "********* commands *********" << endl;
}

void RegisterCmd()
{
	mapCommand.insert(make_pair("help", &ShowCmds));
	mapCommand.insert(make_pair("signmsg", &SignMsg));
	mapCommand.insert(make_pair("verifymsg", &SignVerify));
	mapCommand.insert(make_pair("compactsign", &CompactSignHandle));
	mapCommand.insert(make_pair("compactverify", &CompactVerifyHandle));
	mapCommand.insert(make_pair("newaddress", &NewAddress));
	mapCommand.insert(make_pair("sendalert", &SendAlertHandle));
	mapCommand.insert(make_pair("gettime", &GetTimeHandle));
	mapCommand.insert(make_pair("blockreward", &BlockReward));
	mapCommand.insert(make_pair("rewardstatistics", &RewardStatisticsHandle));
	mapCommand.insert(make_pair("rewardexample", &RewardExample));
	mapCommand.insert(make_pair("genesis", &GenesisLookFor));
	mapCommand.insert(make_pair("findaddress", &FindAddress));
	mapCommand.insert(make_pair("privkey", &PrivKey));
}

int main(int argc, char* argv[])
{
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	InitSys();
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

	if(iterCMD != mapCommand.end())
		iterCMD->second(argc, argv);
	else
		cout << "Error: unknown method " << strcmd << endl;

    return 0;
}

