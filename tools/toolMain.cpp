#include "utils.h"
//#include "walletTool.h"

int main()
{
    SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();
	ECCVerifyHandle globalVerifyHandle;

//    MakeNewKey();
}
