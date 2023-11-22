// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
//

#include <_types/_uint64_t.h>
#include <cstddef>
#include <iostream>
#include <string>
#include "carbon/numeric/strtox.h"

using std::string;

int main() {
  std::cout << "this is case3\n";

  std::string s1("123456");
  std::string s2("12345ss");
  std::string s3("12.3456");
  std::string s4("123.123.13.1");

  uint64_t u64_1 = 0;
  uint64_t u64_2 = 0;

  float f1 = 0;
  float f2 = 0;

  u64_1 = carbon::safe_strtoi(s1, &u64_1) ? u64_1 : 0;
  u64_2 = carbon::safe_strtoi(s2, &u64_2) ? u64_2 : 0;
  f1 = carbon::safe_strtof(s3, &f1) ? f1 : 0;
  f2 = carbon::safe_strtof(s4, &f2) ? f2 : 0;

  std::cout << "after tranforming s1:" << u64_1 << std::endl;
  std::cout << "after tranforming s2:" << u64_2 << std::endl;
  std::cout << "after tranforming s3:" << f1 << std::endl;
  std::cout << "after tranforming s4:" << f2 << std::endl;

  return 0;
}
