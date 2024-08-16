
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>
#include <interrelated.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
sf::Vector2f p1Pos{ 30.f,700.f };
sf::Vector2f p2Pos{ 150.f,700.f };




//
//struct PlayerData
//{
//	int xpos{};
//	int ypos{};
//};
//
//struct BallData
//{
//	int xpos{};
//	int ypos{};
//};
//
//struct FrameData
//{
//	int ypos{};
//};
//
//struct WorldData
//{
//	int yourYPos{};
//	int otherYPos{};
//	int ballXPos{};
//	int ballYPos{};
//};

int gPort = 55555;
int numPlayersConnected = 0;
//
//int ballDirX = -1;
//int ballDirY = 1;
//int xSpeed = 0;
//int ySpeed = 0;
//
//PlayerData player1{};
//PlayerData player2{};
//FrameData inData{};
//WorldData outData{};
//BallData ball{};
//

ClientFrameInput p1Input{};
ClientFrameInput p2Input{};

void update();

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

   // Get the hostname of the current machine
char hostname[256];
if (gethostname(hostname, sizeof(hostname)) != 0) {
	std::cout << "gethostname failed" << std::endl;
	WSACleanup();
	return 0;
}

// Get the IP address of the current machine
struct addrinfo hints, * result, * ptr;
ZeroMemory(&hints, sizeof(hints));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;

if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
	std::cout << "getaddrinfo failed" << std::endl;
	WSACleanup();
	return 0;
}

sockaddr_in service;
for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
	if (ptr->ai_family == AF_INET) {
		service.sin_family = AF_INET;
		service.sin_addr = ((struct sockaddr_in*)ptr->ai_addr)->sin_addr;
		service.sin_port = htons(gPort);
		break;
	}
}

freeaddrinfo(result);

