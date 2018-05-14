#include "utils.h"

#include "rewardtool.h"
#include "alerttool.h"
#include "wallettool.h"

#include <iomanip>

using namespace std;

static boost::scoped_ptr<ECCVerifyHandle> globalVerifyHandle;

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

	if(2 != argc)
		return showreturn("command as:./tools filename");

	ReadCurrentFile(mapArgs, mapMultiArgs, string(argv[argc - 1]));

	if(!mapArgs.count("-toolcommand"))
		return showreturn("File without command");

	if("rewardshow" == mapArgs["-toolcommand"])
		RewardHandle();
	else if("sendalert" == mapArgs["-toolcommand"])
		SendAlert();
	else if("newaddress" == mapArgs["-toolcommand"])
	{
		if(mapArgs.count("-newnumber"))
		{
			for(int i = 0; i < atoi(mapArgs["-newnumber"]); i++)
				MakeNewKey(GetBoolArg("-fCompressed", true));
		}
		else
			MakeNewKey(GetBoolArg("-fCompressed", true));
	}
	else if("checkkey" == mapArgs["-toolcommand"])
		CheckKey();
	else
		cout << "Error: unknown method " << mapArgs["-toolcommand"] << endl;

    return 0;
}

