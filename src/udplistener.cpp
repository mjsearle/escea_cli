#include "udplistener.hpp"
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
#include <netdb.h>



const char* DefaultAddress_Listen = "192.168.1.11";
const int DefaultPort_Listen = 3300;
const char* ConfigFileName_Listen = "escea.conf";
char portstring[5]="";


bool UDPListener::readConfig (const std::string& configFileName)
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
						portstring[0]=portStr[0];
						portstring[1]=portStr[1];
						portstring[2]=portStr[2];
						portstring[3]=portStr[3];

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
					std::cout << "Failed to read port number from " << configFileName << ". Using default port (" << DefaultPort_Listen << ")." << std::endl;
				}
                        }
			else
			{
				std::cout << "Failed to read address from " << configFileName << ". Using default address (" << DefaultAddress_Listen << ")." << std::endl;
			}
                }
                else
                {
                        std::cout << "Unable to open " << configFileName << " for reading!" << std::endl;
			std::cout << "Using default address (" << DefaultAddress_Listen << ") and port ("  << DefaultPort_Listen << ")." << std::endl;
                }
        }
        catch (...)
        {
                //Swallow all exceptions
                retVal = false;
        }

        return retVal;
}


bool UDPListener::listenCommand (char *response, char *dsize, char *data )
{
        UDPListener udpListener (Utils::getProgramDir () + ConfigFileName_Listen);
        bool result = udpListener.listen (response, dsize, data);
        if (!result)
        {
    //            std::cout << "Failed to listen for repsonse from fireplace" << std::endl;
        }

        return result;
}

UDPListener::UDPListener ():
	mAddress (DefaultAddress_Listen),
	mPort (DefaultPort_Listen),
	mVerbose (false)
{
}

UDPListener::UDPListener (const std::string& address, unsigned int port, bool verbose):
	mAddress (address),
	mPort (port),
	mVerbose (verbose)
{
}

UDPListener::UDPListener (const std::string& configFileName):
	mAddress (DefaultAddress_Listen),
	mPort (DefaultPort_Listen),
	mVerbose (false)
{
	readConfig (configFileName);
}

UDPListener::UDPListener (const UDPListener& other):
	mAddress (other.mAddress),
        mPort (other.mPort),
	mVerbose (other.mVerbose)
{
}

UDPListener::~UDPListener ()
{
}

UDPListener& UDPListener::operator= (const UDPListener& other)
{
	mAddress = other.mAddress;
        mPort = other.mPort;
	mVerbose = other.mVerbose;

	return *this;
}

bool UDPListener::listen ( char *response, char *dsize, char *data)
{
	bool retVal = false;

        int udpSocket = socket (AF_INET, SOCK_DGRAM, 0);
        if (udpSocket >= 0)
	{
		auto char n=0;
		auto unsigned char crc=0; 
		auto char raw_response[15];

		const char* hostname=0; /* wildcard, receive datagrams directed at any address */
		const char* portname=portstring;
		struct addrinfo hints;
		memset(&hints,0,sizeof(hints));
		hints.ai_family=AF_UNSPEC;
		hints.ai_socktype=SOCK_DGRAM;
		hints.ai_protocol=0;
		hints.ai_flags=AI_PASSIVE|AI_ADDRCONFIG;
		struct addrinfo* res=0;
		int err=getaddrinfo(hostname,portname,&hints,&res);

		bind(udpSocket,res->ai_addr,res->ai_addrlen);

		struct timeval tv;
		tv.tv_sec = 2;  // 2 Secs Timeout 
		setsockopt(udpSocket, SOL_SOCKET, SO_RCVTIMEO,(struct timeval *)&tv,sizeof(struct timeval));  

		struct sockaddr_storage src_addr;
		socklen_t src_addr_len=sizeof(src_addr);

//		std::cout << "Gets to here" << std::endl; 
	        int bytesRecv = recvfrom (udpSocket, raw_response, 15, 0, (struct sockaddr*)&src_addr, &src_addr_len );
//		std::cout << "Gets to here after" << std::endl; 

	        if (bytesRecv >= 0)
		{
			retVal = true;

			if (mVerbose)
			{
				std::cout << "Received UDP Datagram"
					<< " 0x" << std::hex << (unsigned int)(raw_response[0])
					<< " 0x" << std::hex << (unsigned int)(raw_response[1])
					<< " 0x" << std::hex << (unsigned int)(raw_response[2])
					<< " 0x" << std::hex << (unsigned int)(raw_response[3])
					<< " 0x" << std::hex << (unsigned int)(raw_response[4])
					<< " 0x" << std::hex << (unsigned int)(raw_response[5])
					<< " 0x" << std::hex << (unsigned int)(raw_response[6])
					<< " 0x" << std::hex << (unsigned int)(raw_response[7])
					<< " 0x" << std::hex << (unsigned int)(raw_response[8])
					<< " 0x" << std::hex << (unsigned int)(raw_response[9])
					<< " 0x" << std::hex << (unsigned int)(raw_response[10])
					<< " 0x" << std::hex << (unsigned int)(raw_response[11])
					<< " 0x" << std::hex << (unsigned int)(raw_response[12])
					<< " 0x" << std::hex << (unsigned int)(raw_response[13])
					<< " 0x" << std::hex << (unsigned int)(raw_response[14])
					<< " from " << mAddress << ":" << std::dec << mPort << std::endl;
			}
			response[0]=raw_response[1];
			dsize[0]=raw_response[2];
			for(n=0;n<11;n++) 
			{
				data[n]=raw_response[n+3];
			}
		}
		else
		{
			if (mVerbose)
	                {
        	                std::cout << "Failed to receive data: " << strerror (errno) << std::endl;
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
