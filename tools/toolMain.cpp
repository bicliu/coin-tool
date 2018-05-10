#include "utils.h"
#include "main.h"

#include <iomanip>

using namespace std;

void YearsReward(int iyears)
{
	cout << endl << "The rewards in every "<< iyears << " from 1st to 4th :" <<endl
		<< "year    height range        MinerSubsidy           Budget        MasternodePayment     FoundersReward       BlockSubsidy" <<endl;
	for(int i = 1; i < 5; i++)
	{
		CAmount aMiner = 0;
		CAmount aBud = 0;
		CAmount aMnode = 0;
		CAmount aFounders = 0;
		CAmount aBlocks = 0;
		int iHl = (i - 1) * 210240 * iyears;
		int iHh = i * 210240 * iyears;
		for (int h = iHl; h < iHh; h ++)
		{
			aMiner += GetMinerSubsidy(h, Params().GetConsensus());
			aBud += GetBudget(h, Params().GetConsensus());
			aMnode += GetMasternodePayment(h);
			aFounders += GetFoundersReward(h, Params().GetConsensus());
			aBlocks += (aMiner + aBud + aMnode + aFounders);

			cout << i << " x" << iyears <<setw(9) << iHl << "--" << (iHh - 1)
				<< setw(20) << aMiner << setw(20) << aBud
				<< setw(20) << aMnode << setw(20) << aFounders
				<< setw(20) << aBlocks << endl;
		}
	}
}

void RewardHandle()
{
	if (mapArgs.count("-height"))
	{
		cout << "  height      MinerSubsidy           Budget        MasternodePayment     FoundersReward       BlockSubsidy" << endl;
		for(string str : mapMultiArgs["-height"])
		{
			int h = atoi(str);
			cout << setw(10) << h
				<< setw(20) << GetMinerSubsidy(h, Params().GetConsensus()) / COIN
				<< setw(20) << GetBudget(h, Params().GetConsensus()) / COIN
				<< setw(20) << GetMasternodePayment(h) / COIN
				<< setw(20) << GetFoundersReward(h, Params().GetConsensus()) / COIN
				<< setw(20) << GetBlockSubsidy(h, Params().GetConsensus()) / COIN << endl;
		}
	}
	
	YearsReward(1);
	YearsReward(4);
}

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

