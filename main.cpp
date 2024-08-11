
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>


struct PlayerData
{
	int xpos{};
	int ypos{};
};

struct BallData
{
	int xpos{};
	int ypos{};
};

struct FrameData
{
	int ypos{};
};

struct WorldData
{
	int yourYPos{};
	int otherYPos{};
	int ballXPos{};
	int ballYPos{};
};

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


	/// send the clients their clientIDs
	char buffer1[2] = { '1','\0' };
	char buffer2[2] = { '2','\0' };
	int byteCount1 = send(clientPipeSocket, buffer1, 2, 0);
	if (byteCount1 == SOCKET_ERROR)
	{
		printf("Server send error %d.\n", WSAGetLastError());
		return -1;
	}
	else
	{
		printf("Server: sent %ld bytes \n to player 1", byteCount1);
	}
	int byteCount2 = send(clientPipeSocket2, buffer2, 2, 0);
	if (byteCount2 == SOCKET_ERROR)
	{
		printf("Server send error %d.\n", WSAGetLastError());
		return -1;
	}
	else
	{
		printf("Server: sent %ld bytes \n to player 2", byteCount2);
	}

	//message "3803800785435" -> out
	// message "380"  <- in  for each client


	//client connected, detach it on another thread and run that, then set up another thread for listening for other requests to join as long as numPlayersConnected < 4
	// with those two threads running, in a main loop, first check if any data came in on any of those threads
	// if the pipe sent data, get the data and use it to update the game world, then send the updated data back out to the players connected, then once they all got the data successfullly,
	//  invalidate all the data on the pipes, and wait for data to come in on one of the pipes ( or all), then update and broadcast back out and rinse and repeat

	// if client connects, bind the new socket to a new pipe on a new thread and detach it and increase number of players connected and if < 4 create a new listening thread and repeat

	// int send ( SOCKET, const char* buf //MESSAGE//, int len //Length of Message Array(individual characters)//, int flags //DEFAULT OK- pass 0//); 
	//  returns number of bytes sent, non-zero is ideal here
	PlayerData player1{};
	PlayerData player2{};
	FrameData inData{};
	WorldData outData{};
	
	
	
	while (true)
	{
		int readBytes = 0;
		char readBuffer[4];
		readBytes = recv(clientPipeSocket, readBuffer, 4, 0);
		if (readBytes == SOCKET_ERROR)
		{
			//error
			goto loopend;
		}

		int readBytes2 = 0;
		char readBuffer2[4];
		readBytes2 = recv(clientPipeSocket2, readBuffer2, 4, 0);
		if (readBytes2 == SOCKET_ERROR)
		{
			//error
			goto loopend;

		}

		// Update world here
		std::string tmp = "000";
		tmp.at(0) = readBuffer[0];
		tmp.at(1) = readBuffer[1];
		tmp.at(2) = readBuffer[2];

		int newYPos = stoi(tmp);
		player1.ypos = newYPos;

		std::string tmp2 = "000";
		tmp2.at(0) = readBuffer2[0];
		tmp2.at(1) = readBuffer2[1];
		tmp2.at(2) = readBuffer2[2];

		int newYPos2 = stoi(tmp2);
		player2.ypos = newYPos2;
		// package updated data into message string and add it to the write buffer


		int sentBytes = 0;
		char writeBuffer[14] = { tmp[0],tmp[1],tmp[2],tmp2[0],tmp2[1],tmp2[2],'0','7','8','5','4','3','5','\0'};
	    sentBytes = send(clientPipeSocket, writeBuffer, 14, 0);
		if (sentBytes == SOCKET_ERROR)
		{
			//error
			goto loopend;

		}

		int sentBytes2 = 0;
		char writeBuffer2[14] = { tmp2[0],tmp2[1],tmp2[2],tmp[0],tmp[1],tmp[2],'0','7','8','5','4','3','5','\0' };
		sentBytes2 = send(clientPipeSocket2, writeBuffer2, 14, 0);
		if (sentBytes2 == SOCKET_ERROR)
		{
			//error
			goto loopend;

		}

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

