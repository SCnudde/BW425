#pragma once

#include <cstdint>
#include <concepts>
#include <iostream>
#include <fstream>
#include <chrono>
#include <type_traits>

namespace our
{
	template<typename T>
	concept IntegerType = std::integral<T> && !std::same_as<T, bool>;

	static const std::uint8_t daySizeInBits = 5, monthSizeInBits = 4;

	const constexpr std::uint8_t DateSupportMinimalSizeInBytes
			= (daySizeInBits + monthSizeInBits + 8) >> 3;

	template<typename T>
	concept DateSupport = IntegerType<T>
			&& sizeof(T) >= DateSupportMinimalSizeInBytes;

	template<DateSupport T>
	class Date;

	template<DateSupport T>
	std::ostream& operator<<(std::ostream& output, const Date<T>& date);

	template<DateSupport T>
	std::istream& operator>>(std::istream& input, Date<T>& date);


	template<DateSupport T>
	std::ofstream& operator<<(std::ofstream& output, const Date<T>& date);

	template<DateSupport T>
	std::ifstream& operator>>(std::ifstream& input, Date<T>& date);
}

template<our::DateSupport T>
class our::Date
{
public:
	// constructors
	Date();
	explicit Date(const Date& Date) = default;//оператор копирования

	explicit Date(const uint8_t day);
	explicit Date(const uint8_t day, const uint8_t month);
	explicit Date(const uint8_t day, const uint8_t month, const T year);

	Date& operator=(const Date& Date) = default;

	T operator-() const;
	T operator-(const Date& Date) const;
	Date& operator-=(const T nDay);
	Date& operator--();
	Date operator--(int); // постфиксный

	Date operator+(const T nDay) const;
	Date& operator+=(const T nDay);
	Date& operator++();
	Date operator++(int); // постфиксный

	std::strong_ordering operator<=>(const Date& date);

	bool operator==(const Date& date) const;
	

	bool operator!=(const Date& date) const;

	bool operator>(const Date& date) const;
	bool operator<(const Date& date) const;

	bool operator>=(const Date& date) const;
	bool operator<=(const Date& date) const;

	bool operator!() const;
	operator bool() const;

	friend std::ostream& operator<< (std::ostream& output, const Date<T>& date);
	{
		output << date.day_ << '/' << date.month_ << '/' << date.year_;
		return output;
	}

	friend std::istream& operator>> (std::istream& input, Date<T>& date);
	{
		int day, month, year;
		input >> day >> month >> year;

		if (Date<T>::isValid(day, month, year))
		{
			date.day_ = day;
			date.month_ = month;
			date.year_ = year;
		}
		else
			throw "\n\033[31mInvalid date!\033[0m\a\n";

		return input;
	};

	/*friend std::ofstream& operator<< <T>
		(std::ofstream& output, const Date<T>& date);

	friend std::ifstream& operator>> <T>(std::ifstream& input, Date<T>& date);*/

	static bool isLeapYear(const T year);

	static bool isCorrectDay(const uint8_t day);
	static bool isCorrectMonth(const uint8_t month);

	static bool isValid(const std::uint8_t day, const std::uint8_t month,
				const T year);

	// getters
	uint8_t day() const;
	uint8_t month() const;
	T year() const;

	// setters
	Date& day(const uint8_t day);
	Date& month(const uint8_t month);
	Date& year(const T year);
	Date& date(const uint8_t day, const uint8_t month, const T year);

private:
	std::make_unsigned_t<T> day_ : daySizeInBits;
	std::make_unsigned_t<T> month_ : monthSizeInBits;
	T year_ : ( sizeof(T) << 3 ) - daySizeInBits - monthSizeInBits;
};


template<our::DateSupport T>
our::Date<T>::Date()
{
	auto const now = std::chrono::system_clock::now();
	auto const days_since_epoch = floor<std::chrono::days>(now);
	std::chrono::year_month_day today = std::chrono::year_month_day(days_since_epoch);

	std::chrono::day day = today.day();
	day_ = static_cast<unsigned>(day);

	std::chrono::month month = today.month();
	month_ = static_cast<unsigned>(month);

	std::chrono::year year = today.year();
	year_ = static_cast<signed>(year);

	
}

template<our::DateSupport T>
inline std::ostream& our::operator<< <T>(std::ostream& ostream,
	const our::Date<T>& date)
{
	ostream << date.day_ << '/' << date.month_ << '/' << date.year_;
	return ostream;
}

template<our::DateSupport T>
std::istream& our::operator>> <T>(std::istream& istream,
	our::Date<T>& date)
{
	int day, month, year;
	istream >> day >> month >> year;

	if (Date<T>::isValid(day, month, year))
	{
		date.day_ = day;
		date.month_ = month;
		date.year_ = year;
	}
	else
		throw "\n\033[31mInvalid date!\033[0m\a\n";

	return istream;
}

template<our::DateSupport T>
bool our::Date<T>::operator== (const our::Date<T>& date) const
{
	return day_ == date.day_ && month_ == date.month_ && year_ == date.year_;
}

template<our::DateSupport T>
bool our::Date<T>::isValid(const std::uint8_t day, const std::uint8_t month,
			const T year)
{
	if (month < 1 || 12 < month)
		return false;

	const int monthsDuration[12] =
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (day < 1 || monthsDuration[month - 1] < day)
		return false;

	if (!isLeapYear(year) && day > 28)
		return false;

	return true;
}

template<our::DateSupport T>
std::ostream& operator<<(std::ostream& output, const our::Date<T>& date)
{
	output << date.day_ << '/' << date.month_ << '/' << date.year_;
	return output;
}

template<our::DateSupport T>
std::istream& operator>> (std::istream& input, const our::Date<T>& date)
{
	int day, month, year;
	input >> day >> month >> year;

	if (Date<T>::isValid(day, month, year))
	{
		date.day_ = day;
		date.month_ = month;
		date.year_ = year;
	}
	else
		throw "\n\033[31mInvalid date!\033[0m\a\n";

	return input;
};


template<our::DateSupport T>
std::strong_ordering our::Date<T>::operator<=>(const Date& date)
{

}

//template<our::DateSupport T>
//inline uint8_t our::Date<T>::day() const
//{
//	check();
//	return day_;
//}
//
//template<our::DateSupport T>
//inline uint8_t our::Date<T>::month() const
//{
//	check();
//	return month_;
//}
//
//template<our::DateSupport T>
//inline T our::Date<T>::year() const
//{
//	check();
//	return year_;
//}

// our::Date Date1;
// std::cout << Date1.year(2024);

//template<our::DateSupport T>
//inline our::Date<T>& our::Date<T>::year(const T year)
//{
//	year_ = year;
//	return *this;
//}
//
//template<our::DateSupport T>
//inline our::Date<T>::operator bool() const
//{
//	return isValid(day_, month_, year_);
//}
//
//template<our::DateSupport T>
//inline void our::Date<T>::check() const
//{
//	if (!(*this))
//		throw "\033[31mInvalid date!\a\033[0m\n";
//}

template<our::DateSupport T>
bool our::Date<T>::isLeapYear(const T year)
{
	// 4.10.1582
	if (1582 < year)
		if (year % 400 == 0 || (year & 3) == 0 && year % 100 != 0)
			return true;
		else
			return false;
	else
		if ((year & 3) == 0)
			return true;
		else
			return false;
}
