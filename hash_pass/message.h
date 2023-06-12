#pragma once
#include <string>
#include <iostream>
#include <SFML/Network.hpp>

struct Message
{
	std::string _sender = "";
	std::string _recipient = "";
	std::string _text_message = "";

	void clear(void)
	{
		 _recipient = "";
		 _text_message = "";
	}
	
	
};

sf::Packet& operator<< (sf::Packet& packet, Message& m)
{
	return packet << m._sender << m._recipient << m._text_message;
}

sf::Packet& operator>> (sf::Packet& packet, Message& m)
{
	return packet >> m._sender >> m._recipient >> m._text_message;
}

std::ostream& operator<< (std::ostream& os, Message& mess)
{
	return os << mess._sender << ": " << mess._text_message;
}
