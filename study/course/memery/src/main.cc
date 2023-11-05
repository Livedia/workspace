// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include <cstddef>
#include <iostream>
#include <new>
#include <string>

namespace test_new {
class A {
 private:
  int i_;
  std::string str_;

 public:
  // A() = default;
  A() : i_(100), str_("default") { std::cout << "this is:" << this << ", call no param constructor\n"; }

  A(const int i, const std::string& s) : i_(i), str_(s) {
    std::cout << "this is:" << this << ", call two params constrcutor\n";
  }

  ~A() { std::cout << "this is:" << this << "call destructor function\n"; }

 public:
  static void* MyAlloc(size_t size) { return malloc(size); }
  static void MyFree(void* ptr) { return free(ptr); }
  // 重载的都是静态函数，这个地方默认就是静态的前面可以不用加 static
  void* operator new(size_t size) {
    std::cout << "use override operator new\n";
    return MyAlloc(size);
  }
  static void operator delete(void* ptr) {
    std::cout << "use override operator delete\n";
    MyFree(ptr);
  }
  static void* operator new[](size_t size) {
    std::cout << "use override operator new[]\n";
    A* p = (A*)malloc(size);
    return p;
  }
  static void operator delete[](void* ptr) {
    std::cout << "use override operator delete[]\n";
    free(ptr);
  }

  // 可以重载 new(...)  repalce new，但是参数列表中第一个参数类型必须为 size_t
  // 重载的 delete(...) 不会被调用，除非调用 delete 的时候发生了异常
};

class Screen {
 public:
  explicit Screen(int x) : i(x){};
  int get() { return i; }

  void* operator new(size_t);
  void operator delete(void*, size_t);  //(2)
  //! void  operator delete(void*);           //(1) 二擇一。若 (1)(2) 並存，會有很奇怪的報錯 (摸不著頭緒)

 private:
  Screen* next;
  static Screen* freeStore;
  static const int screenChunk;

 private:
  int i;
};
Screen* Screen::freeStore = 0;
const int Screen::screenChunk = 24;

void* Screen::operator new(size_t size) {
  Screen* p;
  if (!freeStore) {
    // linked list 是空的，所以攫取一大塊 memory
    // 以下呼叫的是 global operator new
    size_t chunk = screenChunk * size;
    freeStore = p = reinterpret_cast<Screen*>(new char[chunk]);
    // 將分配得來的一大塊 memory 當做 linked list 般小塊小塊串接起來
    for (; p != &freeStore[screenChunk - 1]; ++p) p->next = p + 1;
    p->next = 0;
  }
  p = freeStore;
  freeStore = freeStore->next;
  return p;
}
//! void Screen::operator delete(void *p)       //(1)
void Screen::operator delete(void* p, size_t)  //(2) 二擇一
{
  // 將 deleted object 收回插入 free list 前端
  (static_cast<Screen*>(p))->next = freeStore;
  freeStore = static_cast<Screen*>(p);
}

void TestScreen() {
  std::cout << "\ntest_per_class_allocator_1().......... \n";

  std::cout << sizeof(Screen) << std::endl;  // 8

  size_t const N = 100;
  Screen* p[N];

  for (int i = 0; i < N; ++i) p[i] = new Screen(i);

  // 輸出前 10 個 pointers, 用以比較其間隔
  for (int i = 0; i < 10; ++i) {
    std::cout << p[i] << std::endl;
  }

  for (auto& i : p) {
    delete i;
  }
}

class Airplane {
 private:
  struct AirplaneRep {
    uint32_t miles;
    char type;
  };

 private:
  union {
    AirplaneRep rep;  // 此栏针对使用中的 object
    Airplane* next;   // 此栏针对 free list 上的 object 嵌入式指针
  };

 public:
  uint32_t getMiles() { return rep.miles; }
  char getType() { return rep.type; }
  void set(uint32_t m, char t) {
    rep.miles = m;
    rep.type = t;
  }

 public:
  static void* operator new(size_t size);
  static void operator delete(void* dead_object, size_t size);

 private:
  inline static const int BLOCK_SIZE = 512;
  inline static Airplane* head_of_free_list;
};

void* Airplane::operator new(size_t size) {
  // 如果大小有误，转交给 ::operator new()，在发生继承的时候可能会发生有误的情况
  if (size != sizeof(Airplane)) {
    return ::operator new(size);
  }

  Airplane* p = head_of_free_list;
  // 如果 p 有效，就把 list 头部下移一个元素
  if (p) {
    head_of_free_list = p->next;
  } else {
    // free list 已空，申请（分配）一大块内存
    Airplane* new_block = static_cast<Airplane*>(::operator new(BLOCK_SIZE * sizeof(Airplane)));
    // 将小块串成一个 free list
    //  但跳过 #0，因为它将被传回作为本次成果
    for (int i = 1; i < BLOCK_SIZE - 1; ++i) {
      new_block[i].next = &new_block[i + 1];
    }
    // 结束 list
    new_block[BLOCK_SIZE - 1].next = 0;
    p = new_block;
    head_of_free_list = &new_block[1];
  }
  return p;
}

void Airplane::operator delete(void* dead_object, size_t size) {
  if (dead_object == nullptr) return;
  if (size != sizeof(Airplane)) {
    ::operator delete(dead_object);
    return;
  }

  auto* carcass = static_cast<Airplane*>(dead_object);
  carcass->next = head_of_free_list;
  head_of_free_list = carcass;
}

void TestAirplane() {
  std::cout << "sizeof Airplane is:" << sizeof(Airplane) << std::endl;
  size_t const N = 100;
  Airplane* p[N];

  for (auto& i : p) {
    i = new Airplane;
  }

  p[1]->set(1000, 'A');
  p[5]->set(2000, 'B');
  p[9]->set(500000, 'C');

  // 输出前 10 个 pointers，用来比较间隔
  for (int i = 0; i < 10; ++i) {
    std::cout << "p[" << i << "]=" << p[i] << std::endl;
  }

  for (int i = 0; i < N; ++i) {
    delete p[i];
  }
}

void Test() {
  A* p = new A();
  delete p;

  A* array_p = new A[3];
  delete[] array_p;
}
}  // namespace test_new

int main() {
  std::cout << "this is memery test\n";

  // test_new::Test();
  test_new::TestScreen();
  test_new::TestAirplane();
  return 0;
}
