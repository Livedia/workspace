// Author: Weida Lei (edward.lei@vipshop.com)
//

#pragma once

#include <cmath>
#include <iostream>
namespace mystudy {

class Complex;
// 前置声明
// do a plus
Complex& __doapl(Complex* ths, const Complex& r);
// do a minus
Complex& __doami(Complex* ths, const Complex& r);
// do a multiply
Complex& __doaml(Complex* ths, const Complex& r);

class Complex {
 public:
  Complex(double re, double im) : re_(re), im_(im) {}

  const double& real() const { return re_; }
  const double& imag() const { return im_; }

  Complex& operator+=(const Complex& r);
  Complex& operator-=(const Complex& r);
  Complex& operator*=(const Complex& r);
  Complex& operator/=(const Complex& r);

  friend Complex& __doapl(Complex* ths, const Complex& r);
  friend Complex& __doami(Complex* ths, const Complex& r);
  friend Complex& __doaml(Complex* ths, const Complex& r);

 private:
  double re_;
  double im_;
};

inline Complex& __doapl(Complex* ths, const Complex& r) {
  ths->im_ += r.im_;
  ths->re_ += r.re_;

  return *ths;
}

inline Complex& Complex::operator+=(const Complex& r) { return __doaml(this, r); }

inline Complex& __doami(Complex* ths, const Complex& r) {
  ths->im_ -= r.im_;
  ths->re_ -= r.re_;

  return *ths;
}

inline Complex& Complex::operator-=(const Complex& r) { return __doami(this, r); }

inline Complex& __doaml(Complex* ths, const Complex& r) {
  double f = ths->re_ * r.re_ - ths->im_ * r.im_;
  ths->im_ = ths->im_ * r.re_ + ths->re_ * r.im_;
  ths->re_ *= f;

  return *ths;
}

inline Complex& Complex::operator*=(const Complex& r) { return __doaml(this, r); }

inline double imag(const Complex& c) { return c.imag(); }

inline double real(const Complex& c) { return c.real(); }

inline Complex operator+(const Complex& lhd, const Complex& rhd) {
  return (Complex(lhd.real() + rhd.real(), lhd.imag() + lhd.imag()));
}

inline Complex operator+(const Complex& lhd, double rhd) { return (Complex(lhd.real() + rhd, lhd.imag())); }

inline Complex operator+(double lhd, const Complex& rhd) { return (Complex(rhd.real() + lhd, rhd.imag())); }

inline Complex operator-(const Complex& lhd, const Complex& rhd) {
  return (Complex(lhd.real() - rhd.real(), lhd.imag() - lhd.imag()));
}

inline Complex operator-(const Complex& lhd, double rhd) { return (Complex(lhd.real() - rhd, lhd.imag())); }

inline Complex operator-(double lhd, const Complex& rhd) { return (Complex(lhd - rhd.real(), -rhd.imag())); }

inline Complex operator*(const Complex& lhd, const Complex& rhd) {
  return Complex(lhd.real() * rhd.real() - lhd.imag() * rhd.imag(), lhd.real() * rhd.imag() + lhd.imag() * rhd.real());
}

inline Complex operator*(const Complex& lhd, double rhd) { return (Complex(lhd.real() * rhd, lhd.imag() * rhd)); }

inline Complex operator*(double lhd, const Complex& rhd) { return (Complex(lhd * rhd.real(), lhd * rhd.imag())); }

inline Complex operator/(const Complex& lhd, double rhd) { return Complex(lhd.real() / rhd, lhd.imag() / rhd); }

// 正负号
inline Complex operator+(const Complex& c) { return c; }

inline Complex operator-(const Complex& c) { return Complex(-c.real(), -c.real()); }

inline bool operator==(const Complex& lhd, const Complex& rhd) {
  return lhd.imag() == rhd.imag() && lhd.real() == rhd.real();
}

inline bool operator==(const Complex& lhd, double rhd) { return lhd.imag() == 0 && lhd.real() == rhd; }

inline bool operator==(double lhd, const Complex& rhd) { return 0 == rhd.imag() && lhd == rhd.real(); }

inline bool operator!=(const Complex& x, const Complex& y) { return real(x) != real(y) || imag(x) != imag(y); }

inline bool operator!=(const Complex& x, double y) { return real(x) != y || imag(x) != 0; }

inline bool operator!=(double x, const Complex& y) { return x != real(y) || imag(y) != 0; }

// 极坐标转 Complex
inline Complex polar(double r, double t) { return Complex(r * cos(t), r * sin(t)); }
// 共轭复数
inline Complex conj(const Complex& x) { return Complex(real(x), -imag(x)); }
// 点乘
inline double norm(const Complex& x) { return real(x) * real(x) + imag(x) * imag(x); }

std::ostream& operator<<(std::ostream& os, const Complex& x) { return os << '(' << real(x) << ',' << imag(x) << ')'; }
}  // namespace mystudy
