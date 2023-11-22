// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include <iostream>
#include <mutex>
#include <string>
#include <vector>

using std::string;

string convertString(string str) {
  string ret;
  bool flag = true;
  for (char i : str) {
    if (i != '(' && flag) {
      ret += i;
    } else if (flag) {
      flag = false;
      ret += "(*)";
    }
    if (i == ')') {
      flag = true;
    }
  }
  return ret;
}

std::vector<int> product(const std::vector<int>& arr) {
  std::vector<int> ret;
  int t = 1;
  for (const auto& i : arr) {
    t *= i;
  }

  ret.reserve(arr.size());
  for (const auto& i : arr) {
    ret.emplace_back(t / i);
  }
  return ret;
}

namespace test {

class A {};

class Node1 {
  inline static std::mutex m_;

 private:
  A data_;

 public:
  void setData(const A& data) {
    std::lock_guard<std::mutex> lock(m_);
    data_ = data;
  }

  const A& getData() const {
    std::lock_guard<std::mutex> lock(m_);
    return data_;
  }
};

}  // namespace test

int main() {
  std::cout << "this is interview\n";

  std::cout << convertString("ABC(xxx)DEF(xxx)");

  return 0;
}
