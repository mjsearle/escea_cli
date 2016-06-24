#include "udpsender.hpp"
#include "utils.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>


const char* DefaultAddress = "192.168.1.11";
const int DefaultPort = 3300;
const char* ConfigFileName = "escea.conf";


bool UDPSender::readConfig (const std::string& configFileName)
{
	bool retVal = false;

        try
        {
                FILE* f = fopen (configFileName.c_str (), "rt");
                if (f != NULL)
                {
                        char line[256];
                        if (fgets (line, 256, f) != NULL)
                        {
                                mAddress = Utils::truncAtNewLine (line);
				if (fgets (line, 256, f) != NULL)
                	        {
					std::string portStr = Utils::truncAtNewLine (line);
					int portNum = 0;
        	                        if (Utils::stoi (portStr, &portNum))
	                                {
						mPort = static_cast<unsigned int> (portNum);
						if (fgets (line, 256, f))
						{
							std::string debugStr = Utils::toUpper (Utils::truncAtNewLine (line));
							mVerbose = (debugStr == "VERBOSE");
                                        		retVal = true;
						}
                                	}
                        	}
				else
				{
					std::cout << "Failed to read port number from " << configFileName << ". Using default port (" << DefaultPort << ")." << std::endl;
				}
                        }
			else
			{
				std::cout << "Failed to read address from " << configFileName << ". Using default address (" << DefaultAddress << ")." << std::endl;
			}
                }
                else
                {
                        std::cout << "Unable to open " << configFileName << " for reading!" << std::endl;
			std::cout << "Using default address (" << DefaultAddress << ") and port ("  << DefaultPort << ")." << std::endl;
                }
        }
        catch (...)
        {
                //Swallow all exceptions
                retVal = false;
        }

        return retVal;
}

bool UDPSender::sendCommand (char cmd, char dsize, char *data )
{
        UDPSender udpSender (Utils::getProgramDir () + ConfigFileName);
        bool result = udpSender.send (cmd, dsize, data);
        if (!result)
        {
                std::cout << "Failed to send command to fireplace" << std::endl;
        }

        return result;
}

UDPSender::UDPSender ():
	mAddress (DefaultAddress),
	mPort (DefaultPort),
	mVerbose (false)
{
}

UDPSender::UDPSender (const std::string& address, unsigned int port, bool verbose):
	mAddress (address),
	mPort (port),
	mVerbose (verbose)
{
}

UDPSender::UDPSender (const std::string& configFileName):
	mAddress (DefaultAddress),
	mPort (DefaultPort),
	mVerbose (false)
{
	readConfig (configFileName);
}

UDPSender::UDPSender (const UDPSender& other):
	mAddress (other.mAddress),
        mPort (other.mPort),
	mVerbose (other.mVerbose)
{
}

UDPSender::~UDPSender ()
{
}

UDPSender& UDPSender::operator= (const UDPSender& other)
{
	mAddress = other.mAddress;
        mPort = other.mPort;
	mVerbose = other.mVerbose;

	return *this;
}

bool UDPSender::send (char cmd, char dsize, char *data)
{
	bool retVal = false;

        int udpSocket = socket (AF_INET, SOCK_DGRAM, 0);
        if (udpSocket >= 0)
	{
		auto char n=0;
		auto unsigned char crc=0; 
		char command[15];
		command[0]= 0x47;
		command[1]= cmd;
		command[2]= dsize;
		for(n=0;n<11;n++) 
		{
			command[n+3]=data[n];
		}
		// calc CRC value
		for(n=1;n<13;n++) 
		{
			crc=crc+command[n];
		}

		command[13]=crc; 
		command[14]= 0x46;



		struct sockaddr_in destSockAddr;
	        memset (&destSockAddr, 0, sizeof (destSockAddr));
        	destSockAddr.sin_family = AF_INET;
	        destSockAddr.sin_port = htons (mPort);
        	destSockAddr.sin_addr.s_addr = inet_addr (mAddress.c_str ());

	        int bytesSent = sendto (udpSocket, command, 15, 0, (const struct sockaddr*)&destSockAddr, sizeof (destSockAddr));
	        if (bytesSent >= 0)
		{
			retVal = true;

			if (mVerbose)
			{
				std::cout << "Sent UDP command"
					<< " 0x" << std::hex << (unsigned int)(command[0])
					<< " 0x" << std::hex << (unsigned int)(command[1])
					<< " 0x" << std::hex << (unsigned int)(command[2])
					<< " 0x" << std::hex << (unsigned int)(command[3])
					<< " 0x" << std::hex << (unsigned int)(command[4])
					<< " 0x" << std::hex << (unsigned int)(command[5])
					<< " 0x" << std::hex << (unsigned int)(command[6])
					<< " 0x" << std::hex << (unsigned int)(command[7])
					<< " 0x" << std::hex << (unsigned int)(command[8])
					<< " 0x" << std::hex << (unsigned int)(command[9])
					<< " 0x" << std::hex << (unsigned int)(command[10])
					<< " 0x" << std::hex << (unsigned int)(command[11])
					<< " 0x" << std::hex << (unsigned int)(command[12])
					<< " 0x" << std::hex << (unsigned int)(command[13])
					<< " 0x" << std::hex << (unsigned int)(command[14])
					<< " to " << mAddress << ":" << std::dec << mPort << std::endl;
			}
		}
		else
		{
			if (mVerbose)
	                {
        	                std::cout << "Failed to send data: " << strerror (errno) << std::endl;
	                }
		}

	        close (udpSocket);
	}
	else
	{
		if (mVerbose)
		{
			std::cout << "Failed to open UDP socket: " << strerror (errno) << std::endl;
		}
	}

	return retVal;
}
