// Author: Weida Lei (edward.lei@vipshop.com)
//

#pragma once

#include <iostream>

class Complex;
Complex& __doapl(Complex* ths, const Complex& r);
Complex& __doami(Complex* ths, const Complex& r);
Complex& __doaml(Complex* ths, const Complex& r);

class Complex {
 public:
  Complex(double re, double im) : re_(re), im_(im) {}

  const double& rel() const { return re_; }
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
