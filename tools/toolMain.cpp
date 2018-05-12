#include "utils.h"
#include "reward.h"
#include "main.h"
#include "governance-classes.h"

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

	ReadFile(mapArgs, mapMultiArgs, string(argv[argc - 1]));

	if(!mapArgs.count("-toolcommand"))
		return showreturn("File without command");

	if("reward" == mapArgs["-toolcommand"])
	{
		RewardHandle();
	}

    return 0;
}

