#include "utils.h"

using namespace std;

int main(int argc, char* argv[])
{
	SetFilePath("tool.conf");
	ReadConfigFile(mapArgs, mapMultiArgs);

	SetParams();
	ECC_Start();

	if(2 != argc)
		return showreturn("command as:./tools filename");

	ReadFile(mapArgs, mapMultiArgs, string(argv[argc - 1]));

	if(mapArgs.count("-toolcommand"))
	{
		cout << "command is " << mapArgs["-toolcommand"];
		//to do command
	}
	else
	{
		return showreturn("File without command");
	}

    return 0;
}

