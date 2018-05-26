#include "utils.h"

using namespace std;

bool InitUCenter()
{
    SetFilePath("ucenter.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

    InitSys();
	ECC_Start();
	ECCVerifyHandle globalVerifyHandle;
}