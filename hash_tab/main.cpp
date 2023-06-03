//g++ -std=c++17 иначе не видит optional
//Хэш таблица, а в ней  хеш функция методом умножения
//И квадратичное пробирование
//Тут хэш функция из файла T-hash.h


#include <iostream>
#include "Thash.h"

int main() {
    HashTable hashTable;
    //Добавили пользователей в таблицу
    hashTable.insert("5", "Value 1");
    hashTable.insert("e#dssWq", "Value 2");
    hashTable.insert("25", "Value 3");
    hashTable.insert("25", "Value 3");//Проверка, что больше указанного количества не добавит
	
	//А тут кто-то пробует залогиниться 
    std::optional<std::string> result = hashTable.search("e#dssWq", "Value 2");
    if (result) {
        std::cout << "Пользователь найден: " << *result << std::endl;
    } else {
        std::cout << "Пользователь не найден!" << std::endl;
    }
	
	//Это просто посмотреть, какой хэш получается
	std::cout << std::endl <<  std::endl << hash("e#dssWq2") << std::endl;
    return 0;
}

