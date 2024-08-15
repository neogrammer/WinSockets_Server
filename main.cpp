
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>

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

	/*ball.xpos = 785;
	ball.ypos = 435;

	xSpeed = 3;
	ySpeed = 3;

	ballDirX = -1;
	ballDirY = 1;
	
	player1.xpos = 40;
	player2.xpos = 1510;*/

	while (true)
	{
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
