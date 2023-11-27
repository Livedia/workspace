// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include <cstdio>
#include <iostream>
#include <string>
#include "../math/math.h"

int main(int argc, char** argv) {
  if (argc < 3) {
    printf("Usage: ./demo base exponent\n");
    return -1;
  }

  double base = std::stold(argv[1]);
  int exponent = std::stoi(argv[2]);

  double result = MyPower(base, exponent);
  printf("%g ^ %d = %g", base, exponent, result);

  return 0;
}
