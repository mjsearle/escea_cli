#if !defined UTILS_HPP
#define UTILS_HPP

#include <string>

namespace Utils
{
	std::string toUpper (const std::string& str);
        void toUpper (std::string& str);
	std::string truncAtNewLine (const std::string& str);
	bool stoi (const std::string& s, int* i);
	std::string getProgramDir ();
}

#endif

