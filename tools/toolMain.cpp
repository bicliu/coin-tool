#include "utils.h"
#include "alert.h"
#include "walletTool.h"

using namespace std;

int main()
{
    CAlert alert;
	
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();
	ECCVerifyHandle globalVerifyHandle;

	MakeNewKey();

    return 0;
}

