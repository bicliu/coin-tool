#include "utils.h"
#include "rewardtool.h"
#include "main.h"
#include "governance-classes.h"

#include <iomanip>

using namespace std;

void AddReward(const CAmount value, CAmount & coin, CAmount & small)
{
	coin += (value / COIN);
	small += (value % COIN);
	if(small > COIN)
	{
		coin += (small/COIN);
		small = small%COIN;
	}
}

void RewardStatistics(int iyears, int isum)
{
	CAmount aSumMr = 0, asmrsmall = 0;
	CAmount aSumBd = 0, asbdsmall = 0;
	CAmount aSumMn = 0, asmnsmall = 0;
	CAmount aSumFr = 0, asfrsmall = 0;
	CAmount aSumbk = 0, asbksmall = 0;
	int lasth = 0;
	cout << endl << "The rewards in every "<< iyears << " years from 1st to " << GetRankString(isum) << " :" <<endl
		<< "year         height range              MinerSubsidy           Budget        MasternodePayment      FoundersReward       BlockSubsidy" <<endl;
	for(int i = 1; i < isum+1; i++)
	{
		CAmount aMiner = 0, aMinersmall = 0;
		CAmount aBud = 0, aBudsmall = 0;
		CAmount aMnode = 0, aMnodesmall = 0;
		CAmount aFounders = 0, aFoundersmall = 0;
		CAmount aBlocks = 0, aBlockssmall = 0;
		int iHl = (i - 1) * 210240 * iyears;
		int iHh = i * 210240 * iyears;
		for (int h = iHl; h < iHh; h ++)
		{
			AddReward(GetMinerSubsidy(h, Params().GetConsensus()), aMiner, aMinersmall);
			AddReward(GetMasternodePayment(h), aMnode, aMnodesmall);
			if(CSuperblockManager::IsSuperblockTriggered(h))
			{
				AddReward(GetFoundersReward(h, Params().GetConsensus()), aFounders, aFoundersmall);
				//if(CSuperblockManager::IsSuperblockVoteTriggered(h))
				AddReward(GetBudget(h, Params().GetConsensus()), aBud, aBudsmall);
			}
		}
		aBlocks += (aMiner + aBud + aMnode + aFounders);
        AddReward((aMinersmall + aBudsmall + aMnodesmall + aFoundersmall), aBlocks, aBlockssmall);
		aSumMr += aMiner;
		aSumBd += aBud;
		aSumMn += aMnode;
		aSumFr += aFounders;
		asmrsmall += aMinersmall;
		asbdsmall += aBudsmall;
		asmnsmall += aMnodesmall;
		asfrsmall += aFoundersmall;
		lasth = iHh - 1;
		cout<< setw(2) << i << " x" << iyears << "     " << setw(9) << iHl << "--" << setw(9)  << (iHh - 1)
			<< setw(20) << aMiner << setw(20) << aBud
			<< setw(20) << aMnode << setw(20) << aFounders
			<< setw(20) << aBlocks << endl;
	}
	aSumbk = (aSumMr + aSumBd + aSumMn + aSumFr);
    AddReward((asmrsmall + asbdsmall + asmnsmall + asfrsmall), aSumbk, asbksmall);
	cout << "total     " << setw(9) << 0 << "--" << setw(9)  << lasth                                                                                                                                                                             
        << setw(20) << aSumMr << setw(20) << aSumBd
        << setw(20) << aSumMn << setw(20) << aSumFr
        << setw(20) << aSumbk << endl;
}

void RewardHandle()
{
	if (mapArgs.count("-height"))
	{
		cout << "    height      MinerSubsidy           Budget        MasternodePayment     FoundersReward       BlockSubsidy" << endl;
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
	
	RewardStatistics(1, 4);
	RewardStatistics(4, 5);
}

void BlockRewardHelp()
{
	cout << "Command \"blockreward\" example :" << endl << endl
        << "blockreward blockheight" << endl << endl;
}

void BlockReward(int h)
{
	CAmount budget = 0;
	CAmount founders = 0;
	if(CSuperblockManager::IsSuperblockTriggered(h))
	{
		budget = GetBudget(h, Params().GetConsensus());
		founders = GetFoundersReward(h, Params().GetConsensus());
	}
	cout << "    height      MinerSubsidy           Budget        MasternodePayment     FoundersReward       BlockSubsidy" << endl;
	cout << setw(10) << h
		<< setw(20) << (float)(GetMinerSubsidy(h, Params().GetConsensus()) / COIN)
		<< setw(20) << (float)(budget / COIN)
		<< setw(20) << (float)(GetMasternodePayment(h) / COIN)
		<< setw(20) << (float)(founders / COIN)
		<< setw(20) << (float)(GetBlockSubsidy(h, Params().GetConsensus()) / COIN) << endl;
	
	return;
}

void RewardStatisticsHelp()
{
	cout << "Command \"rewardstatistics\" example :" << endl << endl
        << "rewardstatistics years count" << endl << endl;
}

void RewardExampleHelp()
{
	cout << "Command \"rewardexample\" example :" << endl << endl
        << "rewardexample filename" << endl << endl;
}

void RewardExample(const std::string & filename)
{
	if(!ReadCurrentFile(mapArgs, mapMultiArgs, filename))
	{
		cout << "read file " << filename << " failed" << endl;
		return;
	}
	RewardHandle();
	return;
}
