#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <cmath>
#include <sstream>
#include <exception>

using namespace std;

const vector<string> Unit_Test =
{
	{ "1+2-3*4/5+6-7*8/9" },
	{ "11+22-33" },
	{ "55*0" },
	{ "44/11"},
	{ "-1+5" },
	{ "-33/3*5" },
	{ "1/0"},
	{ "+36-5&4"},
	{ "1/5**9"}
};

set<char> Numbers = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' }; // Множество цифр

const map <char, int> Operators = { { '+', 1 }, { '-', 2 }, { '*', 3 }, { '/', 4 }}; // С помощью данного словаря будем определять тип оператора

int Transformation(const char& symbol)
{
	return int(symbol - '0');
} // С помощью данной функции будем переводить символ цифры в саму цифру. Преобразователь char в int.

bool First_Symbol(const char& ch)
{
	if (ch == '*' || ch == '/')
	{
		return false;
	}
	if (Transformation(ch) < 0 || Transformation(ch) > 9)
	{
		return false;
	}
	return true;
} // Данная функция определяет, удовлетворителен ли для нас 1 символ в строке

double Calculate(const string& Expression)
{
	vector<double> StrExpr; // Сюда добавим все числа подряд из потока
	vector<char> StrOper; // А сюда все знаки (операторы)

	int size = Expression.size(); // Здесь храним кол-во элементов в строке.

	char tmp;
	int i = 0; // Счетчик, сколько символов в строке прошли
	int IZM = 1;
	while (i < size)
	{
		tmp = Expression[i];

		if (i == 0 && tmp == '-')
		{
			IZM = -1;
			i++;
			continue;
		} // Проверка первого числа на отрицательность

		if (i == 0 && tmp == '+')
		{
			i++;
			continue;
		} // Проверка на ввод знака + перед первым числом

		if (i == 0 && !(First_Symbol(tmp)))
		{
			throw runtime_error("Invalid input");
		} // проверка на корректность первого сивола
		if (Operators.count(tmp) == 0 && Numbers.count(tmp) == 0)
		{
			throw runtime_error("Invalid input");
		} // Проверка на корректность символа

		if (Numbers.count(tmp) == 1) // Если символ - цифра
		{
			double rez = 0; // Создаем переменную для числа
			int k = 1;
			while (Numbers.count(Expression[i + k]) == 1) // Если последующий символ тоже цифра 
			{
				k++;
			}
			for (int n = 0; n < k; n++)
			{
				rez += double(Transformation(Expression[i + n])) * pow(10, (k - 1 - n)); // Создаем число по набору последовательных цифр
			}
			if (i == 1)
			{
				rez *= IZM;
			}
			StrExpr.push_back(double(rez)); // Добавляем его в вектор с числами
			i += k; // Увеличиваем счетчик на кол-во пройденных символов
		} 

		else // Если символ - знак
		{
			if (Operators.count(Expression[i + 1]) == 1)
			{
				throw runtime_error("Invalid input");
			} // Проверка на два знака подряд
			StrOper.push_back(tmp); // Добавляем его в вектор операторов
			i += 1; // Увеличиваем счетчик
		}
	}

	int size1 = StrExpr.size();
	int size2 = StrOper.size();

	vector <double> new_Expr; // Создаем вектор для значений, после операций * и /
	vector <char> new_Oper; // создаем вектор операторов, в котором будут только + или -

	for (int i = 0; i < size2; i++)
	{
		char tmp = StrOper[i];
		if (Operators.at(tmp) == 1 || Operators.at(tmp) == 2) // Если оператор + или -
		{
			new_Expr.push_back(StrExpr[i]); // Добавляем число, предшествующее оператору в вектор новых чисел
			new_Oper.push_back(tmp); // И аналогично с оператором
		}

		if (Operators.at(tmp) == 3 || Operators.at(tmp) == 4) // Если оператор * или /
		{
			double tmp1 = 0; // Создаем переменную для результата
			if (Operators.at(tmp) == 3) // Если оператор *
			{
				tmp1 = StrExpr[i] * StrExpr[i + 1]; // переменожаем 2 сеседних числа, слева и справа от оператора
			}
			if (Operators.at(tmp) == 4) // Если оператор /
			{
				if (StrExpr[i + 1] == 0)
				{
					throw invalid_argument("Cannot be divided by 0");
				}
				tmp1 = StrExpr[i] / StrExpr[i + 1]; // Делим то, что слева от оператора, на то, что справа
			}
			StrExpr[i + 1] = tmp1;
		} // После таких операций помещаем результат в ячейку, которая справа от оператора, чтобы обработать ее при следующем операторе

		if (i == size2 - 1) // В конце добавляем последнее число, так как оно не обработается (после него нет оператора)
		{
			new_Expr.push_back(StrExpr[size1 - 1]);
		}
	}

	int size3 = new_Oper.size();
	double REZ = new_Expr[0]; // Создаем переменную для результата
	for (int i = 0; i < size3; i++)
	{
		if (Operators.at(new_Oper[i]) == 1) // Если +
		{
			REZ += new_Expr[i + 1]; // Увеличиваем результат на правый операнд
		}

		if (Operators.at(new_Oper[i]) == 2) // Если -
		{
			REZ -= new_Expr[i + 1]; // Уменьшаем результат на правый операнд
		}
	}

	return REZ;
}

int main()
{
	stringstream IN;
	for (const string& T : Unit_Test)
	{
		IN << T << endl;
	}
	string Expression;
	while (getline(IN, Expression))
	{
		try
		{
			cout << "Result is " << Calculate(Expression) << endl;
		}
		catch (runtime_error& er)
		{
			cout << er.what() << endl;
		}
		catch (invalid_argument& er)
		{
			cout << er.what() << endl;
		}
	}
	return 0;
}