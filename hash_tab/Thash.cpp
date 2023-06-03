#include "Thash.h"
#include <iostream>

int hash(const std::string& str)
{
	//Метод умножения
	//По идеи, это не надежная функция
	//Пароль можно подобрать из разных символов, главное чтобы сумма этих символов подходила
	//Каждый чар переводим в инт и складываем с другим чар переведенным в инт и т.д.
    	int key = 0;
    	const double A = 0.007;
  	const int M = 1124;
    	for(int i = 0; i < str.size(); i++) key += str[i];
    
    	return int(M * (A * key - int(A* key)));
}


HashTable::HashTable()	{table.resize(TABLE_SIZE);}


int HashTable::quadraticProbe(int index, int attempt)	
{
	return (index + attempt * attempt) % TABLE_SIZE;
}


void HashTable::insert(const std::string& key, const std::string& value)
{
    int attempt = 0;
    int index = 0;
    while (table[index].first != 0) {
    	attempt++;
        index = quadraticProbe(index, attempt);

        // Проверка правильной позиции для вставки
        if (attempt >= TABLE_SIZE) {
        	std::cout << "Таблица хэшей заполнена"<< std::endl;
        	return;
            }
        }

        table[index] = std::make_pair(hash(key), value);
}



std::optional<std::string> HashTable::search(const std::string& key, const std::string& log) {
        int index = 0;
        int attempt = 0;

        while (table[index].second != log) {
            attempt++;
            index = quadraticProbe(index, attempt);

            // Проверить ключ в таблице
            if (table[index].first == 0 || attempt >= TABLE_SIZE) {
                return std::nullopt;
            }
        }
		if (table[index].first == hash(key))return table[index].second;
		
		return std::nullopt;
    }
