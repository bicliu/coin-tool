#include "utils.h"

using namespace std;

bool InitUCenter()
{
    SetFilePath("ucenter.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

    SetParams();
	ECC_Start();
	ECCVerifyHandle globalVerifyHandle;
}