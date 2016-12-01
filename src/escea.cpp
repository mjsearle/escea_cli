/* Escea Gas Fireplace Linux CLI.  Github: mjsearle. 2016 
Adapted from code "milight" by iqjar 2013. http://iqjar.com/download/jar/milight/milight_sources.zip    */

#include "udpsender.hpp"
#include "commands.hpp"
#include "utils.hpp"
#include <iostream>
#include <string>

using namespace std;

void printAbout (const std::string& programName)
{
	cout << "Escea Fire Command Line Utility. " << endl;
	cout << "Usage: " << programName << " COMMAND [PARAMETER]" << endl;
	cout << "Supported Commands:" << endl;
	cout << "STATUS Get Fireplace Status" << endl;
	cout << "SETTEMP [TEMP] Set Fire Desired Temperature between 3C and 31C" << endl;
	cout << "ON  Switch Fire On" << endl;
	cout << "OFF  Switch Fire Off" << endl;
	cout << "FANB  Boost Fan Speed" << endl;
	cout << "FANB_OFF  Stop boost Fan Speed" << endl;
	cout << "FLAME_ONLY Enable Flame Effect Only Mode" << endl;
	cout << "HEAT Enable full Heating Mode" << endl;
	cout << "SERIAL Get Fire Serial Number and Pin" << endl;
	cout <<endl;
}

bool processCommand (const string& programName, const string& cmdStr, const string& paramStr)
{
	if (cmdStr == "STATUS")
	{
		return Status ();
	}

	if (cmdStr == "SETTEMP")
	{
		int settemp = 0;
		if (paramStr.empty () || (!Utils::stoi (paramStr, &settemp)) ||
			(settemp < 3) || (settemp > 31))
                {
                        printAbout (programName);
                        return false;
                }

		return NewSetTemp (settemp);
	}

	if (cmdStr == "ON")
	{
		return PowerOn ();
	}

	if (cmdStr == "OFF")
	{
		return PowerOff ();
	}

	if (cmdStr == "FANB")
	{
		return Fan_Boost ();
	}

	if (cmdStr == "FANB_OFF")
	{
		return No_Fan_Boost ();
	}

	if (cmdStr == "FLAME_ONLY")
	{
		return FlameEOn ();
	}

	if (cmdStr == "HEAT")
	{
		return FlameEOff ();
	}

	if (cmdStr == "SERIAL")
        {
                return GetSerial ();
        }



	printAbout (programName);
	return false;
}


int main (int argc, const char* argv[])
{
	if ((argc < 2) || (argc > 3))
	{
		printAbout (argv[0]);
		return 1;
	}

	string arg1 = Utils::toUpper (argv[1]);
	string arg2;
	if (argc > 2)
	{
		arg2 = Utils::toUpper (argv[2]);
	}



	bool result = processCommand (argv[0], arg1, arg2);
	return (result ? 0 : 1);
}
