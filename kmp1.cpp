#include <iostream>
#include <string>
#include <vector>

using namespace std;

void func_pref(const string &stroka, vector<pair<string,int>> &preffix)
{
	string str;
	for(int i = 0; i < 1; i++)
	{
		preffix.push_back(make_pair(str = stroka[i], 0));
		str.clear();
		
		cout << endl <<"Префиксы: " << endl;
		cout << preffix.back().first << " ";
		for(int j = i+1; j < (stroka.size()/2); j++)
		{
			auto t = make_pair(preffix.back().first + stroka[j], 0);
			preffix.push_back(t);
			cout << preffix.back().first << " ";
		}
		cout << endl << endl;
	}
}

void func_array_of_templates(vector< pair<string, int> > &preffix, const string &str)
{
	for(int i = preffix.size() - 1; i >= 0 ; i--)
	{
		int ii = 0, jj = 0;
		while(ii < str.size())
		{
			if(preffix[i].first[jj] == str[ii]) {jj++; ii++;}
			else {preffix[i].second = 0; break;}
			if(jj == preffix[i].first.size()) {++preffix[i].second; jj = 0;}
		}
	}
}

void show_result(const vector< pair<string, int> > &preffix, const string &str)
{
	int count = 0;
	for(int i = 0; i < preffix.size(); i++)
	{
		if(preffix[i].second != 0){cout << "Строку " << str << " можно разбить на " << preffix[i].second 
						 << " одинаковых подстроки "<< preffix[i].first << endl; ++count;}
	}
	if (count == 0) cout << endl << "Строку " << str << " нельзя разбить на одинаковые части" << endl;
}


void start()
{
	string str;//исходная строка
	
	cout << "Введите строку: ";
	getline(cin, str);
	cout << endl;

	vector <pair<string, int>> preffix;//хранит пары  префикс и количество вхождений
	func_pref(str, preffix);//вычисляет префиксы
	func_array_of_templates(preffix, str);//считает количество вхождений

	show_result(preffix, str);

}

int main()
{
	start();

	return 0;
}
