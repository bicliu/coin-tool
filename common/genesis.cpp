#include "utils.h"

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>
#include "arith_uint256.h"
#include "chainparamsseeds.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <string>
#include "utiltime.h"
#include <random>
#include <cmath>
#include <iomanip>
#include <util.h>
#include <new>
#include <chrono>
#include <thread>
#include <mutex>
#include "random.h"

#include <sys/time.h>

typedef uint32_t uint;
typedef CBlockHeader ch;
typedef long long ll;

static std::mutex mtx;

//test cnt 1000 times time
int64_t getCurrentTime()  
{      
   struct timeval tv;      
   gettimeofday(&tv,NULL);   
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;      
}  

// find a genesis in about 10-20 mins
void _get(const ch * const pblock, const arith_uint256 hashTarget)
{
    uint256 hash;
    ch *pb = new ch(*pblock);
	
    for (int cnt = 0, tcnt=0; true; ++cnt,++tcnt)
    {
        uint256 hash = pb->GetHash();

		//std::cout<<"hex hash = "<<hash.GetHex()<<std::endl;
				
        if (UintToArith256(hash) <= hashTarget) break;
        pb->nNonce = ArithToUint256(UintToArith256(pb->nNonce) + 1);
        if (cnt > 1e3)
        {
            pb->nTime = GetTime();
            cnt = 0;
        }
		if (tcnt !=0 and tcnt % 1000 == 0)
        {
            std::cout<<"cryptohello tcnt = "<<tcnt<<" time = "<<getCurrentTime()<<" ms"<<std::endl;       
        }

    }
    
    std::lock_guard<std::mutex> guard(mtx);
    std::cout << "\n\t\t----------------------------------------\t" << std::endl;
    std::cout << "\t" << pb->ToString() << std::endl;
    std::cout << "\n\t\t----------------------------------------\t" << std::endl;
    delete pb;

    // stop while found one
    assert(0);
}

static void findGenesis(CBlockHeader *pb, const std::string &net)
{
    arith_uint256 hashTarget = arith_uint256().SetCompact(pb->nBits);
    std::cout << " finding genesis using target " << hashTarget.ToString()
        << ", " << net << std::endl;

    std::vector<std::thread> threads;

    for (int i = 0; i < std::min(GetNumCores(), 100); ++i)
    //for (int i = 0; i < 1; ++i)
    {
        if (i >= 0)
        {
		// Randomise nonce
        	arith_uint256 nonce = UintToArith256(GetRandHash());
        	// Clear the top and bottom 16 bits (for local use as thread flags and counters)
        	nonce <<= 32;
        	nonce >>= 16;
        	pb->nNonce = ArithToUint256(nonce);
		//std::cout<<"i = "<<i<<"    nNonce = "<<pb->nNonce.ToString()<<std::endl;	
        }
        threads.push_back(std::thread(_get, pb, hashTarget));
    }

    for (auto &t : threads)
    {
        t.join();
    }
}

void GenesisTest()
{
    arith_uint256 nTempBit =  UintToArith256( Params().GetConsensus().powLimit);
    CBlock genesis =CreateGenesisBlock1(1524045652, uint256S("0x01"), nTempBit.GetCompact(), 1, Params().GetConsensus().genesisReward);

    arith_uint256 a("0x000009b173000000000000000000000000000000000000000000000000000000");
    cout << "\tpow:\t" << a.GetCompact()  << " "<< nTempBit.GetCompact() << endl;
    findGenesis(&genesis, "main");
    cout <<"hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() << endl
        << "hashGenesisBlock: " << genesis.GetHash() << endl;
}