// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
// 两个队列实现一个栈

#include <iostream>
#include <queue>
#include <stack>

template <typename T>
class MockStack {
 private:
  std::queue<T> q1;
  std::queue<T> q2;

 public:
  void Push(const T& e) { q1.push(e); }

  size_t size() { return q1.size(); }

  T Top() {
    T ret;
    size_t size = q1.size();
    for (size_t i = 0; i < size; ++i) {
      if (i == size - 1) {
        ret = q1.front();
      }
      q2.push(q1.front());
      q1.pop();
    }
    for (size_t i = 0; i < size; ++i) {
      q1.push(q2.front());
      q2.pop();
    }
    return ret;
  }

  void Pop() {
    size_t size = q1.size();
    for (size_t i = 0; i < size; ++i) {
      if (i != size - 1) {
        q2.push(q1.front());
        q1.pop();
      } else {
        q1.pop();
      }
    }
    size = q2.size();
    for (size_t i = 0; i < size; ++i) {
      q1.push(q2.front());
      q2.pop();
    }
  }
};

void MockStackTest() {
  std::cout << "## MockStackTest ##\n";
  MockStack<int> s;
  s.Push(1);
  s.Push(2);
  s.Push(3);
  std::cout << "1-- mock stack size is:" << s.size() << "mock stack top is:" << s.Top() << std::endl;
  s.Push(4);
  std::cout << "2-- mock stack size is:" << s.size() << "mock stack top is:" << s.Top() << std::endl;
  s.Pop();
  s.Pop();
  std::cout << "3-- mock stack size is:" << s.size() << "mock stack top is:" << s.Top() << std::endl;
}

template <typename T>
class MockQueue {
 private:
  std::stack<T> s1;
  std::stack<T> s2;

 public:
  size_t size() { return s1.size(); }

  void Push(const T& e) { s1.push(e); }

  T Front() {
    while (!s1.empty()) {
      s2.push(s1.top());
      s1.pop();
    }

    T ret = s2.top();

    while (!s2.empty()) {
      s1.push(s2.top());
      s2.pop();
    }

    return ret;
  }

  void Pop() {
    while (!s1.empty()) {
      s2.push(s1.top());
      s1.pop();
    }
    s2.pop();
    while (!s2.empty()) {
      s1.push(s2.top());
      s2.pop();
    }
  }
};

void TestMockQueue() {
  std::cout << "## TestMockQueue ##\n";
  MockQueue<int> q;
  q.Push(1);
  q.Push(2);
  q.Push(3);
  std::cout << "1-- mock stack size is:" << q.size() << "front is:" << q.Front() << std::endl;
  q.Push(4);
  std::cout << "2-- mock stack size is:" << q.size() << "front is:" << q.Front() << std::endl;
  q.Pop();
  q.Pop();
  std::cout << "3-- mock stack size is:" << q.size() << "front is:" << q.Front() << std::endl;
}

int main() {
  std::cout << "this is casex\n";
  MockStackTest();
  TestMockQueue();

  return 0;
}