// bind the specifics for the socket with its specific configuration
if (bind(listener, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
	std::cout << "bind failed on listener" << WSAGetLastError() << std::endl;
	closesocket(listener);
	WSACleanup();
	return 0;
}
else {
	std::cout << "Socket bound!" << std::endl;
}

if (listen(listener, 8) == SOCKET_ERROR) {
	std::cout << "listen(): Error listening on socket! " << WSAGetLastError() << std::endl;
	closesocket(listener);
	WSACleanup();
	return 0;
}
else {
	std::cout << "Socket now listening for incoming requests on port: " << gPort << std::endl;
}

std::cout << "Waiting for client connection" << std::endl;

	SOCKET clientPipeSocket = INVALID_SOCKET;
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
	
	SOCKET clientPipeSocket2 = INVALID_SOCKET;
	clientPipeSocket2 = accept(listener, NULL, NULL);
	if (clientPipeSocket2 == INVALID_SOCKET)
	{
		std::cout << "accept failed with player 2: " << WSAGetLastError() << std::endl;
		closesocket(clientPipeSocket2);
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

	{
		/// send the clients their clientIDs
		char buffer1[2] = { '1','\0' };
		char buffer2[2] = { '2','\0' };
		sf::Vector2f tmp1 = p1Pos;
		unsigned long long byteCount1 = 0Ui64;
		while (byteCount1 < sizeof(buffer1))
			byteCount1 = send(clientPipeSocket, (char*)&buffer1, sizeof(buffer1), 0);
		if (byteCount1 == SOCKET_ERROR)
		{
			printf("Server send error %d.\n", WSAGetLastError());
			return -1;
		}
		else
		{
			//printf("Server: sent %ld bytes \n to player 1", byteCount1);
		}


		sf::Vector2f tmp2 = p2Pos;
		unsigned long long byteCount2 = 0Ui64;
		while (byteCount2 < sizeof(buffer2))
			byteCount2 = send(clientPipeSocket2, (char*)&buffer2, sizeof(buffer2), 0);
		if (byteCount2 == SOCKET_ERROR)
		{
			//printf("Server send error %d.\n", WSAGetLastError());
			return -1;
		}
		else
		{
			//printf("Server: sent %ld bytes \n to player 2", byteCount2);
		}
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

	/*ball.xpos = 785;
	ball.ypos = 435;

	xSpeed = 3;
	ySpeed = 3;

	ballDirX = -1;
	ballDirY = 1;
	
	player1.xpos = 40;
	player2.xpos = 1510;*/

	//	// Send a response to the client
	//	char buffer[8];
	//	std::cout << "Enter the message: ";
	//	buffer[0] = _itoa_s'1';
	//	buffer[1] = '1';
	//	buffer[2] = '1';
	//	buffer[3] = '1';
	//	buffer[4] = '1';
	//	buffer[5] = '1';
	//	buffer[6] = '1';
	//	buffer[7] = '\0';

	//	111111\0';
	//	int sbyteCount = send(clientPipeSocket, buffer, 200, 0);
	//	if (sbyteCount == SOCKET_ERROR) {
	//		std::cout << "Server send error: " << WSAGetLastError() << std::endl;
	//		return -1;
	//	}
	//	else {
	//		std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
	//	}

	//	// Receive data from the client
	//	char receiveBuffer[200];
	//	int rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
	//	if (rbyteCount < 0) {
	//		std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
	//		return 0;
	//	}
	//	else {
	//		std::cout << "Received data: " << receiveBuffer << std::endl;
	//	}

	//	
	//	ClientFrameInput tmp;

	//	
	//	int byteCount = recv(clientPipeSocket, (char*)&tmp, sizeof(ClientFrameInput), 0);
	//	if (byteCount < 0)
	//	{
	//		std::cout << "Unable to receive data from server supplying the Client ID for this machine" << std::endl;
	//		closesocket(clientPipeSocket2);
	//		closesocket(clientPipeSocket);

	//		WSACleanup();
	//		return 0;
	//	}
	//	p1Input.attack = tmp.attack;
	//	p1Input.start = tmp.start;
	//	p1Input.run = tmp.run;
	//	p1Input.left = tmp.left;
	//	p1Input.right = tmp.right;
	//	p1Input.down = tmp.down;
	//	p1Input.up = tmp.up;
	//}

	//{
	//	ClientFrameInput tmp2;
	//	
	//	int byteCount2 = recv(clientPipeSocket2, (char*)&tmp2, sizeof(ClientFrameInput), 0);
	//	if (byteCount2 < 0)
	//	{
	//		std::cout << "Unable to receive data from server supplying the Client ID for this machine" << std::endl;
	//		closesocket(clientPipeSocket2);
	//		closesocket(clientPipeSocket);

	//		WSACleanup();
	//		return 0;
	//	}
	//	p2Input.attack = tmp2.attack;
	//	p2Input.start = tmp2.start;
	//	p2Input.run = tmp2.run;
	//	p2Input.left = tmp2.left;
	//	p2Input.right = tmp2.right;
	//	p2Input.down = tmp2.down;
	//	p2Input.up = tmp2.up;
	//}
	float dt{ 0.f };
	sf::Clock timer;
	timer.restart();
	while (true)
	{



		char recvbuffP1[9];
		int ret, nLeft, idx;

		nLeft = 9;
		idx = 0;

		while (nLeft > 0)
		{
			ret = recv(clientPipeSocket, &recvbuffP1[idx], nLeft, 0);
			if (ret == SOCKET_ERROR)
			{

			}
			idx += ret;
			nLeft -= ret;
		}
		std::cout << "Got message from client 1 " << recvbuffP1 << std::endl;






		char recvbuffP2[9];
		int ret2, nLeft2, idx2;

		nLeft2 = 9;
		idx2 = 0;

		while (nLeft2 > 0)
		{
			ret2 = recv(clientPipeSocket2, &recvbuffP2[idx2], nLeft2, 0);

			if (ret2 == SOCKET_ERROR)
			{

			}
			idx2 += ret2;
			nLeft2 -= ret2;
		}
		std::cout << "Got message from client 2: " << recvbuffP2 << std::endl;


		// use input to update world
		if (recvbuffP1[(int)InputType::Escape] == 1 || recvbuffP2[(int)InputType::Escape] == 1)
			goto loopend;
		dt = timer.restart().asSeconds();
		if (recvbuffP1[(int)InputType::Right] == 1)
		{
			p1Pos.x += 40.f * dt;
		}
		if (recvbuffP1[(int)InputType::Left] == 1)
		{
			p1Pos.x -= 40.f * dt;
		}
		if (recvbuffP1[(int)InputType::Down] == 1)
		{
			p1Pos.y += 40.f * dt;
		}
		if (recvbuffP1[(int)InputType::Up] == 1)
		{
			p1Pos.y -= 40.f * dt;
		}


		if (recvbuffP2[(int)InputType::Right] == 1)
		{
			p2Pos.x += 40.f * dt;
		}
		if (recvbuffP2[(int)InputType::Left] == 1)
		{
			p2Pos.x -= 40.f * dt;
		}
		if (recvbuffP2[(int)InputType::Down] == 1)
		{
			p2Pos.y += 40.f * dt;
		}
		if (recvbuffP2[(int)InputType::Up] == 1)
		{
			p2Pos.y -= 40.f * dt;
		}

		std::string posXP1 = ((std::to_string(p1Pos.x).length() == 4) ? std::to_string(p1Pos.x) : (std::to_string(p1Pos.x).length() == 3) ? "0" + std::to_string(p1Pos.x) : (std::to_string(p1Pos.x).length() == 2) ? "00" + std::to_string(p1Pos.x) : "000" + std::to_string(p1Pos.x));
		std::string posYP1 = ((std::to_string(p1Pos.y).length() == 3) ? std::to_string(p1Pos.y) : (std::to_string(p1Pos.y).length() == 2) ? "0" + std::to_string(p1Pos.y) : "00" + std::to_string(p1Pos.y));
		std::string posXP2 = ((std::to_string(p2Pos.x).length() == 4) ? std::to_string(p2Pos.x) : (std::to_string(p2Pos.x).length() == 3) ? "0" + std::to_string(p2Pos.x) : (std::to_string(p2Pos.x).length() == 2) ? "00" + std::to_string(p2Pos.x) : "000" + std::to_string(p2Pos.x));
		std::string posYP2 = ((std::to_string(p2Pos.y).length() == 3) ? std::to_string(p2Pos.y) : (std::to_string(p2Pos.y).length() == 2) ? "0" + std::to_string(p2Pos.y) : "00" + std::to_string(p2Pos.y));

		std::string mystr = posXP1 + posYP1 + posXP2 + posYP2;

		mystr[14] = '\0';
		const char* sendbuf = mystr.c_str();
		int bytesSent{ 0 }, nlen{ 15 };

		while (bytesSent < 15)
		{
			bytesSent = send(clientPipeSocket, const_cast<char*>(sendbuf), 15, 0);

			if (bytesSent == SOCKET_ERROR)
			{
				std::cout << "server: send failed" << WSAGetLastError() << std::endl;
			}
			else
			{
				//std::cout << "server: sent total " << bytesSent << " bytes of 27 bytes sent total" << std::endl;

			}
		}
		//std::cout << "server: sent message";
		/*	else
			{
				std::cout << "Client: send ok" << WSAGetLastError() << std::endl;
				memset(&ThisSenderInfo, 0, sizeof(ThisSenderInfo));
				nlen - sizeof(ThisSenderInfo);

				getsockname(connSocket, (SOCKADDR*)&ThisSenderInfo, &nlen);

			}*/








		std::string mystr2 = posXP1 + posYP1 + posXP2 + posYP2;
		mystr2[14] = '\0';
			const char* sendbuf2 = mystr2.c_str();
			int bytesSent2{ 0 }, nlen2{ 15 };

			while (bytesSent2 < 15)
			{
				bytesSent2 = send(clientPipeSocket2, const_cast<char*>(sendbuf2), 15, 0);

				if (bytesSent2 == SOCKET_ERROR)
				{
					std::cout << "server: send failed" << WSAGetLastError() << std::endl;
				}
				else
				{
					//std::cout << "server: sent total " << bytesSent << " bytes of 27 bytes sent total" << std::endl;

				}
			}
			//std::cout << "server: sent message";
			/*	else
				{
					std::cout << "Client: send ok" << WSAGetLastError() << std::endl;
					memset(&ThisSenderInfo, 0, sizeof(ThisSenderInfo));
					nlen - sizeof(ThisSenderInfo);

					getsockname(connSocket, (SOCKADDR*)&ThisSenderInfo, &nlen);

				}*/




		



		// pack new values into entities and send the data to each
		
		//{
		//	struct combo
		//	{
		//		float tmp1x{ 0.f }, tmp1y{ 0.f };
		//		float tmp2x{ 0.f }, tmp2y{ 0.f };
		//	};

		//	combo data;
		//	/// send the clients their clientIDs
		//
		//	unsigned long long byteCount1 = 0Ui64;
		//	while (byteCount1 < sizeof(combo))
		//		byteCount1 = send(clientPipeSocket, (char*)&data, sizeof(combo), 0);
		//	if (byteCount1 == SOCKET_ERROR)
		//	{
		//		//printf("Server send error %d.\n", WSAGetLastError());
		//		return -1;
		//	}
		//	else
		//	{
		//		//printf("Server: sent %ld bytes \n to player 1", byteCount1);
		//	}

		//	auto t = data.tmp1x;
		//	data.tmp1x = data.tmp2x;
		//	data.tmp2x = t;
		//	auto t2 = data.tmp1y;
		//	data.tmp1y = data.tmp2y;
		//	data.tmp2y = t2;
		////	sf::Vector2f tmp2 = p2Pos;
		//	unsigned long long byteCount2 = 0Ui64;
		//	while (byteCount2 < sizeof(combo))
		//		byteCount2 = send(clientPipeSocket2, (char*)&data, sizeof(combo), 0);
		//	if (byteCount2 == SOCKET_ERROR)
		//	{
		//		//printf("Server send error %d.\n", WSAGetLastError());
		//		return -1;
		//	}
		//	else
		//	{
		//		//printf("Server: sent %ld bytes \n to player 2", byteCount2);
		//	}
		//}

		//

		//// collect input
		//{
		//	ClientFrameInput tmp;

		//	
		//	int byteCount = recv(clientPipeSocket, (char*)&tmp, sizeof(ClientFrameInput), 0);
		//	if (byteCount < 0)
		//	{
		//		std::cout << "Unable to receive data from server supplying the Client ID for this machine" << std::endl;
		//		closesocket(clientPipeSocket);
		//		closesocket(clientPipeSocket2);

		//		WSACleanup();
		//		return 0;
		//	}
		//	p1Input.attack = tmp.attack;
		//	p1Input.start = tmp.start;
		//	p1Input.run = tmp.run;
		//	p1Input.left = tmp.left;
		//	p1Input.right = tmp.right;
		//	p1Input.down = tmp.down;
		//	p1Input.up = tmp.up;
		//}

		//{
		//	ClientFrameInput tmp2;
		//	int byteCount2 = recv(clientPipeSocket2, (char*)&tmp2, sizeof(ClientFrameInput), 0);
		//	if (byteCount2 < 0)
		//	{
		//		std::cout << "Unable to receive data from server supplying the Client ID for this machine" << std::endl;
		//		closesocket(clientPipeSocket2);
		//		closesocket(clientPipeSocket);

		//		WSACleanup();
		//		return 0;
		//	}
		//	p2Input.attack = tmp2.attack;
		//	p2Input.start = tmp2.start;
		//	p2Input.run = tmp2.run;
		//	p2Input.left = tmp2.left;
		//	p2Input.right = tmp2.right;
		//	p2Input.down = tmp2.down;
		//	p2Input.up = tmp2.up;
		//}


		//int readBytes = 0;
		//char readBuffer[4];
		//readBytes = recv(clientPipeSocket, readBuffer, 4, 0);
		//if (readBytes == SOCKET_ERROR)
		//{
		//	//error
		//	goto loopend;
		//}

		//int readBytes2 = 0;
		//char readBuffer2[4];
		//readBytes2 = recv(clientPipeSocket2, readBuffer2, 4, 0);
		//if (readBytes2 == SOCKET_ERROR)
		//{
		//	//error
		//	goto loopend;

		//}

		// Update world here



		//std::string tmp = "000";
		//tmp.at(0) = readBuffer[0];
		//tmp.at(1) = readBuffer[1];
		//tmp.at(2) = readBuffer[2];

		//int newYPos = stoi(tmp);
		//player1.ypos = newYPos;

		//std::string tmp2 = "000";
		//tmp2.at(0) = readBuffer2[0];
		//tmp2.at(1) = readBuffer2[1];
		//tmp2.at(2) = readBuffer2[2];

		//int newYPos2 = stoi(tmp2);
		//player2.ypos = newYPos2;

		//

		//update();
		//std::string ballxStr = "0000";
		//std::string bx = std::to_string(ball.xpos);
		//size_t diff = ballxStr.length() - bx.length();
		//for (size_t i = 0; i < bx.length(); i++)
		//{
		//	ballxStr.at(i + diff) =  bx.at(i);
		//}


		//std::string ballyStr = "000";
		//std::string by = std::to_string(ball.ypos);
		//size_t diffy = ballyStr.length() - by.length();
		//for (size_t i = 0; i < by.length(); i++)
		//{
		//	ballyStr.at(i + diffy) = by.at(i);
		//}


		// package updated data into message string and add it to the write buffer


		//int sentBytes = 0;
		//char writeBuffer[14] = "0000000000000";// { tmp[0],tmp[1],tmp[2],tmp2[0],tmp2[1],tmp2[2],ballxStr[0],ballxStr[1],ballxStr[2],ballxStr[3],ballyStr[0],ballyStr[1],ballyStr[2],'\0'};
	 //   sentBytes = send(clientPipeSocket, writeBuffer, 14, 0);
		//if (sentBytes == SOCKET_ERROR)
		//{
		//	//error
		//	goto loopend;

		//}

		//int sentBytes2 = 0;
		//char writeBuffer2[14] = "0000000000000";  //{ tmp2[0],tmp2[1],tmp2[2],tmp[0],tmp[1],tmp[2],ballxStr[0],ballxStr[1],ballxStr[2],ballxStr[3],ballyStr[0],ballyStr[1],ballyStr[2],'\0' };
		//sentBytes2 = send(clientPipeSocket2, writeBuffer2, 14, 0);
		//if (sentBytes2 == SOCKET_ERROR)
		//{
		//	//error
		//	goto loopend;

		//}

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

void update()
{
	

	//ball.xpos += (int)((float)(ballDirX * xSpeed));
	//ball.ypos += (int)((float)(ballDirY * ySpeed));

	//if (ball.xpos < 0)
	//{
	//	std::cout << "Player2 Scored" << std::endl;
	//	system("Pause");
	//	ball.xpos = 785;
	//	ball.ypos = 435;
	//}
	//if (ball.xpos >= 1600 - 30)
	//{
	//	std::cout << "Player1 Scored" << std::endl;
	//	system("Pause");
	//	ball.xpos = 785;
	//	ball.ypos = 435;
	//}
	//if (ball.ypos <= 0)
	//{
	//	ballDirY = -ballDirY;
	//}
	//if (ball.ypos >= 900 - 30)
	//{
	//	ballDirY = -ballDirY;
	//}

	//if (ball.xpos < 100)
	//{
	//	if (ball.xpos < player1.xpos + 30 && ball.xpos + 30 > player1.xpos &&
	//		ball.ypos < player1.ypos + 140 && ball.ypos + 30 > player1.ypos)
	//	{
	//		//collided
	//		ballDirX = -ballDirX;
	//		
	//	}
	//}

	//if (ball.xpos > 1300)
	//{
	//	if (ball.xpos < player2.xpos + 30 && ball.xpos + 30 > player2.xpos &&
	//		ball.ypos < player2.ypos + 140 && ball.ypos + 30 > player2.ypos)
	//	{
	//		//collided
	//		ballDirX = -ballDirX;
	//		
	//	}
	//}

	
}


//
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iostream>
//
//int main() {
//	WSADATA wsaData;
//	int wsaerr;
//	WORD wVersionRequested = MAKEWORD(2, 2);
//	wsaerr = WSAStartup(wVersionRequested, &wsaData);
//	if (wsaerr != 0) {
//		std::cout << "The winsock dll not found!" << std::endl;
//		return 0;
//	}
//	else {
//		std::cout << "The winsock dll was found!" << "\n" << "The status: " << wsaData.szSystemStatus << std::endl;
//	}
//
//	// set up the socket configuration
//	SOCKET listener = INVALID_SOCKET;
//	listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (listener == INVALID_SOCKET) {
//		std::cout << "Error at socket(); " << WSAGetLastError() << std::endl;
//		WSACleanup();
//		return 0;
//	}
//	else {
//		std::cout << "socket() is OK!" << std::endl;
//	}
//
//	// Get the hostname of the current machine
//	char hostname[256];
//	if (gethostname(hostname, sizeof(hostname)) != 0) {
//		std::cout << "gethostname failed" << std::endl;
//		WSACleanup();
//		return 0;
//	}
//
//	// Get the IP address of the current machine
//	struct addrinfo hints, * result, * ptr;
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_flags = AI_PASSIVE;
//
//	if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
//		std::cout << "getaddrinfo failed" << std::endl;
//		WSACleanup();
//		return 0;
//	}
//
//	sockaddr_in service;
//	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//		if (ptr->ai_family == AF_INET) {
//			service.sin_family = AF_INET;
//			service.sin_addr = ((struct sockaddr_in*)ptr->ai_addr)->sin_addr;
//			service.sin_port = htons(gPort);
//			break;
//		}
//	}
//
//	freeaddrinfo(result);
//
//	// bind the specifics for the socket with its specific configuration
//	if (bind(listener, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
//		std::cout << "bind failed on listener" << WSAGetLastError() << std::endl;
//		closesocket(listener);
//		WSACleanup();
//		return 0;
//	}
//	else {
//		std::cout << "Socket bound!" << std::endl;
//	}
//
//	if (listen(listener, 8) == SOCKET_ERROR) {
//		std::cout << "listen(): Error listening on socket! " << WSAGetLastError() << std::endl;
//		closesocket(listener);
//		WSACleanup();
//		return 0;
//	}
//	else {
//		std::cout << "Socket now listening for incoming requests on port: " << gPort << std::endl;
//	}
//
//	std::cout << "Waiting for client connection" << std::endl;
//
//	WSACleanup();
//	return 0;
//}
