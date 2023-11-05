// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

/*
case1 赋值运算符函数
为 CMyString 类型添加赋值运算符函数

*/

#include <cstring>
#include <iostream>

using std::cout, std::endl;

namespace case1 {

class CMyString {
 public:
  CMyString(char* p_data = nullptr);
  CMyString(const CMyString& str);
  ~CMyString(void);

  // 返回值需要设为该类型的引用，这样才能连续赋值 CMyString a = b = c = d;
  CMyString& operator=(const CMyString& str);

  void Print() {
    std::cout << "str is:";
    for (int i = 0; *(p_data_ + i) != '\0'; ++i) {
      std::cout << *(p_data_ + i);
    }
    std::cout << std::endl;
  }

 private:
  char* p_data_;
};

CMyString::CMyString(char* p_data) {
  if (p_data) {
    // 新申请空间
    cout << "str size is:" << strlen(p_data_) << endl;
    p_data_ = new char[strlen(p_data_) + 1];
    strcpy(p_data_, p_data);
  } else {
    p_data_ = new char[1];
    *p_data_ = '\0';
  }
}

CMyString::CMyString(const CMyString& str) {
  p_data_ = new char[strlen(str.p_data_) + 1];
  strcpy(p_data_, str.p_data_);
}
CMyString::~CMyString(void) { delete[] p_data_; }

#if 0
CMyString& CMyString::operator=(const CMyString& str) {
  // 考虑是自己的情况
  if (this == &str) {
    return *this;
  }

  if (str.p_data_) {
    if (p_data_) {
      // 清空已有数据
      delete[] p_data_;
      p_data_ = nullptr;
    }
    // 新申请空间深拷贝
    p_data_ = new char[strlen(str.p_data_) + 1];
    strcpy(p_data_, str.p_data_);
  }
  return *this;
}
#endif

// 考虑 new 时候异常安全性的情况，创建一个临时对象，交换临时对象所指的数据
CMyString& CMyString::operator=(const CMyString& str) {
  // 考虑是自己的情况
  if (this != &str) {
    CMyString str_t(str);

    // 交换指针所指内容
    char* p_t = str_t.p_data_;
    str_t.p_data_ = p_data_;
    p_data_ = p_t;
  }
  return *this;
}

void Test() {
  char s1[] = "test22";
  CMyString a(s1);
  a.Print();

  char s2[] = "study";
  CMyString b(s2);
  b.Print();

  b = a;
  b.Print();
}
}  // namespace case1

int main() {
  std::cout << "--- this is case1!\n";

  case1::Test();

  return 0;
}
