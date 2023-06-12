//Клиентская часть
//Функцию регистрации сделал отдельной, так удобней, когда проводиться проверка на существующего пользователя 
//Меню клиента реализовано через свитч
//Использована стороняя библиотека SFML

#include <iostream>
#include <string>
#include "message.h"
#include <SFML/Network.hpp>

void Registration (sf::TcpSocket& sock, sf::Packet& pack, Message& mess, std::string& log, std::string& pas)
{
	
	std::cout << "Ваш логин: ";
	std::cin >> log;
	
	std::cout << "\nВаш пароль: ";
	std::cin >> pas;
	std::cout << std::endl;
	
	mess._sender = log;

	pack << log << pas;
	sock.send(pack);
	pack.clear();
	mess.clear();
}

int main()
{

	sf::TcpSocket socket;
	socket.setBlocking(false);
	socket.connect("192.168.0.149", 55001); //Прописываем тот ip, который показывает сервер
	
	Message message;
	sf::Packet packet;

	std::string login(""), pass("");

	Registration(socket, packet, message, login, pass);

	while(true)
	{
		socket.receive(packet);
		packet >> message;
		
		if(message._text_message == "Exist")
		{
			std::cout << "\nПользователь с таким логином сейчас онлайн. Придумайте новый логин и пароль\n";
			packet.clear();
			message.clear();
			socket.disconnect();
			socket.connect("192.168.0.149", 55001);//Тут тоже нужно прописать ip
			Registration(socket, packet, message, login, pass);
		}
		
		else if(message._text_message == "good")
		{
			std::cout << "\nВы Зарегистрированы\n\n";
			break;
		}
	}

	while(true)
	{
		char select('0');
		
		std::cout << "Для получения ответа на запрос или проверки входящих выберите 4-ый вариант"
		     << "\n1) Получить список онлайн пользователей\n"
		     << "2) Отправить личное сообщение\n"
		     << "3) Отправить сообщение всем\n"
		     << "4) Проверить входящие сообщения\n"
		     << "5) Выход(Закрыть приложение)\n";
		     
		while(true)
		{//Проверка ввода на соответствие меню
			std::cout << "Введите номер из меню: ";
			std::cin >> select;
			std::cout << std::endl;
			if(select >= '1' && select <= '5') break;
			else continue;
		}

		switch(select)
		{
			case '1':
				{
					packet.clear();
					message._sender = login;
					message._text_message = "list_of_users";
					message._recipient = "Server";
					
					packet << message;
					socket.send(packet);
					
					break;
				}
			
			case '2':
				{
					packet.clear();
					
					message._sender = login;
					
					std::cout << "\nКому отправим(ник)?: ";
					std::cin >> message._recipient;
					
					std::cout << "\nВаше послание?: ";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::getline(std::cin, message._text_message);
					
					packet << message;
					socket.send(packet);

					break;
				}
			
			case '3':
				{
					packet.clear();
					
					message._sender = login;
					
					message._recipient = "all";
					
					std::cout << "\nВаше послание?: ";
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::getline(std::cin, message._text_message);
					
					packet << message;
					socket.send(packet);
					break;
				}
			
			case '4':
				{
					if(socket.receive(packet) == sf::Socket::Done)
					{
						packet >> message;
						std::cout  << "\nСообщение от "<<  message << "\n\n";
						packet.clear();
						message.clear();
					}
					else std::cout << "\nВходящих пока не было\n\n";
					
					break;
				}

			case '5':
				{
					packet.clear();
					message.clear();

					message._sender = login;
					message._text_message = "delete";
					message._recipient = "Server";
					
					packet << message;
					socket.send(packet);
					
					packet.clear();
					message.clear();

					socket.disconnect();

					return 0;
				}

			default:
				std::cout << "Нет такого варианта\n";
				break;
		}
		
	}
	
	return 0;
}
