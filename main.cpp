
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>

int gPort = 55555;
int numPlayersConnected = 0;
int main()
{
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		std::cout << "The winsock dll not found!" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "The winsock dll was found!" << "\n" << "The status: " << wsaData.szSystemStatus << std::endl;
	}

	// set up the socket configuration
	SOCKET listener = INVALID_SOCKET;
	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listener == INVALID_SOCKET)
	{
		std::cout << "Error at socket(); " << WSAGetLastError() << std::endl;
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "socket() is OK!" << std::endl;
	}

	// bind the specifics for the socket with its specific configuration
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, L"127.0.0.1", &service.sin_addr.s_addr);
	service.sin_port = htons(gPort);
	if (bind(listener, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		std::cout << "bind failed on listener" << WSAGetLastError() << std::endl;
		closesocket(listener);
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "Socket bound!" << std::endl;
	}

	if (listen(listener, 8) == SOCKET_ERROR)
	{
		std::cout << "listen(): Error listening on socket! " << WSAGetLastError() << std::endl;
		closesocket(listener);
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "Socket now listening for incoming requests on port: " << gPort << std::endl;
	}

	std::cout << "Waiting for client connection" << std::endl;

	SOCKET clientPipeSocket = INVALID_SOCKET;
	SOCKET clientPipeSocket2 = INVALID_SOCKET;
	clientPipeSocket = accept(listener, NULL, NULL);
	if (clientPipeSocket == INVALID_SOCKET)
	{
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		closesocket(listener);
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "Connection established!" << std::endl;
		std::cout << ++numPlayersConnected << " player(s) connected, starting game" << std::endl;
		numPlayersConnected++;
	}
	
	clientPipeSocket2 = accept(listener, NULL, NULL);
	if (clientPipeSocket2 == INVALID_SOCKET)
	{
		std::cout << "accept failed with player 2: " << WSAGetLastError() << std::endl;
		closesocket(clientPipeSocket);
		closesocket(listener);
		WSACleanup();
		return 0;
	}
	else
	{
		std::cout << "Connection established!" << std::endl;
		std::cout << ++numPlayersConnected << "Second player(s) connected, starting game" << std::endl;
		numPlayersConnected++;
	}


	//client connected, detach it on another thread and run that, then set up another thread for listening for other requests to join as long as numPlayersConnected < 4
	// with those two threads running, in a main loop, first check if any data came in on any of those threads
	// if the pipe sent data, get the data and use it to update the game world, then send the updated data back out to the players connected, then once they all got the data successfullly,
	//  invalidate all the data on the pipes, and wait for data to come in on one of the pipes ( or all), then update and broadcast back out and rinse and repeat

	// if client connects, bind the new socket to a new pipe on a new thread and detach it and increase number of players connected and if < 4 create a new listening thread and repeat

	while (true)
	{
		char result;
		std::cout << "Type 'e' to exit: ";
		std::cin >> result;
		if (result == 'e')
			goto loopend;
	}
	loopend:


	// exit program


	//close any open sockets
	if (listener != INVALID_SOCKET)
	{
		closesocket(listener);
	}
	if (clientPipeSocket != INVALID_SOCKET)
	{
		closesocket(clientPipeSocket);
	}
	if (clientPipeSocket2 != INVALID_SOCKET)
	{
		closesocket(clientPipeSocket2);
	}

	WSACleanup();

	return 0;
}

