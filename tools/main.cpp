#include "utils.h"

#include "rewardtool.h"
#include "alerttool.h"
#include "wallettool.h"

#include <iomanip>

using namespace std;

int main(int argc, char* argv[])
{
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();

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
		MakeNewKey();
	else
		cout << "Error: unknown method " << mapArgs["-toolcommand"] << endl;

    return 0;
}

