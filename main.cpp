
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>
#include <interrelated.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <CidNet64.h>

sf::Vector2f p1Pos{ 30.f,700.f };
sf::Vector2f p2Pos{ 150.f,700.f };


int gPort{ 55555 };
int numPlayersConnected{ 0 };


ClientFrameInput p1Input{};
ClientFrameInput p2Input{};

bool client1Connected{ false };
bool client2Connected{ false };
cid::SocketHandle client1Handle{ };
cid::SocketHandle client2Handle{ };

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



	cid::Socket listener(cid::IPVersion::IPv4, INVALID_SOCKET);
	{
		cid::CResult result = cid::CResult::C_NotYetImplemented;
		auto tmp = listener.Create();
		if (tmp != cid::CResult::C_Success)
		{
			std::cout << "Creation of socket error, exiting program" << std::endl;
			cid::net::shutdown();
			return 1;
		}
		else
		{
			if (listener.Listen(cid::IPEndpoint("0.0.0.0", gPort)) != cid::CResult::C_Success)
			{
				std::cout << "Exiting program: Error trying to listen on port " << gPort << std::endl;
				listener.Close();
				cid::net::shutdown();
				return 1;
			}

		}
	}
	std::cout << "Listening on port " << gPort << std::endl;
	
	cid::Socket clientPipeSocket;
	cid::Socket clientPipeSocket2;

