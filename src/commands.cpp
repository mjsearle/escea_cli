#include "commands.hpp"
#include "udpsender.hpp"
#include "udplistener.hpp"
#include <unistd.h>
#include <iostream>
#include <stdio.h>


bool Status ( void )
{
	auto char n=0;
	auto char response[2]={ 0x0 };
	auto char dsize[2]={ 0x0 };

	auto bool sendstatus=0;
	auto bool receivestatus=0;


        auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };

	while ( ! receivestatus && n < 10 )
	{
        sendstatus=UDPSender::sendCommand (0x31, 0x0, data );
	receivestatus=UDPListener::listenCommand (response, dsize, data );
	n++;
	}

	if ( n > 1 && n < 10) { std::cout << "Note: Took " << std::dec << (unsigned int)(n-1) << " retries to receive status." << std::endl; }
	if ( n == 10) { std::cout << "Failed to receieve status after 9 retries, giving up....  ." << std::endl; }

	std::cout << "Fireplace Status: " << std::endl;  

	if ( response[0] == -128 || response[0] == 128  )
	{
		if (data[0]) {  std::cout << "FirePlace has new timers" 	<< std::endl; }

		if (data[1]) {  std::cout << "FirePlace is ON" 	<< std::endl; }
		else {  std::cout << "FirePlace is OFF" << std::endl; }

		if (data[2]) {  std::cout << "Fan Boost is ON" 	<< std::endl; }
		else {  std::cout << "Fan Boost is OFF" << std::endl; }

		if (data[3]) {  std::cout << "Flame Effect only is ON" 	<< std::endl; }
		else {  std::cout << "Flame Effect only is OFF" << std::endl; }

		std::cout << "Desired temperature is set to: " << std::dec << (unsigned int)(data[4]) << " C" << std::endl;
		std::cout << "Current room temperature is: " << std::dec << (unsigned int)(data[5]) << " C" << std::endl;
	}
	else { std::cout << "FirePlace did not respond to status request" << std::endl; }
		return sendstatus & receivestatus; 	
}


bool NewSetTemp ( char newtemp)
{
 	auto char data[12]={ newtemp,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x57, 0x1, data );
}

bool PowerOn ( void )
{
 	auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x39, 0x0, data );
}

bool PowerOff ( void )
{
 	auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x3A, 0x0, data );
}

bool Fan_Boost ( void )
{
 	auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x37, 0x0, data );
}

bool No_Fan_Boost ( void )
{
 	auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x38, 0x0, data );
}

bool FlameEOn ( void )
{
 	auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x56, 0x0, data );
}

bool FlameEOff ( void )
{
 	auto char data[12]={ 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0 };
    	return UDPSender::sendCommand (0x55, 0x0, data );
}


