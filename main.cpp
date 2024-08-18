
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

	

	{
		/// send the clients their clientIDs
		char buffer1[2];
		char buffer2[2];
	
		
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
				
			std::cout << "Got message from client 2 " << buffer2 << std::endl;
		}
		std::cout << "received input data from client2" << std::endl;

	
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
	

	
	
}

