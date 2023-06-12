#include <SFML/Network.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "user.h"
#include "message.h"

int main()
{
	std::cout << "Server is running on: " << sf::IpAddress::getLocalAddress() << std::endl;

	
	sf::TcpListener listener;
	sf::SocketSelector selector;

	std::vector<User*> clients;

	listener.listen(55001);
	selector.add(listener);
	
	while(true)
	{
		if(selector.wait())
		{ //Часть отвечающая за создание нового пользователя
		  //Когда есть входящее соединение
		  //Сюда же добавил проверку на существующего с таким же  именем пользователя, как и у входящего 
			if(selector.isReady(listener))
			{
				sf::TcpSocket *socket = new sf::TcpSocket;
				listener.accept(*socket);
				
				sf::Packet packet;
				std::string log, pas;
				socket -> receive(packet);
				packet >> log >> pas;
				
				User *usr = new User(log, pas);
				usr -> set_socket(socket);
				
				for(int r = 0; r <= (clients.size() -1); r++)
				{
					if (clients.empty())
					{
						clients.push_back(usr);
						selector.add(*socket);
						
						packet.clear();
						
						Message message;
						message._sender = "server";
						message._recipient = log;
						message._text_message = "good";
						
						packet.clear();
						packet << message;
						socket -> send(packet);
						
						packet.clear();
						message.clear();
						
						usr = nullptr;
						socket = nullptr;
						
						break;
					}
					
					if(clients[r] -> get_name() == usr -> get_name())
					{
						std::cout << "Client exist\n";
						Message message;
						message._sender = "server";
						message._recipient = log;
						message._text_message = "Exist";
						
						packet.clear();
						packet << message;
						socket -> send(packet);
						
						packet.clear();
						message.clear();

						selector.remove(*socket);
						delete socket;
						delete usr;
						
						break;
					}
					
					if( (r == (clients.size() -1)) && ( (clients[r] -> get_name()) != (usr -> get_name()) ) )
					{
						clients.push_back(usr);
						selector.add(*socket);
						
						packet.clear();
						
						Message message;
						message._sender = "server";
						message._recipient = log;
						message._text_message = "good";
						
						packet.clear();
						packet << message;
						socket -> send(packet);
						
						packet.clear();
						message.clear();
						
						usr = nullptr;
						socket = nullptr;
						
						break;
					}
				}				
			}
		//конец 'создания(проверки) пользователя'
			
			//часть проверки запросов от пользователя
			else
			{
				for(int i = 0; i < clients.size(); i++)
				{
					if(selector.isReady(*(clients[i] -> get_socket())))
					{
						sf::Packet packet;
						if(clients[i] -> get_socket() -> receive(packet) == sf::Socket::Done)
						{
							Message message;
							packet >> message;
							if (message._text_message == "list_of_users")
							{
								packet.clear();
								message.clear();

								for(int j = 0; j < clients.size(); j++)
								{
									message._text_message += clients[j] -> get_name() + ' ';
								}
								
								message._sender = "server";
								message._recipient = clients[i] -> get_name();

								packet << message;
								(*clients[i]).get_socket() -> send(packet);
								packet.clear();
								message.clear();
							}
							
							if(message._recipient == "all")
							{
								packet << message;

								for(int i = 0; i < clients.size(); i++)
								{
									if(clients[i] -> get_name() != message._sender) (*clients[i]).get_socket() -> send(packet);
								}

								packet.clear();
								message.clear();
							}
							
							if (message._text_message == "delete")
							{//очищает вектор от офлайн юзеров
								for(auto iter = clients.begin(); iter < clients.end(); iter++)
								{
									if((*iter) -> get_name() == message._sender)
									{
										selector.remove( *((*iter) -> get_socket()));
										clients.erase(std::remove(clients.begin(), clients.end(), *iter), clients.end());
										
										packet.clear();
										message.clear();
										
										break;
									}
								}
							//Конец очистки
							}
							
							else
							{
								for(int i = 0; i < clients.size(); i++)
								{
									if(message._recipient == clients[i] -> get_name())
									{
										packet.clear();
										packet << message;
										(*clients[i]).get_socket() -> send(packet);
										packet.clear();
										message.clear();
										break;
									}
								}
							}
						}
					}
				}
			}
			//Конец проверки запросов пользователя
		}		

	}
			
	return 0;
}
