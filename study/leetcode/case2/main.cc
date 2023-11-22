// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
// BM2 链表内指定区间反转
// https://www.nowcoder.com/practice/b58434e200a648c589ca2063f1faf58c?tpId=295&tqId=654&ru=%2Fpractice%2Fc3120c1c1bc44ad986259c0cf0f0b80e&qru=%2Fta%2Fformat-top101%2Fquestion-ranking&sourceUrl=%2Fexam%2Foj

#include <iostream>
#include <list>
#include <vector>

struct ListNode {
  int val;
  struct ListNode* next{};
  explicit ListNode(int x) : val(x) {}
};
class Solution {
 public:
  /**
   * 代码中的类名、方法名、参数名已经指定，请勿修改，直接返回方法规定的值即可
   *
   *
   * @param head ListNode类
   * @param m int整型
   * @param n int整型
   * @return ListNode类
   */
  ListNode* reverseBetween(ListNode* head, int m, int n) {
    // write code here
    // 时间复杂度O(N)，空间复杂度O(1)

    auto* dummy = new ListNode(-1);
    dummy->next = head;  // 记住头

    ListNode* pre = dummy;
    for (int i = 1; i < m; ++i) pre = pre->next;  // 走到要翻转的前一位置
    ListNode *cur = pre->next, *nex;

    for (int i = m; i < n; ++i) {
      nex = cur->next;
      cur->next = nex->next;
      nex->next = pre->next;
      pre->next = nex;
    }
    return dummy->next;
  }
};

int main() {
  std::cout << "this is case2\n";

  return 0;
}
