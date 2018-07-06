#include "utils.h"
#include "core_io.h"
#include "policy/policy.h"
#include "txmempool.h"

using namespace std;

CTxMemPool testmempool(CFeeRate(10000));
CKey secret;
CPubKey pubkey;

void GetPrevTX(CTransaction & tx, CAmount fee, unsigned int & nSigOps)
{
	CMutableTransaction rawTx;
	
	//txin
	rawTx.nLockTime = 100;
	uint32_t nSequence = (rawTx.nLockTime ? std::numeric_limits<uint32_t>::max() - 1 : std::numeric_limits<uint32_t>::max());
	CTxIn in(COutPoint(uint256(), 0), CScript(), nSequence);
	rawTx.vin.push_back(in);
	nSigOps += in.scriptSig.GetSigOpCount(false);

	//txout
	//CKey secret;
    secret.MakeNewKey(true);
	/*CPubKey */pubkey = secret.GetPubKey();
	CBitcoinAddress address(pubkey.GetID());
	
	CScript scriptPubKey = GetScriptForDestination(address.Get());
	CAmount nAmount = 1000 * COIN + fee;
	CTxOut out(nAmount, scriptPubKey);
	rawTx.vout.push_back(out);
	nSigOps += out.scriptPubKey.GetSigOpCount(false);
	
	if (!DecodeHexTx(tx, EncodeHexTx(rawTx)))
	{
		cout << "TX decode failed" << endl;
		return;
	}
	//cout << "prev tx hash is " << tx.GetHash().GetHex() << endl;
}

void OutputInfo()
{
	cout << "Test MemPool :" << endl
         << "size : " << (int64_t)testmempool.size()
         << ", bytes : " << (int64_t)testmempool.GetTotalTxSize()
         << ", usage : " << (int64_t)testmempool.DynamicMemoryUsage()
         << ", Max size : " << (int64_t)GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000 << endl
		 << "1 byte minifee : " << testmempool.GetMinFee(GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000).GetFee(1)
		 << ", 1000 minifee : " << testmempool.GetMinFee(GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000).GetFee(1000)
		 << ", 10000 minifee : " << testmempool.GetMinFee(GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000).GetFee(10000) << endl << endl;
}

bool NewTX(const CTransaction & prevtx, unsigned int & nSigOps, CTransaction & newtx, CAmount fee)
{
	CMutableTransaction rawTx;

	//txin
	rawTx.nLockTime = 1000;
    uint32_t nSequence = (rawTx.nLockTime ? std::numeric_limits<uint32_t>::max() - 1 : std::numeric_limits<uint32_t>::max());
    CTxIn in(COutPoint(prevtx.GetHash(), 0), CScript(), nSequence);
    rawTx.vin.push_back(in);
    nSigOps += in.scriptSig.GetSigOpCount(false);

	//txout
	CBitcoinAddress address(pubkey.GetID());
    CScript scriptPubKey = GetScriptForDestination(address.Get());
    CAmount nAmount = 1000 * COIN + fee;
    CTxOut out(nAmount, scriptPubKey);
    rawTx.vout.push_back(out);
    nSigOps += out.scriptPubKey.GetSigOpCount(false);

	if (!DecodeHexTx(newtx, EncodeHexTx(rawTx)))
    {
        cout << "TX decode failed" << endl;
        return false;
    }
	return true;
}

bool IsFull()
{
	return (int64_t)testmempool.GetTotalTxSize() >= (int64_t)GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000;
}

void GetMemPoolInfo(int argc, char* argv[])
{
	if(argc < cmdindex+3)
	{
		cout << "Example: getmempoolinfo 0.0 true" << endl;
		return;
	}
	OutputInfo();
	CTransaction prevtx;
	CAmount fee = 10000;
	unsigned int nSigOps = 0;
	LockPoints lp;
	CTxMemPool::setEntries setAncestors;
	GetPrevTX(prevtx, fee, nSigOps);
	int64_t size = 0;

	double val = atof(argv[cmdindex+1]);
	bool is = atoi(argv[cmdindex+2]) == 1 ? true : false;
	//testmempool.EnableFeeCalc(val);
	/*std::vector<uint256> vNoSpendsRemaining;
    testmempool.TrimToSize(GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000, &vNoSpendsRemaining);

	std::vector<CTransaction> vtx;
	list<CTransaction> txConflicted;
	testmempool.removeForBlock(vtx, 100000, txConflicted, false);*/
	

	while(!IsFull())
	{
		CTransaction tx;
		nSigOps = 0;
		if(!NewTX(prevtx, nSigOps, tx, fee))
			continue;
		
		testmempool.EnableFeeCalc(is, val);
		CTxMemPoolEntry entry(tx, fee, GetTime(), tx.ComputePriority(0.0), 100000, false, (1000*COIN + fee), false, nSigOps, lp);

		size += (int64_t)entry.GetTxSize();

		if(((int64_t)entry.GetTxSize() + (int64_t)testmempool.GetTotalTxSize()) >= (int64_t)GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 1000000)
		{
			cout << "MemPool is Full" << endl;
			break;
		}

		if(size > ((int64_t)GetArg("-maxmempool", DEFAULT_MAX_MEMPOOL_SIZE) * 100000))
		{
			size = 0;
			OutputInfo();
		}
	
		testmempool.addUnchecked(tx.GetHash(), entry, setAncestors, false);
	}
	OutputInfo();
}
