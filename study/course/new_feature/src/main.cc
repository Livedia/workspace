// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include <fstream>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <tuple>

namespace variadic_templates_test {

// 终止时候的调用函数
void PrintX() {}

template <typename T, typename... Types /*模板参数包*/>
void PrintX(const T& first_arg, const Types&... other_args /*函数参数类型包*/) {
  std::cout << "first_arg:" << first_arg << ", ther_args size is:" << sizeof...(other_args) << std::endl;

  PrintX(other_args... /*函数参数包*/);
}

// 这个版本永远都不会被调用
template <typename... Types>
void PrintX(const Types&... args) {
  std::cout << "1111";
  PrintX(args...);
}

// 重写 printf

void MyPrintf(const char* s) {
  while (*s) {
    if (*s == '%' && *(++s) != '%') {
      throw std::runtime_error("invalid format string: missing arguments");
    }
    std::cout << *s++;
  }
}

template <typename T, typename... Args>
void MyPrintf(const char* s, T value, Args... args) {
  while (*s) {
    if (*s == '%' && *(++s) != '%') {
      std::cout << value;
      MyPrintf(++s, args...);
      return;
    }
    std::cout << *s++;
  }
  throw std::logic_error("extra arguments provided to MyPrintf");
}

//----------------------------------------------------

int Maximum(int n) { return n; }

template <typename... Args>
int Maximum(int n, Args... args) {
  return std::max(n, Maximum(args...));
}

//----------------------------------------------------

template <int IDX, int MAX, typename... Args>
struct PRINT_TUPLE {
  static void print(std::ostream& os, const std::tuple<Args...>& t) {
    os << std::get<IDX>(t) << (IDX + 1 == MAX ? "" : ",");
    PRINT_TUPLE<IDX + 1, MAX, Args...>::print(os, t);
  }
};

template <int MAX, typename... Args>
struct PRINT_TUPLE<MAX, MAX, Args...> {
  static void print(std::ostream& os, const std::tuple<Args...>& t) {}
};

template <typename... Args>
std::ostream& operator<<(std::ostream& os, const std::tuple<Args...>& t) {
  os << "[";
  PRINT_TUPLE<0, sizeof...(Args), Args...>::print(os, t);
  return os << "]";
}

void Test() {
  std::cout << "--- variadic template test --\n";
  PrintX("hello", 12, "world", 3.55);

  int* pi = new int;
  MyPrintf("%d %s %p %f\n", 15, "this is ace", pi, 3.14159);

  std::cout << "Maximum(57,48,60,100,20,15)=" << Maximum(57, 48, 60, 100, 20, 15) << std::endl;

  std::cout << "std::make_tuple(7.5, std::string(\"hello\"), std::bitset<16>(377), 42):\n"
            << std::make_tuple(7.5, std::string("hello"), std::bitset<16>(377), 42) << std::endl;
}

}  // namespace variadic_templates_test

namespace nullptr_test {
void Fun(int) { std::cout << "call fun(int)\n"; }
void Fun(void*) { std::cout << "call fun(void*)\n"; }

void Test() {
  std::cout << "-- nullptr test -- \n";
  Fun(0);
  // Fun(NULL);// Call to 'Fun' is ambiguous
  Fun(32768);
  Fun(nullptr);
}

}  // namespace nullptr_test

namespace initializer_list_test {
class A {
 public:
  A(int a, int b) { std::cout << "call A(int a, int b) a=" << a << ", b=" << b << std::endl; }

  A(std::initializer_list<int> l) {
    std::cout << "call A(std::initializer<int>), value=";
    for (const auto& i : l) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
};

void Test() {
  std::cout << "-- initializer list test -- \n";
  A a1(77, 5);
  A a2{77, 5};
  A a3{75, 5, 42};
  A a4 = {77, 5};

  std::cout << "std::max({1,2,3,4,5})=" << std::max({1, 2, 3, 4, 5}) << std::endl;
  std::cout << "std::min({\"aa\",\"ab\",\"ac\"})=" << std::min({"aa", "ab", "ac"}) << std::endl;
}
}  // namespace initializer_list_test

namespace lambda_test {
// 编译器将 lambda 转换为仿函数
/*
[...](...) mutable throwSpec -> retType {...}
mutable
throwSpec
-> retType
这中间三个可有可无，如果有其中一个就要写（...）
*/

void Test() {
  std::cout << "this is lambda test\n";
  // 一个类型后面加 () 来调用
  [] { std::cout << "call lambda []{...}() \n"; }();

  // 最简形式
  auto fun = [] {
    // 静态变量依旧有效的
    static int count = 0;
    std::cout << "call lambda fun, count is:" << count++ << std::endl;
  };
  fun();
  fun();

  int id1 = 0;
  // 捕获的 id = 0，由于加了 mutable，所以 id 是可变的
  auto f1 = [id1]() mutable { std::cout << "call lambda f1, id1:" << id1++ << std::endl; };
  id1 = 42;
  f1();
  std::cout << "--call f1() id1 is:" << id1 << std::endl;
  f1();
  std::cout << "--call f2()2 id1 is:" << id1 << std::endl;
  f1();
  std::cout << "--call f2()3 id1 is:" << id1 << std::endl;

  int id2 = 0;
  // 捕获的是 id 的引用，id 会随外界变化而变化
  auto f2 = [&id2](int param) { std::cout << "call lambda f2, id2:" << id2++ << ", param:" << param++ << std::endl; };
  id2 = 42;
  f2(7);
  std::cout << "--call f2(7) id2 is:" << id2 << std::endl;
  f2(7);
  std::cout << "--call f2(7)2 id2 is:" << id2 << std::endl;
  f2(7);
  std::cout << "--call f2(7)3 id2 is:" << id2 << std::endl;

  int id3 = 0;
  // 捕获的 id = 0，由于加了 mutable，所以 id 是可变的
  auto f3 = [id3]() {
    // 下面会报错，id3 传值过来的没办法改变
    // std::cout << "call lambda f3, id3:" << id3++ << std::endl;
    std::cout << "call lambda f3, id3:" << id3 << std::endl;
  };
  id3 = 42;
  f3();
  std::cout << "--call f3() id3 is:" << id3 << std::endl;
  f3();
  std::cout << "--call f3()2 id3 is:" << id3 << std::endl;
  f3();
  std::cout << "--call f3()3 id3 is:" << id3 << std::endl;

  // decltype(f3) f4;// 会报错，lambda 没有默认构造函数
  auto f4 = f3;
  f4();
}

}  // namespace lambda_test

int main() {
  std::cout << "this is new feature test\n";

  variadic_templates_test::Test();
  // nullptr_test::Test();
  // initializer_list_test::Test();
  // lambda_test::Test();

  return 0;
}
