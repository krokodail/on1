
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//Мы читаем слева на право
//Это функция есть переборка возможных комбинаций для строки
//Начинаем с первой буквы, потом первая + вторая и т.д.
//Дошли до конца строки, перешли ко второй букве, вторая + третья и т.д до конца строки
//И всех символов в строке

void func_pref(const string &stroka, vector<pair<string,int>> &preffix)
{
	string str;
	for(int i = 0; i < stroka.size(); i++)
	{
		preffix.push_back(make_pair(str = stroka[i], 0));
		str.clear();
		if(i == (stroka.size() - 1)) break;
		
		for(int j = i+1; j < stroka.size(); j++)
		{
			preffix.push_back( make_pair(preffix.back().first + stroka[j], 0) );
		}
	}

	for(auto it = 0; it < preffix.size(); it++)
	{
		for(auto t = 1; t < preffix.size(); t++)
		{
			if(preffix[it].first == preffix[t].first) preffix.erase(preffix.begin()+t);
		}
	}
}
//Тут идея такова, что для каждой строки делаеться ипс массив
//потом сразу же ищется количество вхождений в строку и пишется в вектор preffix

void func_array_of_templates( vector< pair<string, int> > &preffix, const string &str)
{
	for(int i = 0; i < preffix.size(); i++)
	{
		int ips[preffix[i].first.size()]{0};
		if(preffix[i].first.size() == 1)//Если в строке пары один символ -> пробегаемся по исходной строке, считая кол-во вхождений символа в строку
		{
			int count = 0;

			for(int k = 0; k < str.size(); k++)
			{
				if(preffix[i].first[0] == str[k]) ++count;
			}

			preffix[i].second = count;
			continue;
		}
		for(int j = 0; j < preffix[i].first.size(); j++) // этот цикл идет по строке в паре...
		{
			int jj = 0, ii = 1;
			while(ii <= preffix[i].first.size())
			{
				if(preffix[i].first[ii] == preffix[i].first[jj]) {jj++; ips[ii] = jj; ii++;}
				else
				{
					if(jj != 0) jj = ips[jj - 1];
					else {ips[ii]=0; ii++;}
				}
			}
			//Заполнили массив
		}
		//Теперь организуем поиск с помощью КМП
		//Число найденных паттернов запишем в вектор с парами
		int ii = 0, jj = 0, count = -1;
		while(ii < str.size())
		{
			if(preffix[i].first[jj] == str[ii]) {jj++; ii++;}
			if(jj == preffix[i].first.size()) {++count; jj = ips[jj - 1];}
			else if (ii < str.size() && preffix[i].first[jj] != str[ii])
			{
				if(jj != 0)
				{
					jj = ips[jj - 1];
				}
				else ii += 1;
			}
		}
		preffix[i].second = count;
		//count = -1;
	}
}



int main()
{
	string str = "abcabcabcabc";//исходная строка
	
	vector <pair<string, int>> preffix;//хранит префиксы
	func_pref(str, preffix);//вычисляет префиксы
	func_array_of_templates(preffix, str);

	for(int i = 0; i < preffix.size(); i++)
	{
		/*if(preffix[i].second == 3)*/ cout << preffix[i].first << " " << preffix[i].second;
		cout << endl;
	}

	return 0;
}
