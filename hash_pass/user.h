#pragma once
#include <string>
#include <SFML/Network.hpp>

class User 
{
	private:
		std::string	_log_user;
		std::string	_pass_user;
		sf::TcpSocket* _socket = nullptr;
	
	public:
		User();
		User(std::string, std::string);
		std::string get_name() const;
		sf::TcpSocket* get_socket();
		void set_socket(sf::TcpSocket*);

};
