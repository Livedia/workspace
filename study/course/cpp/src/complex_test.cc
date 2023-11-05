// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include "course/cpp/include/complex.h"
#include <iostream>

using namespace mystudy;

void TestComplex() {
  Complex c1(2, 1);
  Complex c2(4, 0);

  std::cout << c1 << std::endl;
  std::cout << c2 << std::endl;

  std::cout << c1 + c2 << std::endl;
  std::cout << c1 - c2 << std::endl;
  std::cout << c1 * c2 << std::endl;
  std::cout << c1 / 2 << std::endl;

  std::cout << conj(c1) << std::endl;
  std::cout << norm(c1) << std::endl;
  std::cout << polar(10, 4) << std::endl;

  std::cout << (c1 += c2) << std::endl;

  std::cout << (c1 == c2) << std::endl;
  std::cout << (c1 != c2) << std::endl;
  std::cout << +c2 << std::endl;
  std::cout << -c2 << std::endl;

  std::cout << (c2 - 2) << std::endl;
  std::cout << (5 + c2) << std::endl;
}

int main() {
  TestComplex();
  return 0;
}
