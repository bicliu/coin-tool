//#include<iostream>
#include "util.h"


using namespace std;

int main(int argc, char* argv[])
{
	map<string, string> mapAlertArgs;
	map<string, vector<string> > mapMultiAlertArgs;

	ReadFile(mapAlertArgs, mapMultiAlertArgs, "alertarg.conf");
	
	return 0;
}

