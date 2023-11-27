// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include <iostream>
#include "../add/add.h"

int main() {
  std::cout << "please input two numbers:\n";
  int a, b;
  std::cin >> a;
  std::cin >> b;

  MyAdd(a, b);
  return 0;
}
