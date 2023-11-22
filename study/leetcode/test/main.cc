// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
// 这个文件用来本地验证一些 C++ 语法

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "carbon/numeric/strtox.h"

using std::string;

namespace test1 {

void TestStrtox() {
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
}

void StringSize() {
  // 字符串对象中包含：字符串内容指针、字符串内容大小、字符串内容最大容量（在不重新分片内存的情况下容纳的字符数）
  // 分别 8 字节，总共 24 字节，字符串内容：1 个字符占用 1 字节
  std::cout << __func__ << std::endl;
  std::string s;
  std::cout << "empty std::string object size is:" << sizeof(s) << ", size is:" << s.size() << std::endl;
  s = "test";
  std::cout << "not empty std::string object size is:" << sizeof(s) << ", size is:" << s.size() << std::endl;
}

}  // namespace test1

namespace test20231031 {

void TestUniquePtr() {
  std::unique_ptr<std::string> up;
  if (up.get() == nullptr) {
    std::cout << "up is nullptr" << std::endl;
  } else {
    std::cout << "up is not nullptr" << std::endl;
  }
}

}  // namespace test20231031

namespace test20231101 {

void TestVecotrAt() {
  std::vector<int> v{0, 1, 2, 3, 4, 5};
  for (size_t i = 0; i < 10; ++i) {
    // 不会报错，会导致未定义的行为
    v[i] = 3 * i;
  }
  v[100] = 2232;
  std::cout << "v[100]:" << v[100] << std::endl;
  std::cout << "v is:";

  for (size_t i = 0; i < 20; ++i) {
    std::cout << "v[" << i << "]:" << v[i] << " ";
  }
  // at 会抛出异常
  std::cout << std::endl << "v.at(10):" << v.at(10);
}

void TestConstChar() {
  char str[] = "hello";
  // char* str2 = "hello"; 在 c++ 11 之后就被不允许隐式转换了 char const* str2 = "hello" 这样可以
  char* str2 = (char*)"hello";
  std::cout << "str1:" << str << ", str2:" << str2 << std::endl;
  str[0] = 't';
  str2[0] = 'x';
  std::cout << "-- str1:" << str << ", str2:" << str2 << std::endl;
  const char arr[] = "world";
  const char* arr2 = "world";
  std::cout << "arr:" << arr << ", arr2:" << arr2 << std::endl;
  // arr[0] = 't';  // 报错
  // arr2[0] = 'x'; // 报错
  // std::cout << "-- arr:" << arr << ", arr2:" << arr2 << std::endl;
}

void TestBigLittle() {
  union U {
    int a;
    char b;
  };
  U u;
  u.a = 0x01020304;

  if (u.b == 0x04) {
    std::cout << u.b << std::endl;  // 输出char字符
    std::cout << "is little" << std::endl;
  } else if (u.b == 0x01) {
    std::cout << u.b << std::endl;  // 输出char字符
    std::cout << "is big" << std::endl;
  }
}
class Student {
 private:
  int id_;
  std::string name_;

 public:
  Student(int i, std::string s) : id_(i), name_(std::move(s)) {}
  void Show() { std::cout << "name is:" << name_ << ", id is:" << id_ << std::endl; }
  ~Student() { std::cout << "~Student\n"; }
};

void TestPlacementNew() {
  auto* p = new Student(101, "lihua");
  p->Show();
  auto* p2 = new (p) Student(202, "zhangwei");
  p2->Show();
  delete p2;
}

void TestAllocator() {
  std::allocator<Student> alloc;
  using traits_t = std::allocator_traits<decltype(alloc)>;

  auto p = alloc.allocate(1);                  // 分配一个 Student 对象的内存
  traits_t::construct(alloc, p, 101, "mike");  // 调用Person的构造函数，如果构造函数有参数，参数写在p之后
  // p 现在是一个指向Person的指针，且其指向对象被初始化过
  // 对p进行一些操作
  p->Show();
  // 销毁对象，但不释放内存，等同于调用p->~Person()
  traits_t::destroy(alloc, p);
  // 释放内存
  alloc.deallocate(p, 1);
}

void TestRhombusInheritance() {
  class Animal {
   public:
    int m_Age;
  };

  class Sheep : public Animal {
    int m_sheep;
  };
  class Camel : public Animal {
    int m_camel;
  };

  class Son : public Sheep, public Camel {
    int m_son;
  };

  std::cout << "sizeof(Animal):" << sizeof(Animal) << std::endl;  // 4：m_Age
  std::cout << "sizeof(Sheep):" << sizeof(Sheep) << std::endl;    // 8：m_sheep,m_Age
  std::cout << "sizeof(Camel):" << sizeof(Camel) << std::endl;    // 8：m_camel,m_Age
  std::cout << "sizeof(Son):" << sizeof(Son) << std::endl;  // 20：m_son,m_camel,Camel::m_Age,m_sheep,Sheep::m_Age

  class Animal2 {
   public:
    double m_Age;
    virtual ~Animal2() = default;
  };

  class Sheep2 : public Animal2 {
    double m_sheep;
  };
  class Camel2 : public Animal2 {
    double m_camel;
  };

  class Son2 : public Sheep2, public Camel2 {
    double m_son;
  };

  // 16：Animal2-Vbptr,m_Age
  std::cout << "sizeof(Animal):" << sizeof(Animal2) << std::endl;
  // 24：Sheep2-Vbptr,m_sheep,m_Age
  std::cout << "sizeof(Sheep):" << sizeof(Sheep2) << std::endl;
  // 24：Camel2-Vbptr,m_camel,m_Age
  std::cout << "sizeof(Camel):" << sizeof(Camel2) << std::endl;
  // 56：m_son,Camel2-Vbptr,m_camel,Camel2::m_Age,Sheep2-Vbptr,m_sheep,Sheep2::m_Age 没有自己的虚表
  std::cout << "sizeof(Son):" << sizeof(Son2) << std::endl;

  class Animal3 {
   public:
    double m_Age;
  };

  class Sheep3 : virtual public Animal3 {
    double m_sheep;
  };
  class Camel3 : virtual public Animal3 {
    double m_camel;
  };

  class Son3 : public Sheep3, public Camel3 {
    double m_son;
  };

  // 8：m_Age
  std::cout << "sizeof(Animal):" << sizeof(Animal3) << std::endl;
  // 24：Sheep3-Vbptr,m_sheep,m_Age
  std::cout << "sizeof(Sheep):" << sizeof(Sheep3) << std::endl;
  // 24：Camel3-Vbptr,m_camel,m_Age
  std::cout << "sizeof(Camel):" << sizeof(Camel3) << std::endl;
  // 48：Camel2-Vbptr,m_camel,Sheep2-Vbptr,m_sheep,m_son,m_Age 没有自己的虚表
  std::cout << "sizeof(Son):" << sizeof(Son3) << std::endl;
}

}  // namespace test20231101

int main() {
  std::cout << "this is test\n";

  // test1::StringSize();
  // test20231031::TestUniquePtr();
  // test20231101::TestVecotrAt();
  // test20231101::TestConstChar();
  // test20231101::TestBigLittle();
  // test20231101::TestPlacementNew();
  // test20231101::TestAllocator();
  test20231101::TestRhombusInheritance();

  return 0;
}
