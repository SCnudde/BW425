#include "Exit_Codes.h"
#include "Date.h"


int main()
{
	using DateSupport = std::int64_t;

	try
	{
		std::setlocale(LC_ALL, "Russian_Russia.1251");

		our::Date<DateSupport> date, date1(date), date2;
		std::cout << "Current date: ";
		our::Date<DateSupport> Date();


		std::cout << "Текущая дата: " << date1 << "\nВведите дату: ";

		std::cin >> date;
		std::cout << "Введена дата: " << date;

		date2 = date;
		std::cout << "\nДата от оператора =: " << date2;

		return our::EXIT_CODES::SUCCESS;
	}
	catch (const char* message)
	{
		std::cerr << message;
		return our::EXIT_CODES::INVALID_DATE;
	}
}
