#ifndef STROKA_H
#define STROKA_H


#include <iostream>
#include <cstring>

class Str {
public:

  explicit Str(const char* str="");
  explicit Str(char);
  explicit Str(int);
  explicit Str(double);

  Str(const Str&);
  Str(Str&&) noexcept;

  ~Str() { delete[] data_; }

  Str& operator=(const Str&);
  Str& operator=(Str&&) noexcept;

  Str& operator+=(const Str&);
  char operator[](size_t) const;

  [[nodiscard]] size_t length() const { return std::strlen(data_); }
  [[nodiscard]] size_t size() const { return size_; }
  [[nodiscard]] char* get_char() const { return data_; }

  Str& to_upper();
  [[nodiscard]] Str upper() const;

private:

  char* data_;
  size_t size_;

  void swap(Str& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
  }
};

Str operator+(const Str&, const Str&);
std::ostream& operator<< (std::ostream&, const Str&);
std::istream& operator>> (std::istream&, Str&);

bool operator==(const Str&, const Str&);
bool operator!=(const Str&, const Str&);
bool operator<(const Str&, const Str&);
bool operator<=(const Str&, const Str&);
bool operator>(const Str&, const Str&);
bool operator>=(const Str&, const Str&);


#endif // STROKA_H