startover:
	if (client1Connected == false)
	{ 
		cid::CResult result = cid::CResult::C_NotYetImplemented;
		while (result != cid::CResult::C_Success)
		{
			while (listener.Accept(clientPipeSocket) != cid::CResult::C_Success)
			{
				std::cout << "unable to establish connection to player1, trying again" << std::endl;

			}

			std::cout << "Player 1 connection established" << std::endl;
			char buffer1[2];
			strcpy_s(buffer1, "1\0");
			tryagain2:
			result = clientPipeSocket.SendAll(buffer1, 2);
			if (result == cid::CResult::C_Success)
			{
				std::cout << "Player 1 successfully received their client ID: 1" << std::endl;
			}
			else
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					goto tryagain2;
				}
				std::cout << "Something happened to the connection, listening for player one again" << std::endl;
				clientPipeSocket.Close();
			}
		}
		client1Handle = clientPipeSocket.GetHandle();
		std::cout << "Player 1 ready, handle " << client1Handle << std::endl;
	}


	if (client2Connected == false)
	{
		{
			cid::CResult result = cid::CResult::C_NotYetImplemented;
			while (result != cid::CResult::C_Success)
			{
				while (listener.Accept(clientPipeSocket2) != cid::CResult::C_Success)
				{
					std::cout << "unable to establish connection to player2, trying again" << std::endl;

				}

				std::cout << "Player 2 connection established" << std::endl;
				char buffer1[2];
				strcpy_s(buffer1, "2\0");
				tryagain1:
				result = clientPipeSocket2.SendAll(buffer1, 2);
				if (result == cid::CResult::C_Success)
				{
					std::cout << "Player 2 successfully received their client ID: 2" << std::endl;
					client2Connected = true;
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						goto tryagain1;
					}
					std::cout << "Something happened to the connection, listening for player two again" << std::endl;
					clientPipeSocket2.Close();
				}
			}
			client2Handle = clientPipeSocket2.GetHandle();
			
			std::cout << "Player 2 ready, handle " << client2Handle << std::endl;

		}
	}
	std::cout << "Both players ready" << std::endl;
		
	if (client1Connected == false)
	{
		char buffer1[2];
		strcpy_s(buffer1, "y\0");
		tryagain3:
		auto result = clientPipeSocket.SendAll(buffer1, 2);
		if (result == cid::CResult::C_Success)
		{
			std::cout << "Sent y to inform player1 to start running" << std::endl;
			client1Connected = true;

		}
		else
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				goto tryagain3;
			}
			std::cout << "Something happened to the connection, listening for player one again" << std::endl;
			client1Connected = false;
			clientPipeSocket.Close();
			goto startover;
		}
	}

	//if (client2Connected == false)
	//{
	//	char buffer1[2];
	//	strcpy_s(buffer1, "y\0");
	//	auto result = clientPipeSocket2.SendAll(buffer1, 2);
	//	if (result == cid::CResult::C_Success)
	//	{
	//		std::cout << "Player 1 successfully received their client ID: 1" << std::endl;
	//		client2Connected = true;
	//	}
	//	else
	//	{
	//		std::cout << "Something happened to the connection, listening for player one again" << std::endl;
	//		client2Connected = false;
	//		clientPipeSocket2.Close();
	//		goto startover;
	//	}
	//}


	float dt{ 0.f };
	sf::Vector2f p1Pos{ 50.f, 600.f };
	sf::Vector2f p2Pos{ 350.f, 600.f };
	sf::Clock timer;
	timer.restart();
	bool running = true;
	while (running)
	{
		
		std::cout << "attempting to receive from client1" << std::endl;

		char buffer[9] = {};
		char buffer2[9] = {};
		{
			cid::CResult result = cid::CResult::C_Success;
				tryagain4:
				result = clientPipeSocket.RecvAll(buffer, 9);
				if (result != cid::CResult::C_Success)
				{
					std::cout << "oh no!  player one disconnected" << std::endl;
					// player 1 disconnected
					clientPipeSocket.Close();
					client1Connected = false;
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						goto tryagain4;
					}
					std::cout << "Got message from client 1 " << buffer << std::endl;
				}
		}
		


		std::cout << "attempting to receive from client2" << std::endl;

		{
			cid::CResult result = cid::CResult::C_Success;
		
			tryagain5:
				result = clientPipeSocket2.RecvAll(buffer2, 9);
				if (result != cid::CResult::C_Success)
				{
					std::cout << "oh no!  player two disconnected" << std::endl;
					// player 1 disconnected
					clientPipeSocket2.Close();
					client2Connected = false;
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						goto tryagain5;
					}
					std::cout << "Got message from client 2 " << buffer2 << std::endl;
				}
			
		}
		


		// use input to update world
		if (client1Connected)
		{
			if (buffer[(int)InputType::Escape] == '1')
			{
				client1Connected = false;
				clientPipeSocket.Close();
			}
		}

		if (client2Connected)
		{
			if (buffer2[(int)InputType::Escape] == '1')
			{
				client2Connected = false;
				clientPipeSocket2.Close();
			}
		}


	

		if (client1Connected == false && client2Connected == false)
		{
			goto startover;
		}
		else
		{
			if (client1Connected == false)
			{
				// reestablish connection with player 1, provide its client ID then receive input from it
				cid::CResult result = cid::CResult::C_NotYetImplemented;
				{
					while (result != cid::CResult::C_Success)
					{
						while (listener.Accept(clientPipeSocket) != cid::CResult::C_Success)
						{
							std::cout << "unable to establish connection to player1, trying again" << std::endl;

						}

						std::cout << "Player 1 connection established" << std::endl;
						char buffer1[2];
						strcpy_s(buffer1, "1\0");
						tryagain6:
						result = clientPipeSocket.SendAll(buffer1, 2);
						if (result == cid::CResult::C_Success)
						{
							std::cout << "Player 1 successfully received their client ID: 1" << std::endl;
						}
						else
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							{
								goto tryagain6;
							}
							std::cout << "Something happened to the connection, listening for player one again" << std::endl;
							clientPipeSocket.Close();
						}
					}
				}
				client1Handle = clientPipeSocket.GetHandle();
				std::cout << "Player 1 ready, handle " << client1Handle << std::endl;

				char buffer1[2];
				strcpy_s(buffer1, "y\0");
				{
					tryagain7:
					auto result = clientPipeSocket.SendAll(buffer1, 2);
					if (result == cid::CResult::C_Success)
					{
						std::cout << "Player 1 successfully received their client ID: 1" << std::endl;
						client1Connected = true;

					}
					else
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							goto tryagain7;
						}
						std::cout << "Something happened to the connection, listening for player one again" << std::endl;
						client1Connected = false;
						clientPipeSocket.Close();
						goto startover;
					}
				}

				{
					char buf[9] = {};
					cid::CResult result = cid::CResult::C_Success;

					tryagain8:
					result = clientPipeSocket.RecvAll(buf, 9);
					if (result != cid::CResult::C_Success)
					{
						std::cout << "oh no!  player one disconnected" << std::endl;
						// player 1 disconnected
						clientPipeSocket.Close();
						client1Connected = false;
						goto startover;
					}
					else
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							goto tryagain8;
						}
						std::cout << "Got message from client 1 " << buf << std::endl;
					}
				}

			}
			else
			{
				if (client2Connected == false)
				{
					// reestablish connection with player 2, provide its clientId then receive input from it
									// reestablish connection with player 1, provide its client ID then receive input from it
					{
						cid::CResult result = cid::CResult::C_NotYetImplemented;
						while (result != cid::CResult::C_Success)
						{
							while (listener.Accept(clientPipeSocket2) != cid::CResult::C_Success)
							{
								std::cout << "unable to establish connection to player2, trying again" << std::endl;

							}

							std::cout << "Player 2 connection established" << std::endl;
							char buffer1[2];
							strcpy_s(buffer1, "1\0");
							tryagain9:
							result = clientPipeSocket2.SendAll(buffer1, 2);
							if (result == cid::CResult::C_Success)
							{
								std::cout << "Player 2 successfully received their client ID: 2" << std::endl;
							}
							else
							{
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
								{
									goto tryagain9;
								}
								std::cout << "Something happened to the connection, listening for player 2 again" << std::endl;
								clientPipeSocket2.Close();
							}
						}
						client1Handle = clientPipeSocket2.GetHandle();
						std::cout << "Player 2 ready, handle " << client1Handle << std::endl;
					}

					char buffer1[2];

					{
						strcpy_s(buffer1, "y\0");
						tryagain10:
						auto result = clientPipeSocket2.SendAll(buffer1, 2);
						if (result == cid::CResult::C_Success)
						{
							std::cout << "Player 2 successfully received their client ID: 2" << std::endl;
							client2Connected = true;

						}
						else
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							{
								goto tryagain10;
							}
							std::cout << "Something happened to the connection, listening for player 2 again" << std::endl;
							client2Connected = false;
							clientPipeSocket2.Close();
							goto startover;
						}
					}
					{
						char buf[9] = {};
						cid::CResult result = cid::CResult::C_Success;
						tryagain11:
						result = clientPipeSocket2.RecvAll(buf, 9);
						if (result != cid::CResult::C_Success)
						{
							std::cout << "oh no!  player 2 disconnected" << std::endl;
							// player 1 disconnected
							clientPipeSocket2.Close();
							client2Connected = false;
							goto startover;
						}
						else
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							{
								goto tryagain11;
							}
							std::cout << "Got message from client 2 " << buf << std::endl;
						}
					}

				}
			}

		}


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
			tryagain12:
				result = clientPipeSocket.SendAll(sendbuffer1, 15);
				if (result != cid::CResult::C_Success)
				{
					std::cout << "oh no!  player one disconnected" << std::endl;
					// player 1 disconnected
					clientPipeSocket.Close();
					client1Connected = false;
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						goto tryagain12;
					}
					std::cout << "sent message to client 1 " << sendbuffer1 << std::endl;
				}
		}

		std::cout << "about to send to client2 the worlds update data" << std::endl;

		{
			cid::CResult result = cid::CResult::C_Success;
			tryagain13:
				result = clientPipeSocket2.SendAll(sendbuffer2, 15);
				if (result != cid::CResult::C_Success)
				{
					std::cout << "oh no!  player one disconnected" << std::endl;
					// player 1 disconnected
					clientPipeSocket2.Close();
					client2Connected = false;
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
					{
						goto tryagain13;
					}
					std::cout << "Got message from client 2 " << sendbuffer2 << std::endl;
				}
		}


		if (client1Connected == false && client2Connected == false)
		{
			goto startover;
		}
		else
		{
			if (client1Connected == false)
			{
				// reestablish connection with player 1, provide its client ID then receive input from it
				cid::CResult result = cid::CResult::C_NotYetImplemented;
				while (result != cid::CResult::C_Success)
				{
					while (listener.Accept(clientPipeSocket) != cid::CResult::C_Success)
					{
						std::cout << "unable to establish connection to player1, trying again" << std::endl;

					}

					std::cout << "Player 1 connection established" << std::endl;
					char buffer1[2];
					strcpy_s(buffer1, "1\0");
					tryagain14:
					result = clientPipeSocket.SendAll(buffer1, 2);
					if (result == cid::CResult::C_Success)
					{
						std::cout << "Player 1 successfully received their client ID: 1" << std::endl;
					}
					else
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							goto tryagain14;
						}
						std::cout << "Something happened to the connection, listening for player one again" << std::endl;
						clientPipeSocket.Close();
					}
				}
				client1Handle = clientPipeSocket.GetHandle();
				std::cout << "Player 1 ready, handle " << client1Handle << std::endl;

				char buffer1[2];
				strcpy_s(buffer1, "y\0");
				{
					tryagain15:
					auto result = clientPipeSocket.SendAll(buffer1, 2);
					if (result == cid::CResult::C_Success)
					{
						std::cout << "Player 1 successfully received their client ID: 1" << std::endl;
						client1Connected = true;

					}
					else
					{
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
						{
							goto tryagain15;
						}
						std::cout << "Something happened to the connection, listening for player one again" << std::endl;
						client1Connected = false;
						clientPipeSocket.Close();
						goto startover;
					}
				}

			}
			else
			{
				if (client2Connected == false)
				{
					// reestablish connection with player 2, provide its clientId then receive input from it
									// reestablish connection with player 1, provide its client ID then receive input from it
					cid::CResult result = cid::CResult::C_NotYetImplemented;
					while (result != cid::CResult::C_Success)
					{
						while (listener.Accept(clientPipeSocket2) != cid::CResult::C_Success)
						{
							std::cout << "unable to establish connection to player2, trying again" << std::endl;

						}

						std::cout << "Player 2 connection established" << std::endl;
						char buffer1[2];
						strcpy_s(buffer1, "1\0");
						tryagain16:
						result = clientPipeSocket2.SendAll(buffer1, 2);
						if (result == cid::CResult::C_Success)
						{
							std::cout << "Player 2 successfully received their client ID: 2" << std::endl;
						}
						else
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							{
								goto tryagain16;
							}
							std::cout << "Something happened to the connection, listening for player 2 again" << std::endl;
							clientPipeSocket2.Close();
						}
					}
					client1Handle = clientPipeSocket2.GetHandle();
					std::cout << "Player 2 ready, handle " << client1Handle << std::endl;

					char buffer1[2];
					strcpy_s(buffer1, "y\0");
					{
						tryagain17:
						auto result = clientPipeSocket2.SendAll(buffer1, 2);
						if (result == cid::CResult::C_Success)
						{
							std::cout << "Player 2 successfully received their client ID: 2" << std::endl;
							client2Connected = true;

						}
						else
						{
							if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
							{
								goto tryagain17;
							}
							std::cout << "Something happened to the connection, listening for player 2 again" << std::endl;
							client2Connected = false;
							clientPipeSocket2.Close();
							goto startover;
						}
					}
					

				}
			}

		}

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

