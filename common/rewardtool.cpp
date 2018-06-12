#include "utils.h"
#include "rewardtool.h"
#include "main.h"
#include "policy/policy.h"
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
	const Consensus::Params cp = Params().GetConsensus();
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
			if(1 == h)
				AddReward(GetMinerSubsidy(h, cp), aFounders, aFoundersmall);
			else
				AddReward(GetMinerSubsidy(h, cp), aMiner, aMinersmall);

			AddReward(GetMasternodePayment(h), aMnode, aMnodesmall);
			if(CSuperblockManager::IsSuperblockTriggered(h))
			{
				AddReward(GetFoundersReward(h, cp), aFounders, aFoundersmall);
				//if(CSuperblockManager::IsSuperblockVoteTriggered(h))
				if(h >= cp.nMasternodePaymentsStartBlock)
					AddReward(GetBudget(h, cp), aBud, aBudsmall);
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
		cout << "    height        MinerSubsidy              Budget    MasternodePayment     FoundersReward        BlockSubsidy" << endl;
		for(string str : mapMultiArgs["-height"])
		{
			int h = atoi(str);
			bool IsSuper = CSuperblockManager::IsSuperblockTriggered(h);
			cout << setw(10) << h
				<< setw(20) << GetMinerSubsidy(h, Params().GetConsensus()) / COIN
				<< setw(20) << (IsSuper ? GetBudget(h, Params().GetConsensus()) / COIN : 0)
				<< setw(20) << GetMasternodePayment(h) / COIN
				<< setw(20) << (IsSuper ? GetFoundersReward(h, Params().GetConsensus()) / COIN : 0)
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

void BlockReward(int argc, char* argv[])
{
	CAmount budget = 0;
	CAmount founders = 0;
	CAmount miner = 0;
	CAmount mstnd = 0;

	if(argc < cmdindex+2)
	{
		BlockRewardHelp();
		return;
	}
	int h = atoi(argv[cmdindex+1]);

	if(CSuperblockManager::IsSuperblockTriggered(h))
	{
		if(h >= Params().GetConsensus().nMasternodePaymentsStartBlock)
			budget = GetBudget(h, Params().GetConsensus());
		founders = GetFoundersReward(h, Params().GetConsensus());
	}
	miner = GetMinerSubsidy(h, Params().GetConsensus());
	mstnd = GetMasternodePayment(h);
	cout << "    height        MinerSubsidy              Budget    MasternodePayment     FoundersReward        BlockSubsidy" << endl;
	cout << setw(10) << h
		<< setw(20) << miner / COIN
		<< setw(20) << budget / COIN
		<< setw(20) << mstnd / COIN
		<< setw(20) << founders / COIN
		<< setw(20) << (budget + founders + miner + mstnd) / COIN << endl;
	
	return;
}

void RewardStatisticsHelp()
{
	cout << "Command \"rewardstatistics\" example :" << endl << endl
        << "rewardstatistics years count" << endl << endl;
}

void RewardStatisticsHandle(int argc, char* argv[])
{
	if(argc < cmdindex+3)
		{
			RewardStatisticsHelp();
			return;
		}
	RewardStatistics(atoi(argv[cmdindex+1]),atoi(argv[cmdindex+2]));
}

void RewardExampleHelp()
{
	cout << "Command \"rewardexample\" example :" << endl << endl
        << "rewardexample filename" << endl << endl;
}

void RewardExample(int argc, char* argv[])
{
	if(argc < cmdindex+2)
	{
		RewardExampleHelp();
		return;
	}
	
	if(!ReadCurrentFile(mapArgs, mapMultiArgs, argv[cmdindex+1]))
	{
		cout << "read file " << argv[cmdindex+1] << " failed" << endl;
		return;
	}
	RewardHandle();
	return;
}

CTxMemPool g_mempool(::CFeeRate(DEFAULT_MIN_RELAY_TX_FEE));
void MemPoolFeeHelp()
{
	cout << "Command \"rewardexample\" example :" << endl << endl
        << "rewardexample filename" << endl << endl;
}
void MemPoolFee(int argc, char* argv[])
{
	if(argc < cmdindex+2)
	{
		MemPoolFeeHelp();
		return;
	}

	uint nSize = atoi(argv[cmdindex+1]);

	CAmount mempoolRejectFee = g_mempool.GetMinFee(GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000).GetFee(nSize);

	cout << "Min MemPoolFee is " << mempoolRejectFee << endl;
	return;
}