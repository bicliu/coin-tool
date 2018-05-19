#include "utils.h"
#include "rewardtool.h"
#include "alerttool.h"
#include "wallettool.h"
#include "genesis.h"

#include <iomanip>

using namespace std;

static boost::scoped_ptr<ECCVerifyHandle> globalVerifyHandle;

void RegisterCmdForHelp()
{
	cout << "********* commands *********" << endl;
	cout << "signmsg" << endl;
	cout << "verifymsg" << endl;
	cout << "compactsign" << endl;
	cout << "compactverify" << endl;
	cout << "newaddress" << endl;
	cout << "sendalert" << endl;
	cout << "gettime" << endl;
	cout << "blockreward" << endl;
	cout << "rewardstatistics" << endl;
	cout << "rewardexample" << endl;
	cout << "********* commands *********" << endl;
}

int main(int argc, char* argv[])
{
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();
	globalVerifyHandle.reset(new ECCVerifyHandle());
	
	// Sanity check no need
    //if (!ECC_InitSanityCheck())
    //    return showreturn(("ECC_InitSanityCheck sanity check failed. Shutting down."));

	if(argc < 2)
	{
		RegisterCmdForHelp();
		return -1;
	}

	string strcmd = string(argv[cmdindex]);
	
	if("signmsg" == strcmd)
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
	{
		int64_t tnow = GetTime();
		int64_t tmnow = GetTimeMicros();
		cout << "Time is " << DateTimeStrFormat("%Y-%m-%d %H:%M:%S", tnow) << endl
			<< "<" << tnow << ">,<" << tmnow << ">" << endl;;
	}
	else if("blockreward" == strcmd)
		BlockReward(argc, argv);
	else if("rewardstatistics" == strcmd)
		RewardStatisticsHandle(argc, argv);
	else if("rewardexample" == strcmd)
		RewardExample(argc, argv);
	else if("genesis" == strcmd)
		GenesisLookFor(argc, argv);
	else
		cout << "Error: unknown method " << strcmd << endl;

    return 0;
}

