#include "utils.h"

#include "rewardtool.h"
#include "alerttool.h"

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
	{
		RewardHandle();
	}
	switch(hash_string(mapArgs["-toolcommand"]))
	{
		case hash_string_time("rewardshow"):
			RewardHandle();
			break;

		case hash_string_time("sendalert"):
			SendAlert();
			break;
		default:
			cout << "Error: unknown method " << mapArgs["-toolcommand"] << endl;
			break;
	}

    return 0;
}

