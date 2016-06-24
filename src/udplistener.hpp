#if !defined UDP_LISTENER_HPP
#define UDP_LISTENER_HPP

#include <string>

class UDPListener
{
private:
	std::string mAddress; //Address (IP or domain name) of the machine from which the messages are received
	unsigned int mPort; //Port number to which the messages are received
	bool mVerbose; //Enable messages

	bool readConfig (const std::string& configFileName);

public:
	static bool listenCommand (char *response, char *dsize, char *data);

	UDPListener ();
	UDPListener (const std::string& ip, unsigned int port, bool verbose);
	UDPListener (const std::string& configFileName);
	UDPListener (const UDPListener& other);
	~UDPListener ();
	UDPListener& operator= (const UDPListener& other);

	bool listen (char *response, char *dsize, char *data);
};

#endif
