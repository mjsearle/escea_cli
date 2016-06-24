#include "utils.hpp"
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <assert.h>
#include <algorithm>
#include <cctype>


std::string Utils::toUpper (const std::string& str)
{
        std::string retVal = str;
        toUpper (retVal);
        return retVal;
}

void Utils::toUpper (std::string& str)
{
        std::transform (str.begin (), str.end (), str.begin (), toupper);
}

std::string Utils::truncAtNewLine (const std::string& str)
{
	std::string s = str;

	std::string::size_type pos = s.find ("\n");
	if (pos != std::string::npos)
	{
		s = s.substr (0, pos);
	}

	return s;
}

bool Utils::stoi (const std::string& s, int* i)
{
        assert (i != NULL);
        const char* str = s.c_str ();
        char* endPtr = const_cast<char*> (str);
        *i = strtol (str, &endPtr, 10);
        bool isNumber = ((endPtr == NULL) || (*endPtr == 0));
        if (!isNumber)
        {
                *i = 0;
        }

        return isNumber;
}

std::string Utils::getProgramDir ()
{
        char programDir[1024];
        programDir[0] = 0;
        if (readlink ("/proc/self/exe", programDir, 1024) != -1)
        {
                dirname (programDir);
                strcat  (programDir, "/");
        }

        return programDir;
}
