#ifndef DATETIME_H
#define DATETIME_H


#include "str.h"


enum class Weekday {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};

class TimeDiff {
public:

	TimeDiff(int days, int hours, int minutes, int seconds);

	[[nodiscard]] int days() const;
	[[nodiscard]] int hours() const;
	[[nodiscard]] int minutes() const;
	[[nodiscard]] int seconds() const;
	[[nodiscard]] int total_hours() const;
	[[nodiscard]] int total_minutes() const;
	[[nodiscard]] int total_seconds() const;

	void add_seconds(int nb_seconds);

	bool operator==(const TimeDiff& other) const;
	bool operator!=(const TimeDiff& other) const;
	bool operator<(const TimeDiff& other) const;
	bool operator<=(const TimeDiff& other) const;
	bool operator>(const TimeDiff& other) const;
	bool operator>=(const TimeDiff& other) const;
	TimeDiff operator+(const TimeDiff& other) const;
	TimeDiff operator-(const TimeDiff& other) const;

private:
	int total_seconds_;
};

class Time {
public:

	virtual ~Time() = default;

	Time(int hour, int minute, int second);

	[[nodiscard]] int get_total_seconds() const;

	[[nodiscard]] int hour() const;
	[[nodiscard]] int minute() const;
	[[nodiscard]] int second() const;

	void add_hours(int nb_hours);
	void add_minutes(int nb_minutes);
	virtual void add_seconds(int nb_seconds);

	[[nodiscard]] virtual Str to_str() const;

	bool operator==(const Time& other) const;
	bool operator!=(const Time& other) const;
	bool operator<(const Time& other) const;
	bool operator<=(const Time& other) const;
	bool operator>(const Time& other) const;
	bool operator>=(const Time& other) const;
	TimeDiff operator-(const Time& other) const;

protected:
	int total_seconds_;
};


class Date {
public:

	virtual ~Date() = default;

	static const int days_in_months[];

	Date(int year, int month, int day);

	[[nodiscard]] int get_total_days() const;

	[[nodiscard]] int year() const;
	[[nodiscard]] int month() const;
	[[nodiscard]] int day() const;
	[[nodiscard]] Weekday weekday() const;
	[[nodiscard]] virtual bool is_leapyear() const;

	void add_days(int nb_days);

	[[nodiscard]] Date next() const;
	[[nodiscard]] Date prev() const;

	static bool is_leapyear(int year);

	[[nodiscard]] virtual Str to_str() const;

	bool operator==(const Date& other) const;
	bool operator!=(const Date& other) const;
	bool operator<(const Date& other) const;
	bool operator<=(const Date& other) const;
	bool operator>(const Date& other) const;
	bool operator>=(const Date& other) const;
	TimeDiff operator-(const Date& other) const;
	Date& operator++();    // Pre-increment
	Date operator++(int);  // Post-increment
	Date& operator--();    // Pre-decrement
	Date operator--(int);  // Post-decrement

	[[nodiscard]] static int days_in_month(int year, int month) ;

private:

	int total_days_;

	static int calculate_year(int& total_days);
	static int calculate_month(int& total_days);
};


class DateTime final : public Date, public Time {
public:

	DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);
	DateTime(const Date& dt, const Time& tm);

	void add_seconds(int nb_seconds) override;
	[[nodiscard]] bool is_leapyear() const override;

	bool operator==(const DateTime& other) const;
	bool operator!=(const DateTime& other) const;
	bool operator<(const DateTime& other) const;
	bool operator<=(const DateTime& other) const;
	bool operator>(const DateTime& other) const;
	bool operator>=(const DateTime& other) const;
	TimeDiff operator-(const DateTime& other) const;
	DateTime operator+(const TimeDiff& other) const;
	DateTime operator-(const TimeDiff& other) const;

	static bool is_leapyear(int year);

	[[nodiscard]] Str to_str() const override;
};


void check_integer_range(int value, const Str& error_message, int min, int max);


#endif // DATETIME_H
