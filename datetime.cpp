#include <stdexcept>
#include <sstream>
#include <limits>
#include <iomanip>

#include "datetime.h"
#include "str.h"


void check_integer_range(const int value, const std::string& name, const int min, const int max) {
  if (value < min || value > max) {
    std::ostringstream oss;
    oss << name << " must be between " << min << " and " << max;
    throw std::invalid_argument(oss.str());
  }
}


TimeDiff::TimeDiff(
  const int days, const int hours, const int minutes, const int seconds
) : total_seconds_(((days * 24 + hours) * 60 + minutes) * 60 + seconds) {

  const Str error_message = Str("Time value is invalid: ") + Str(hours) + Str(":") + Str(minutes) + Str(":") + Str(seconds);

  check_integer_range(hours, "hour", -23, 23);
  check_integer_range(minutes, "minute", -59, 59);
  check_integer_range(seconds, "second", -59, 59);

  if ((days < 0 && (hours > 0 || minutes > 0 || seconds > 0)) ||
      (days > 0 && (hours < 0 || minutes < 0 || seconds < 0))) {
    throw std::invalid_argument("all arguments must have the same sign");
  }
}

int TimeDiff::total_seconds() const {
  return total_seconds_;
}
int TimeDiff::total_minutes() const {
  return total_seconds() / 60;
}
int TimeDiff::total_hours() const {
  return total_seconds() / (60 * 60);
}

int TimeDiff::days() const {
  return total_seconds() / (24 * 60 * 60);
}
int TimeDiff::hours() const {
  return total_seconds() / (60 * 60) % 24;
}
int TimeDiff::minutes() const {
  return total_seconds() / 60 % 60;
}
int TimeDiff::seconds() const {
  return total_seconds() % 60;
}

bool TimeDiff::operator==(const TimeDiff& other) const {
  return total_seconds() == other.total_seconds();
}
bool TimeDiff::operator!=(const TimeDiff& other) const {
  return !(*this == other);
}
bool TimeDiff::operator<(const TimeDiff& other) const {
  return total_seconds() < other.total_seconds();
}
bool TimeDiff::operator<=(const TimeDiff& other) const {
  return total_seconds() <= other.total_seconds();
}
bool TimeDiff::operator>(const TimeDiff& other) const {
  return total_seconds() > other.total_seconds();
}
bool TimeDiff::operator>=(const TimeDiff& other) const {
  return total_seconds() >= other.total_seconds();
}

void TimeDiff::add_seconds(const int nb_seconds) {
  total_seconds_ += nb_seconds;
}

TimeDiff TimeDiff::operator+(const TimeDiff& other) const {

  TimeDiff result(0, 0, 0, 0);

  result.add_seconds(total_seconds() + other.total_seconds());

  return result;
}
TimeDiff TimeDiff::operator-(const TimeDiff& other) const {
  return *this + TimeDiff(-other.days(), -other.hours(), -other.minutes(), -other.seconds());
}


Time::Time(const int hour, const int minute, const int second) : total_seconds_((hour * 60 + minute) * 60 + second) {

  const Str error_message = Str("Time value is invalid: ") + (hour < 10 ? Str("0") : Str("")) + Str(hour) + Str(":")
    + (minute < 10 ? Str("0") : Str("")) + Str(minute) + Str(":") + (second < 10 ? Str("0") : Str("")) + Str(second);

  check_integer_range(hour, "hour", 0, 23);
  check_integer_range(minute, "minute", 0, 59);
  check_integer_range(second, "second", 0, 59);
}

int Time::get_total_seconds() const {
  return total_seconds_;
}

int Time::hour() const {
  return get_total_seconds() / (60 * 60);
}
int Time::minute() const {
  return get_total_seconds() / 60 % 60;
}
int Time::second() const {
  return get_total_seconds() % 60;
}

void Time::add_seconds(const int nb_seconds) {

  total_seconds_ += nb_seconds;

  if (total_seconds_ < 0) {
    total_seconds_ = 0;
  } else if (total_seconds_ > 24 * 60 * 60 - 1) {
    total_seconds_ = 24 * 60 * 60 - 1;
  }
}
void Time::add_minutes(const int nb_minutes) {
  add_seconds(nb_minutes * 60);
}
void Time::add_hours(const int nb_hours) {
  add_minutes(nb_hours * 60);
}

