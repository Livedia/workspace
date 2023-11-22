// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
// BM1 链表内指定区间反转

#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

using std::string;
class Solution {
 public:
  /**
   * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
   *
   *
   * @param s string字符串
   * @param n int整型
   * @return string字符串
   */
  string trans(string s, int n) {
    // write code here
    std::vector<string> segs;
    string t;
    int begin = 0;
    for (int i = 0; i < n; ++i) {
      if (s[i] >= 65 && s[i] <= 90) {
        s[i] += 32;
      } else if (s[i] >= 97 && s[i] <= 122) {
        s[i] -= 32;
      }

      if (s[i] == ' ') {
        t = s.substr(begin, i - begin);
        std::cout << "-- t is:" << t << std::endl;
        segs.emplace_back(t);
        segs.emplace_back(" ");
        begin = i + 1;
      }
    }

    string end = s.substr(begin, n - begin + 1);
    segs.emplace_back(end);

    string ret;
    for (auto it = segs.rbegin(); it != segs.rend(); ++it) {
      ret += *it;
    }
    return ret;
  }
};

int main() {
  std::cout << "this is case1\n";
  Solution s;

  std::cout << s.trans("This is a sample", 16);

  return 0;
}
