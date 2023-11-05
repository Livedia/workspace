#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

class A {
  inline static size_t c2_count_ = 0;

 private:
  int i;
  std::string s;

 public:
  A() = default;
  A(int _i, std::string _s) : i(_i), s(_s) {
    ++c2_count_;
    std::cout << "call A two param constructor\n";
  }

  friend std::ostream& operator<<(std::ostream& os, const A& a);
};

std::ostream& operator<<(std::ostream& os, const A& a) { return os << "i is:" << a.i << ", s is:" << a.s; }

template <typename T>
void PrintContainer(const T& container) {
  std::cout << "---- PrintContainer\n";
  for (const auto& it : container) {
    std::cout << it << std::endl;
  }
}

void TestVector() {
  // 使用{}来初始化数组的时候，应该是用到了 reserve 所以他的 size 和 capacity 都是一样的大小
  std::vector<int> iv{1, 2, 3, 4, 5, 6, 7};
  std::cout << "init iv size is:" << iv.size() << std::endl;
  std::cout << "init iv max size is:" << iv.max_size() << std::endl;
  std::cout << "init iv capacity size is:" << iv.capacity() << std::endl;
  // 插入一个元素后，capacity 2 倍增长了
  iv.emplace_back(8);
  std::cout << "after emplace 8 iv size2 is:" << iv.size() << std::endl;
  std::cout << "after emplace 8 iv capacity size is:" << iv.capacity() << std::endl;

  std::vector<int> iv2;
  for (size_t i = 0; i < 10; ++i) {
    iv2.emplace_back(i);
    std::cout << "add " << i << " for iv2, size is:" << iv2.size() << ", capacity is:" << iv2.capacity() << std::endl;
  }

  PrintContainer(iv);

  std::vector<A> va;
  A aa(2, "t2");
  A a = {1, "t1"};
  va.push_back(a);
  va.emplace_back(aa);
  // A 必须要有对应参数的构造函数的时候才能这么用
  va.emplace_back(3, "t3");

  PrintContainer(va);
}

int main() {
  std::cout << "--- this is start program! ---" << std::endl;

  TestVector();
  return 0;
}
