//#include<iostream>
#include "net.h"
#include "chainparams.h"


#include<string> 

using namespace std;

int main(int argc, char* argv[])
{
	CChainParams *pCurrentParams = 0;

	if(argc < 2)
	{
		cout << "args error number is " << argc << endl;
	}

	string strnetpara = string(argv[1]);

	pCurrentParams = &Params(strnetpara);

	return 0;
}

