#include "utils.h"
#include "main.h"

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

	if("subsidy" == mapArgs["-toolcommand"])
	{
		if (mapArgs.count("-height"))
		{
			cout << "  height      MinerSubsidy           Budget        MasternodePayment     FoundersReward       BlockSubsidy" << endl;
			for(string str : mapMultiArgs["-height"])
			{
				int h = atoi(str);
				cout << setw(10) << h
					<< setw(20) << GetMinerSubsidy(h, Params().GetConsensus())
					<< setw(20) << GetBudget(h, Params().GetConsensus())
					<< setw(20) << GetMasternodePayment(h)
					<< setw(20) << GetFoundersReward(h, Params().GetConsensus())
					<< setw(20) << GetBlockSubsidy(h, Params().GetConsensus()) << endl;
			}
		}
		else
			return showreturn("file content set : height=x (multi)");
	}

    return 0;
}