bool Time::operator==(const Time &other) const {
  return get_total_seconds() == other.get_total_seconds();
}
bool Time::operator!=(const Time &other) const {
  return !(*this == other);
}
bool Time::operator<(const Time &other) const {
  return get_total_seconds() < other.get_total_seconds();
}
bool Time::operator<=(const Time &other) const {
  return *this < other || *this == other;
}
bool Time::operator>(const Time &other) const {
  return !(*this <= other);
}
bool Time::operator>=(const Time &other) const {
  return !(*this < other);
}
TimeDiff Time::operator-(const Time &other) const {

  const int total_seconds = get_total_seconds() - other.get_total_seconds();

  return {0, total_seconds / (60 * 60), total_seconds / 60 % 60, total_seconds % 60};
}

Str Time::to_str() const {
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hour() << ":"
      << std::setw(2) << std::setfill('0') << minute() << ":"
      << std::setw(2) << std::setfill('0') << second();
  return Str(oss.str().c_str());
}

// Date
const int Date::days_in_months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool Date::is_leapyear(const int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::get_total_days() const {
  return total_days_;
}

int Date::days_in_month(const int year, const int month) {

  if (month == 2 && is_leapyear(year)) {
    return 29;
  }

  return days_in_months[month - 1];
}

int Date::calculate_year(int& total_days) {

  int year = 0;

  while (true) {

    const int days_in_year = is_leapyear(year) ? 366 : 365;

    if (total_days < days_in_year) {
      break;
    }

    total_days -= days_in_year;
    ++year;
  }

  return year;
}

int Date::year() const {

  int total_days = get_total_days();

  return calculate_year(total_days);
}

int Date::calculate_month(int &total_days) {

  const int year = calculate_year(total_days);

  int month = 1;

  while (true) {

    const int days_in_month = (month == 2 && is_leapyear(year)) ? 29 : days_in_months[month - 1];

    if (total_days < days_in_month) {
      break;
    }

    total_days -= days_in_month;
    ++month;
  }

  return month;
}


int Date::month() const {

  int total_days = get_total_days();

  return calculate_month(total_days);
}

int Date::day() const {

  int total_days = get_total_days();

  calculate_month(total_days);

  return total_days + 1;
}

Date::Date(const int year, const int month, const int day) {

  check_integer_range(year, "year", 0, std::numeric_limits<int>::max());
  check_integer_range(month, "month", 1, 12);
  check_integer_range(day, "day", 1, days_in_month(year, month));

  total_days_ = day - 1 + year * 365 + year / 4 - year / 100 + year / 400 + (is_leapyear(year) ? 0 : 1);

  for (int i = 0; i < month - 1; ++i) {
    total_days_ += days_in_month(year, i + 1);
  }
}

Weekday Date::weekday() const {
  // Zeller's Congruence algorithm

  int year = this->year();
  int month = this->month();
  const int day = this->day();

  if (month < 3) {
    month += 12;
    year -= 1;
  }

  const int century_year = year % 100;
  const int century = year / 100;

  const int weekday_calculation =
    day + 13 * (month + 1) / 5 + century_year + century_year / 4 + century / 4 + 5 * century;

  switch (weekday_calculation % 7) {
    case 0: return Weekday::SATURDAY;
    case 1: return Weekday::SUNDAY;
    case 2: return Weekday::MONDAY;
    case 3: return Weekday::TUESDAY;
    case 4: return Weekday::WEDNESDAY;
    case 5: return Weekday::THURSDAY;
    case 6: return Weekday::FRIDAY;
    default: throw std::logic_error("Invalid day of week calculation");
  }
}

bool Date::is_leapyear() const {
    return is_leapyear(year());
}

void Date::add_days(int nb_days) {
  total_days_ += nb_days;
}

Date Date::next() const {

  Date next_date = *this;
  next_date.add_days(1);

  return next_date;
}
Date Date::prev() const {

  Date prev_date = *this;
  prev_date.add_days(-1);

  return prev_date;
}

bool Date::operator==(const Date& other) const {
    return year() == other.year() && month() == other.month() && day() == other.day();
}
bool Date::operator!=(const Date& other) const {
    return !(*this == other);
}
bool Date::operator<(const Date& other) const {
  return get_total_days() < other.get_total_days();
}
bool Date::operator<=(const Date& other) const {
    return *this < other || *this == other;
}
bool Date::operator>(const Date& other) const {
    return !(*this <= other);
}
bool Date::operator>=(const Date& other) const {
    return !(*this < other);
}

TimeDiff Date::operator-(const Date& other) const {
  return {get_total_days() - other.get_total_days(), 0, 0, 0};
}
Date& Date::operator++() {
  add_days(1);
  return *this;
}
Date Date::operator++(int) {
  Date tmp = *this;
  add_days(1);
  return tmp;
}
Date& Date::operator--() {
  add_days(-1);
  return *this;
}
Date Date::operator--(int) {
  Date tmp = *this;
  add_days(-1);
  return tmp;
}

Str Date::to_str() const {
  std::ostringstream oss;
  oss << std::setw(4) << std::setfill('0') << year() << "."
      << std::setw(2) << std::setfill('0') << month() << "."
      << std::setw(2) << std::setfill('0') << day();
  return Str(oss.str().c_str());
}

// DateTime
DateTime::DateTime(
  const int year, const int month, const int day, const int hour, const int minute, const int second
) : Date(year, month, day), Time(hour, minute, second) {}
DateTime::DateTime(const Date& dt, const Time& tm) : Date(dt), Time(tm) {}

void DateTime::add_seconds(const int nb_seconds) {

  total_seconds_ += nb_seconds;

  constexpr int seconds_in_day = 24 * 60 * 60;

  if (total_seconds_ >= seconds_in_day) {
    add_days(total_seconds_ / seconds_in_day);
    total_seconds_ %= seconds_in_day;
  } else if (total_seconds_ < 0) {
    add_days(total_seconds_ / seconds_in_day - 1);
    total_seconds_ = seconds_in_day + total_seconds_ % seconds_in_day;
  }
}
bool DateTime::is_leapyear() const {
  return Date::is_leapyear(year());
}

bool DateTime::operator==(const DateTime& other) const {
  return Date::operator==(other) && Time::operator==(other);
}
bool DateTime::operator!=(const DateTime& other) const {
  return !(*this == other);
}
bool DateTime::operator<(const DateTime& other) const {
  return Date::operator<(other) || (Date::operator==(other) && Time::operator<(other));
}
bool DateTime::operator<=(const DateTime& other) const {
  return *this < other || *this == other;
}
bool DateTime::operator>(const DateTime& other) const {
  return !(*this <= other);
}
bool DateTime::operator>=(const DateTime& other) const {
  return !(*this < other);
}
TimeDiff DateTime::operator-(const DateTime& other) const {

  const TimeDiff date_timediff = Date::operator-(other);
  const TimeDiff time_timediff = Time::operator-(other);

  if (time_timediff.total_seconds() < 0) {
    return date_timediff - TimeDiff(1, 0, 0, 0) + (time_timediff + TimeDiff(1, 0, 0, 0));
  }

  return date_timediff + time_timediff;
}
DateTime DateTime::operator+(const TimeDiff &other) const {

  DateTime result = *this;

  result.add_days(other.days());
  result.add_hours(other.hours());
  result.add_minutes(other.minutes());
  result.add_seconds(other.seconds());

  return result;
}
DateTime DateTime::operator-(const TimeDiff &other) const {
  return *this + TimeDiff(-other.days(), -other.hours(), -other.minutes(), -other.seconds());
}

bool DateTime::is_leapyear(const int year) {
  return Date::is_leapyear(year);
}

Str DateTime::to_str() const {
  std::ostringstream oss;
  oss << std::setw(4) << std::setfill('0') << year() << "."
      << std::setw(2) << std::setfill('0') << month() << "."
      << std::setw(2) << std::setfill('0') << day() << " "
      << std::setw(2) << std::setfill('0') << hour() << ":"
      << std::setw(2) << std::setfill('0') << minute() << ":"
      << std::setw(2) << std::setfill('0') << second();
  return Str(oss.str().c_str());
}
