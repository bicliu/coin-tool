#include "utils.h"
#include "signtool.h"

#include <iomanip>

using namespace std;

static boost::scoped_ptr<ECCVerifyHandle> globalVerifyHandle;
const int cmdindex = 1;

int main(int argc, char* argv[])
{
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();
	globalVerifyHandle.reset(new ECCVerifyHandle());
	
	// Sanity check no need
    //if (!ECC_InitSanityCheck())
    //    return showreturn(("ECC_InitSanityCheck sanity check failed. Shutting down."));

	if(argc < 2)
	{
		cout << "no cmd \n";
		return -1;
	}

	string strcmd = string(argv[cmdindex]);
	
	if("signmsg" == strcmd)
	{
		SignMsg(argv[cmdindex+1], argv[cmdindex+2]);
	}
	else if("signcheck" == strcmd)
	{
		SignVerify(argv[cmdindex+1], argv[cmdindex+2],argv[cmdindex+3]);
	}
	else if("compactsign" == strcmd)
	{
		CompactSign(argv[cmdindex+1], argv[cmdindex+2]);
	}
	else if("compactverify" == strcmd)
	{
		CompactVerify(argv[cmdindex+1], argv[cmdindex+2],argv[cmdindex+3]);
	}
	else
		cout << "Error: unknown method " << strcmd << endl;

    return 0;
}

