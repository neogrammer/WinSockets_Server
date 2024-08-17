
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>
#include <interrelated.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <CidNet64.h>

sf::Vector2f p1Pos{ 30.f,700.f };
sf::Vector2f p2Pos{ 150.f,700.f };


int gPort = 55555;
int numPlayersConnected = 0;


ClientFrameInput p1Input{};
ClientFrameInput p2Input{};

void update();

int main()
{
	if (!cid::net::initialize())
	{
		return 1;
	}
	else
	{
		std::cout << "Success on initialization of winsock!" << std::endl;
	}

	// server to listen for connections on port 4790
	// socket - bind to 4790
	// then listen on correct port

	//cid::IPEndpoint test("www.google.com", 8080);
	//if (test.GetIPVersion() == cid::IPVersion::IPv4)
	//{
	//	std::cout << "Hostname: " << test.GetHostname() << std::endl;
	//	std::cout << "IP: " << test.GetIPStr() << std::endl;
	//	std::cout << "Port: " << test.GetPort() << std::endl;
	//	std::cout << "IP Bytes... " << std::endl;
	//	for (auto& digit : test.GetBytes())
	//	{
	//		std::cout << (int)digit << std::endl;
	//	}
	//}
	//else
	//{
	//	std::cout << "This is not an ipv4 address" << std::endl;
	//}

	//WSADATA wsaData;
	//int wsaerr;
	//WORD wVersionRequested = MAKEWORD(2, 2);
	//wsaerr = WSAStartup(wVersionRequested, &wsaData);
	//if (wsaerr != 0)
	//{
	//	std::cout << "The winsock dll not found!" << std::endl;
	//	return 0;
	//}
	//else
	/*{
		std::cout << "The winsock dll was found!" << "\n" << "The status: " << wsaData.szSystemStatus << std::endl;
	}*/

	// set up the socket configuration
	cid::Socket listener(cid::IPVersion::IPv4, INVALID_SOCKET);
	{
		auto result = listener.Create();
		if (result != cid::CResult::C_Success)
		{
			std::cout << "Error at socket creation for main connection listener" << std::endl;
			cid::net::shutdown();
			return 1;
		}
		else
		{
			std::cout << "socket connector created" << std::endl;
		}
	}

	if (listener.Listen(cid::IPEndpoint("0.0.0.0", gPort)) == cid::CResult::C_Success)
	{
		std::cout << "Socket successfully listening on port " << gPort << std::endl;
	}
	else
	{
		std::cout << "socket not listening ton port 55555. failure " << std::endl;
		listener.Close();
		cid::net::shutdown();
		return 1;
	}

	cid::Socket clientPipeSocket;
	cid::Socket clientPipeSocket2;
	if (listener.Accept(clientPipeSocket) == cid::CResult::C_Success)
	{
		std::cout << "Player 1 connection established" << std::endl;
		if (listener.Accept(clientPipeSocket2) == cid::CResult::C_Success)
		{
			std::cout << "Player 2 connection established" << std::endl;


		}
		else
		{
			std::cout << "Failed to establish a connection with player 2" << std::endl;
			clientPipeSocket.Close();
			listener.Close();
			cid::net::shutdown();
			return 1;
		}
		
	}
	else
	{
		std::cout << "Failed to establish a connection with player 1" << std::endl;
		listener.Close();
		cid::net::shutdown();
		return 1;
	}

	// both clients are now connected

	//SOCKET listener = INVALID_SOCKET;
	//listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (listener == INVALID_SOCKET)
	//{
	//	std::cout << "Error at socket(); " << WSAGetLastError() << std::endl;
	//	cid::net::shutdown();
	//	return 0;
	//}
	//else
	//{
	//	std::cout << "socket() is OK!" << std::endl;
	//}

   // Get the hostname of the current machine
//char hostname[256];
//if (gethostname(hostname, sizeof(hostname)) != 0) {
//	std::cout << "gethostname failed" << std::endl;
//	cid::net::shutdown();
//	return 0;
//}
//
//// Get the IP address of the current machine
//struct addrinfo hints, * result, * ptr;
//ZeroMemory(&hints, sizeof(hints));
//hints.ai_family = AF_INET;
//hints.ai_socktype = SOCK_STREAM;
//hints.ai_flags = AI_PASSIVE;
//
//if (getaddrinfo(hostname, NULL, &hints, &result) != 0) {
//	std::cout << "getaddrinfo failed" << std::endl;
//	cid::net::shutdown();
//	return 0;
//}
//
//sockaddr_in service;
//for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//	if (ptr->ai_family == AF_INET) {
//		service.sin_family = AF_INET;
//		service.sin_addr = ((struct sockaddr_in*)ptr->ai_addr)->sin_addr;
//		service.sin_port = htons(gPort);
//		break;
//	}
//}
//
//freeaddrinfo(result);
//
//// bind the specifics for the socket with its specific configuration
//if (bind(listener.GetHandle(), (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
//	std::cout << "bind failed on listener" << WSAGetLastError() << std::endl;
//	closesocket(listener.GetHandle());
//	cid::net::shutdown();
//	return 0;
//}
//else {
//	std::cout << "Socket bound!" << std::endl;
//}

//if (listen(listener.GetHandle(), 8) == SOCKET_ERROR) {
//	std::cout << "listen(): Error listening on socket! " << WSAGetLastError() << std::endl;
//	listener.Close();
//	cid::net::shutdown();
//	return 0;
//}
//else {
//	std::cout << "Socket now listening for incoming requests on port: " << gPort << std::endl;
//}
//
//std::cout << "Waiting for client connection" << std::endl;
//
//	SOCKET clientPipeSocket = INVALID_SOCKET;
//	clientPipeSocket = accept(listener.GetHandle(), NULL, NULL);
//	if (clientPipeSocket == INVALID_SOCKET)
//	{
//		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
//		closesocket(listener.GetHandle());
//		cid::net::shutdown();
//		return 0;
//	}
//	else
//	{
//		std::cout << "Connection established!" << std::endl;
//		std::cout << ++numPlayersConnected << " player(s) connected, starting game" << std::endl;
//		numPlayersConnected++;
//	}
//	
//	SOCKET clientPipeSocket2 = INVALID_SOCKET;
//	clientPipeSocket2 = accept(listener.GetHandle(), NULL, NULL);
//	if (clientPipeSocket2 == INVALID_SOCKET)
//	{
//		std::cout << "accept failed with player 2: " << WSAGetLastError() << std::endl;
//		closesocket(clientPipeSocket2);
//		closesocket(listener.GetHandle());
//		cid::net::shutdown();
//		return 0;
//	}
//	else
//	{
//		std::cout << "Connection established!" << std::endl;
//		std::cout << ++numPlayersConnected << "Second player(s) connected, starting game" << std::endl;
//		numPlayersConnected++;
//	}
	{
		/*char buffer[256];
		strcpy_s(buffer, "Hello world from client!\0");
		cid::CResult result = cid::CResult::C_Success;
		while (result == cid::CResult::C_Success)
		{
			result = clientPipeSocket.SendAll(buffer, 256);
			std::cout  << "Attempting to send chunk of data..." << std::endl;
		}*/
	}
	{
	
	}
	{
		/// send the clients their clientIDs
		char buffer1[2];
		char buffer2[2];
		/*sf::Vector2f tmp1 = p1Pos;
		unsigned long long byteCount1 = 0Ui64;
		while (byteCount1 < sizeof(buffer1))
			byteCount1 = send(clientPipeSocket.GetHandle(), (char*)&buffer1, sizeof(buffer1), 0);
		if (byteCount1 == SOCKET_ERROR)
		{
			printf("Server send error %d.\n", WSAGetLastError());
			return -1;
		}*/
		
		strcpy_s(buffer1, "1\0");
		strcpy_s(buffer2, "2\0");
		{
			cid::CResult result = cid::CResult::C_Success;
			
				result = clientPipeSocket.SendAll(buffer1, 2);
				if (result != cid::CResult::C_Success)
				{
					clientPipeSocket.Close();
					clientPipeSocket2.Close();
					listener.Close();
					cid::net::shutdown();
					return 1;
				}
		}

		{
			cid::CResult result = cid::CResult::C_Success;
		
				result = clientPipeSocket2.SendAll(buffer2, 2);
				if (result != cid::CResult::C_Success)
				{
					clientPipeSocket.Close();
					clientPipeSocket2.Close();
					listener.Close();
					cid::net::shutdown();
					return 1;
				}
			
		}

		//sf::Vector2f tmp2 = p2Pos;
		//unsigned long long byteCount2 = 0Ui64;
		//while (byteCount2 < sizeof(buffer2))
		//	byteCount2 = send(clientPipeSocket2.GetHandle(), (char*)&buffer2, sizeof(buffer2), 0);
		//if (byteCount2 == SOCKET_ERROR)
		//{
		//	return -1;
		//}
	}

	float dt{ 0.f };
	sf::Vector2f p1Pos{ 50.f, 600.f };

	sf::Vector2f p2Pos{ 350.f, 600.f };
	sf::Clock timer;
	timer.restart();
	while (true)
	{
		std::cout << "about to recevie the input data from client1" << std::endl;

		char buffer[9];
		char buffer2[9];
		{
			cid::CResult result = cid::CResult::C_Success;
			
				result = clientPipeSocket.RecvAll(buffer, 9);
				if (result != cid::CResult::C_Success)
				{
					clientPipeSocket.Close();
					clientPipeSocket2.Close();
					listener.Close();
					cid::net::shutdown();
					return 1;
				}
				//std::cout << buffer << std::endl;
			
	
		/*char recvbuffP1[9];
		int ret, nLeft, idx;

		nLeft = 9;
		idx = 0;

		while (nLeft > 0)
		{
			ret = recv(clientPipeSocket.GetHandle(), &recvbuffP1[idx], nLeft, 0);
			if (ret == SOCKET_ERROR)
			{

			}
			idx += ret;
			nLeft -= ret;
		}*/
			std::cout << "Got message from client 1 " << buffer << std::endl;
		}
		std::cout << "received input data from client1" << std::endl;


		std::cout << "about to receive the input data from client2" << std::endl;

		{
			cid::CResult result = cid::CResult::C_Success;
		
				result = clientPipeSocket2.RecvAll(buffer2, 9);
				if (result != cid::CResult::C_Success)
				{
					clientPipeSocket.Close();
					clientPipeSocket2.Close();
					listener.Close();
					cid::net::shutdown();
					return 1;
				}
				//std::cout << buffer << std::endl;
		

			/*char recvbuffP1[9];
			int ret, nLeft, idx;

			nLeft = 9;
			idx = 0;

			while (nLeft > 0)
			{
				ret = recv(clientPipeSocket.GetHandle(), &recvbuffP1[idx], nLeft, 0);
				if (ret == SOCKET_ERROR)
				{

				}
				idx += ret;
				nLeft -= ret;
			}*/
			std::cout << "Got message from client 2 " << buffer2 << std::endl;
		}
		std::cout << "received input data from client2" << std::endl;

	/*	char recvbuffP2[9];
		int ret2, nLeft2, idx2;

		nLeft2 = 9;
		idx2 = 0;

		while (nLeft2 > 0)
		{
			ret2 = recv(clientPipeSocket2.GetHandle(), &recvbuffP2[idx2], nLeft2, 0);

			if (ret2 == SOCKET_ERROR)
			{

			}
			idx2 += ret2;
			nLeft2 -= ret2;
		}
		std::cout << "Got message from client 2: " << recvbuffP2 << std::endl;*/


		// use input to update world
		if (buffer[(int)InputType::Escape] == '1' || buffer2[(int)InputType::Escape] == '1')
			break;
		dt = timer.restart().asSeconds();
		if (buffer[(int)InputType::Right] == '1')
		{
			p1Pos.x += 40.f * dt;
		}
		if (buffer[(int)InputType::Left] == '1')
		{
			p1Pos.x -= 40.f * dt;
		}
		if (buffer[(int)InputType::Down] == '1')
		{
			p1Pos.y += 40.f * dt;
		}
		if (buffer[(int)InputType::Up] == '1')
		{
			p1Pos.y -= 40.f * dt;
		}


		if (buffer2[(int)InputType::Right] == '1')
		{
			p2Pos.x += 40.f * dt;
		}
		if (buffer2[(int)InputType::Left] == '1')
		{
			p2Pos.x -= 40.f * dt;
		}
		if (buffer2[(int)InputType::Down] == '1')
		{
			p2Pos.y += 40.f * dt;
		}
		if (buffer2[(int)InputType::Up] == '1')
		{
			p2Pos.y -= 40.f * dt;
		}

		std::string posXP1 = ((std::to_string((int)p1Pos.x).length() == 4) ? std::to_string((int)p1Pos.x) : (std::to_string((int)p1Pos.x).length() == 3) ? "0" + std::to_string((int)p1Pos.x) : (std::to_string((int)p1Pos.x).length() == 2) ? "00" + std::to_string((int)p1Pos.x) : "000" + std::to_string((int)p1Pos.x));
		std::string posYP1 = ((std::to_string((int)p1Pos.y).length() == 3) ? std::to_string((int)p1Pos.y) : (std::to_string((int)p1Pos.y).length() == 2) ? "0" + std::to_string((int)p1Pos.y) : "00" + std::to_string((int)p1Pos.y));
		std::string posXP2 = ((std::to_string((int)p2Pos.x).length() == 4) ? std::to_string((int)p2Pos.x) : (std::to_string((int)p2Pos.x).length() == 3) ? "0" + std::to_string((int)p2Pos.x) : (std::to_string((int)p2Pos.x).length() == 2) ? "00" + std::to_string((int)p2Pos.x) : "000" + std::to_string((int)p2Pos.x));
		std::string posYP2 = ((std::to_string((int)p2Pos.y).length() == 3) ? std::to_string((int)p2Pos.y) : (std::to_string((int)p2Pos.y).length() == 2) ? "0" + std::to_string((int)p2Pos.y) : "00" + std::to_string((int)p2Pos.y));

		std::string tmp = posXP1 + posYP1 + posXP2 + posYP2;
		char mystr[15];
		tmp.copy(mystr, tmp.length());
		mystr[14] = '\0';

		/// send the clients their clientIDs
	
		char sendbuffer1[15];
		char sendbuffer2[15];
		strcpy_s(sendbuffer1, mystr);
		strcpy_s(sendbuffer2, mystr);

		/*sf::Vector2f tmp1 = p1Pos;
		unsigned long long byteCount1 = 0Ui64;
		while (byteCount1 < sizeof(buffer1))
			byteCount1 = send(clientPipeSocket.GetHandle(), (char*)&buffer1, sizeof(buffer1), 0);
		if (byteCount1 == SOCKET_ERROR)
		{
			printf("Server send error %d.\n", WSAGetLastError());
			return -1;
		}*/
		std::cout << "about to send to client1 the worlds update data" << std::endl;
		{
			cid::CResult result = cid::CResult::C_Success;
			
				result = clientPipeSocket.SendAll(sendbuffer1, 15);
				if (result != cid::CResult::C_Success)
				{
					clientPipeSocket.Close();
					clientPipeSocket2.Close();
					listener.Close();
					cid::net::shutdown();
					return 1;
				}
		}
		std::cout << "sent to client1 the worlds update data" << std::endl;

		std::cout << "about to send to client2 the worlds update data" << std::endl;

		{
			cid::CResult result = cid::CResult::C_Success;
			
				result = clientPipeSocket2.SendAll(sendbuffer2, 15);
				if (result != cid::CResult::C_Success)
				{
					clientPipeSocket.Close();
					clientPipeSocket2.Close();
					listener.Close();
					cid::net::shutdown();
					return 1;
				}
		}
		std::cout << "sent to client2 the worlds update data" << std::endl;

		/*const char* sendbuf = mystr.c_str();
		int bytesSent{ 0 }, nlen{ 15 };

		while (bytesSent < 15)
		{
			bytesSent = send(clientPipeSocket.GetHandle(), const_cast<char*>(sendbuf), 15, 0);

			if (bytesSent == SOCKET_ERROR)
			{
				std::cout << "server: send failed" << WSAGetLastError() << std::endl;
			}
			
		}









		std::string mystr2 = posXP1 + posYP1 + posXP2 + posYP2;
		mystr2[14] = '\0';
			const char* sendbuf2 = mystr2.c_str();
			int bytesSent2{ 0 }, nlen2{ 15 };

			while (bytesSent2 < 15)
			{
				bytesSent2 = send(clientPipeSocket2.GetHandle(), const_cast<char*>(sendbuf2), 15, 0);

				if (bytesSent2 == SOCKET_ERROR)
				{
					std::cout << "server: send failed" << WSAGetLastError() << std::endl;
				}
				
			}*/


	}



	// exit program


	//close any open sockets
	if (listener.GetHandle() != INVALID_SOCKET)
	{
		listener.Close();
	}
	if (clientPipeSocket.GetHandle() != INVALID_SOCKET)
	{
		clientPipeSocket.Close();
	}
	if (clientPipeSocket2.GetHandle() != INVALID_SOCKET)
	{
		clientPipeSocket2.Close();
	}

	cid::net::shutdown();

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
