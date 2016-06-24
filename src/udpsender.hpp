#if !defined UDP_SENDER_HPP
#define UDP_SENDER_HPP

#include <string>

class UDPSender
{
private:
	std::string mAddress; //Address (IP or domain name) of the machine to which the messages are sent
	unsigned int mPort; //Port number to which the messages are sent
	bool mVerbose; //Enable messages

	bool readConfig (const std::string& configFileName);

public:
	static bool sendCommand (char cmd, char dsize, char *data);

	UDPSender ();
	UDPSender (const std::string& ip, unsigned int port, bool verbose);
	UDPSender (const std::string& configFileName);
	UDPSender (const UDPSender& other);
	~UDPSender ();
	UDPSender& operator= (const UDPSender& other);

	bool send (char cmd, char dsize, char *data);
};

#endif
