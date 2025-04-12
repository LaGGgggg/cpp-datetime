#include <cstring>

#include "str.h"


Str::Str(const char* str) {
  size_ = std::strlen(str) + 1;
  data_ = new char[size_];
  std::memcpy(data_, str, size_);
}
Str::Str(const char ch) {
  size_ = 2;  // 2 = 1 char + 1 termination symbol
  data_ = new char[size_];
  data_[0] = ch;
  data_[1] = '\0';
}
Str::Str(const int num) {
  size_ = 12;  // 12 = max int length + 1 termination symbol
  data_ = new char[size_];
  std::snprintf(data_, size_, "%d", num);
}
Str::Str(const double num) {
  size_ = 32;  // 32 = max double length + 1 termination symbol
  data_ = new char[size_];
  std::snprintf(data_, size_, "%.2f", num);
}

Str::Str(const Str& other) {
  size_ = other.size();
  data_ = new char[size_];
  std::memcpy(data_, other.get_char(), size_);
}
Str::Str(Str&& other) noexcept : data_(nullptr), size_(0) {
  swap(other);
}

Str& Str::operator=(const Str& other) {

  if (this != &other){
    Str tmp(other);
    swap(tmp);
  }

  return *this;
}
Str& Str::operator=(Str&& other) noexcept {

  if (this != &other) {
    swap(other);
  }

  return *this;
}

char Str::operator[](const size_t index) const {

  if (index >= size()){
    throw std::out_of_range("Index out of range");
  }

  return get_char()[index];
}
Str& Str::operator+=(const Str& other) {

  const size_t len_this = length();
  const size_t len_other = other.length();

  if (len_this + len_other + 1 > size()) {

    size_ = len_this + len_other + 1;

    auto* tmp = new char[size()];
    std::memcpy(tmp, get_char(), len_this);

    delete[] get_char();
    data_ = tmp;
  }

  std::memcpy(data_ + len_this, other.get_char(), len_other + 1);

  return *this;
}

Str& Str::to_upper() {

  for (size_t i = 0; i < length(); ++i) {
    data_[i] = static_cast<char>(std::toupper(data_[i]));
  }

  return *this;
}
Str Str::upper() const {

  auto* tmp = new char[size()];

  for (size_t i = 0; i < length(); ++i) {
    tmp[i] = static_cast<char>(std::toupper(data_[i]));
  }
  tmp[size() - 1] = '\0';

  Str result(tmp);
  delete[] tmp;

  return result;
}


Str operator+(const Str& left, const Str& right) {
  Str result(left);
  result += right;
  return result;
}
std::ostream& operator<<(std::ostream& out, const Str& string) {
  out << string.get_char();
  return out;
}
std::istream& operator>> (std::istream& in, Str& string) {

  char buffer[1024];
  in >> buffer;
  string = Str(buffer);

  return in;
}

bool operator==(const Str& left, const Str& right) {
  return std::strcmp(left.get_char(), right.get_char()) == 0;
}
bool operator!=(const Str& left, const Str& right) {
  return std::strcmp(left.get_char(), right.get_char()) != 0;
}
bool operator<(const Str& left, const Str& right) {
  return std::strcmp(left.get_char(), right.get_char()) < 0;
}
bool operator<=(const Str& left, const Str& right) {
  return std::strcmp(left.get_char(), right.get_char()) <= 0;
}
bool operator>(const Str& left, const Str& right) {
  return std::strcmp(left.get_char(), right.get_char()) > 0;
}
bool operator>=(const Str& left, const Str& right) {
  return std::strcmp(left.get_char(), right.get_char()) >= 0;
}
