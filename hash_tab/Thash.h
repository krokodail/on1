#pragma once
#include <vector>
#include <optional>
#include <cstring>

class HashTable {
private:
    static const int TABLE_SIZE = 3;//для всех экзэмпляров будет один TABLE_SIZE
    std::vector<std::pair<int, std::string>> table;

    int quadraticProbe(int, int);//Квадратичное пробирование

public:
    HashTable();
	
	friend int hash(const std::string&);
	
    void insert(const std::string&, const std::string&);

    std::optional<std::string> search(const std::string&, const std::string&);
};

int hash(const std::string&);
