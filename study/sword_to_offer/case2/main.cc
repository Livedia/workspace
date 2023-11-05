// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
/*
case2
设计一个类，我们只能生成该类的一个实例
单例模式
单例模式是指在内存中只会创建且仅创建一次对象的设计模式。

单例模式有两种类型：

1.懒汉式：在真正需要使用对象时才去创建该单例类对象
2.饿汉式：在类加载时已经创建好该单例对象，等待被程序使用


*/

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>

using std::cout, std::endl;

namespace case2 {

// 懒汉模式，用的时候才构造
class SingletonClass1 {
 private:
  int i_;
  std::string s_;
  inline static SingletonClass1* instance_p_ = nullptr;

 private:
  SingletonClass1() = default;
  SingletonClass1(int i, const std::string& s) : i_(i), s_(s) { cout << "---- call two params constructor" << endl; }

 public:
  SingletonClass1(const SingletonClass1&) = delete;
  SingletonClass1& operator=(const SingletonClass1&) = delete;

  static SingletonClass1* getInstance() {
    if (!instance_p_) {
      instance_p_ = new SingletonClass1(1, "default");
    }
    return instance_p_;
  }
};
// SingletonClass1 存在内存泄露问题（new 了对象后面没处理），有两种解决办法：
// 1.使用独占式智能指针 std::unique_ptr
// 2.使用静态的嵌套类对象

// 1.智能指针改进，利用 unique_ptr 对象在析构的时候会释放内存的特点解决内存泄露问题
class SingletonClass1_1 {
 private:
  int i_;
  std::string s_;
  inline static std::unique_ptr<SingletonClass1_1> instance_p_;

 private:
  SingletonClass1_1() = default;
  SingletonClass1_1(int i, const std::string& s) : i_(i), s_(s) { cout << "---- call two params constructor" << endl; }

 public:
  SingletonClass1_1(const SingletonClass1_1&) = delete;
  SingletonClass1_1& operator=(const SingletonClass1_1&) = delete;

  static SingletonClass1_1* getInstance() {
    if (instance_p_.get() == nullptr) {
      instance_p_.reset(new SingletonClass1_1(1, "default"));
    }
    return instance_p_.get();
  }
};

// 2.使用静态的嵌套类对象的代码示例，在程序运行结束时，系统会调用静态成员 delector
// 的析构函数，该析构函数会删除单例的唯一实例
class SingletonClass1_2 {
 private:
  int i_;
  std::string s_;
  inline static SingletonClass1_2* instance_p_ = nullptr;

 private:
  struct Delector {
    ~Delector() {
      if (SingletonClass1_2::instance_p_ != nullptr) {
        delete SingletonClass1_2::instance_p_;
      }
    }
  };
  inline static Delector delector_;

 private:
  SingletonClass1_2() = default;
  SingletonClass1_2(int i, const std::string& s) : i_(i), s_(s) { cout << "---- call two params constructor" << endl; }

 public:
  SingletonClass1_2(const SingletonClass1_2&) = delete;
  SingletonClass1_2& operator=(const SingletonClass1_2&) = delete;

  static SingletonClass1_2* getInstance() {
    if (instance_p_ == nullptr) {
      instance_p_ = new SingletonClass1_2(1, "default");
    }
    return instance_p_;
  }
};

// 上述的两种实现都是线程不安全的，问题出在第一次 new 对象的时候，可以采用双检测锁 (DCL) 来解决这个问题
class SingletonClass1_3 {
 private:
  int i_;
  std::string s_;
  inline static SingletonClass1_3* instance_p_ = nullptr;
  inline static std::mutex m_;  // 创建一个互斥锁

 private:
  struct Delector {
    ~Delector() {
      if (SingletonClass1_3::instance_p_ != nullptr) {
        delete SingletonClass1_3::instance_p_;
      }
    }
  };
  inline static Delector delector_;

 private:
  SingletonClass1_3() = default;
  SingletonClass1_3(int i, const std::string& s) : i_(i), s_(s) { cout << "---- call two params constructor" << endl; }

 public:
  SingletonClass1_3(const SingletonClass1_3&) = delete;
  SingletonClass1_3& operator=(const SingletonClass1_3&) = delete;

  static SingletonClass1_3* getInstance() {
    // 第一次检查
    if (instance_p_ == nullptr) {
      // 在函数内创建一个锁保护区
      std::lock_guard<std::mutex> lock(m_);
      // 第二次检查
      if (instance_p_ == nullptr) {
        instance_p_ = new SingletonClass1_3(1, "default");
      }
    }
    return instance_p_;
  }
};

// 双检测锁由于编译器的优化可能会导致多线程下出现取得未调用构造函数的对象的情况（new
// 过程被调整为拿内存返回再调用构造函数）C++ 11 引进了 memory model，提供了 atomic 实现内存的同步访问
class SingletonClass1_4 {
 private:
  int i_;
  std::string s_;
  inline static std::atomic<SingletonClass1_4*> instance_p_ = nullptr;
  inline static std::mutex m_;  // 创建一个互斥锁

 private:
  struct Delector {
    ~Delector() {
      if (SingletonClass1_4::instance_p_ != nullptr) {
        delete SingletonClass1_4::instance_p_;
      }
    }
  };
  inline static Delector delector_;

 private:
  SingletonClass1_4() = default;
  SingletonClass1_4(int i, const std::string& s) : i_(i), s_(s) { cout << "---- call two params constructor" << endl; }

 public:
  SingletonClass1_4(const SingletonClass1_4&) = delete;
  SingletonClass1_4& operator=(const SingletonClass1_4&) = delete;

  static SingletonClass1_4* getInstance() {
    // 第一次检查
    if (instance_p_ == nullptr) {
      // 在函数内创建一个锁保护区
      std::lock_guard<std::mutex> lock(m_);
      // 第二次检查
      if (instance_p_ == nullptr) {
        instance_p_ = new SingletonClass1_4(1, "default");
      }
    }
    return instance_p_;
  }
};

}  // namespace case2

int main() {
  std::cout << "--- this is template!\n";
  return 0;
}
