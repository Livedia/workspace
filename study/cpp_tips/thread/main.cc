// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

namespace test_thread {

// 定义一个全局变量
int count = 0;

void Process0() {
  uint64_t sum{0};
  for (int i = 0; i < 10000; ++i) {
    sum += i;
  }
  // 可以使用 std::this_thread::get_id() 来获取当前线程的 id
  std::cout << __FUNCTION__ << "thread id is:" << std::this_thread::get_id() << ", sum is:" << sum;
}

void Process00(int i, std::string s, float f) {
  std::cout << __FUNCTION__ << "thread id is:" << std::this_thread::get_id() << ", i is:" << i << ", s is:" << s
            << ", f is:" << f;
}

void Test0() {
  // 创建新线程，直接传递函数名字即可
  std::thread thread_a(Process0);
  // 有参数的函数，将实参直接写在后面即可
  std::thread thread_b(Process00, 10, "test", 3.14);
  std::thread thread_c(Process00, 55, "fly", 1.024);

  // 当前线程不用管子线程，让他后台运行，结束后自动回收
  thread_a.detach();

  // 当前线程阻塞等待子线程运行完加入
  // joinable() 可以判断该线程是否能 join
  if (thread_b.joinable()) {
    thread_b.join();
  }
  thread_c.join();
}

void Process1() {
  for (int i = 0; i < 10000; ++i) {
    count++;
  }
  for (int i = 0; i < 10000; ++i) {
    count--;
  }
}

void Test1() {
  // 创建两个线程
  std::thread thread_a(Process1);
  std::thread thread_b(Process1);

  thread_a.join();
  thread_b.join();
  std::cout << "count is:" << count;
}

// 使用互斥量来改进
void Process2(std::mutex* m) {
  m->lock();
  for (int i = 0; i < 10000; ++i) {
    count++;
  }
  for (int i = 0; i < 10000; ++i) {
    count--;
  }
  m->unlock();
}
void Test2() {
  // 创建一个互斥量
  std::mutex m;
  // 创建两个线程
  std::thread thread_a(Process2, &m);
  std::thread thread_b(Process2, &m);

  thread_a.join();
  thread_b.join();
  std::cout << "count is:" << count;
}

// 使用 lock_guard 对象来自动管理互斥量
void Process3(std::mutex* m) {
  // lock_guard 对象在构造函数会调用 lock，析构的时候会调用 unlock
  std::lock_guard<std::mutex> lock(*m);
  for (int i = 0; i < 10000; ++i) {
    count++;
  }
  for (int i = 0; i < 10000; ++i) {
    count--;
  }
}

void Test3() {
  std::mutex m;

  std::thread thread_a(Process3, &m);
  std::thread thread_b(Process3, &m);

  thread_a.join();
  thread_b.join();
  std::cout << "count is:" << count;
}

// 使用更加灵活的 unique_lock 对象来代替 lock_guard 对象
// lock_guard 不能在使用过程中进行 unlock 操作，而 unique_lock 可以
void Process4(std::mutex* m) {
  // unique_lock 在构造对象时候，可以传递第二个参数为 std::defer_lock，这样创建出来的对象一开始就不会调用 lock 函数
  std::unique_lock<std::mutex> lock(*m, std::defer_lock);
  // 上锁，当有多个 unique_lock 对象的时候，可以采用 std::lock(lock1,lock2) 的方式来进行批量的上锁操作
  lock.lock();

  for (int i = 0; i < 10000; ++i) {
    count++;
  }
  for (int i = 0; i < 10000; ++i) {
    count--;
  }

  // 离开作用域的时候 lock 对象析构会自动调用 unlock 操作，或者也可以主动调用 unlock 操作如：
  // lock.unlock();
}

void Test4() {
  std::mutex m;

  std::thread thread_a(Process4, &m);
  std::thread thread_b(Process4, &m);

  thread_a.join();
  thread_b.join();
  std::cout << "count is:" << count;
}

// 条件变量 condition_variable wait() 使用：
// 条件变量 condition_variable 必须结合 unique_lock 来使用
bool NotZero() { return count != 0; }

void Process5(std::mutex* m, std::condition_variable* cv, int n) {
  for (int i = 0; i < n; ++i) {
    // 自动上锁
    std::unique_lock<std::mutex> lock(*m);
    // wait() 的第二个参数为 false 的时候会导致当前线程被阻塞
    // 阻塞直到 cv 收到 notify_one() 的调用，会终止阻塞并调用 lock.unlock()
    cv->wait(lock, NotZero);
    std::cout << __FUNCTION__ << ":" << count << std::endl;
    // 此时 count 不为 0 主线程 while 阻塞中，所以不用加锁
    count = 0;
  }
}

void Test5() {
  std::mutex m;
  std::condition_variable cv;
  std::thread thread_a(Process5, &m, &cv, 10);

  for (int i = 0; i < 10; ++i) {
    // count == 0 的时候才执行
    while (NotZero()) {
      // 阻塞当前线程主动让出 CPU 执行权
      std::cout << "yield\n";
      std::this_thread::yield();
    }
    std::unique_lock<std::mutex> lock(m);
    count = i + 1;
    // 发出通知信号
    cv.notify_one();
  }

  thread_a.join();
}

// 条件变量 condition_variable wait_for() 使用：
void Process6(std::condition_variable* cv) {
  std::cin >> count;
  cv->notify_one();
}

void Test6() {
  std::condition_variable cv;
  std::cout << "Please, enter an integer (I'll be printing dots): \n";
  std::thread thread_a(Process6, &cv);
  std::mutex m;
  std::unique_lock<std::mutex> lock(m);
  // 主线程 cv 等待超时每 2 秒钟输出一个 .
  // 等收到 notify_one() 的通知跳出 while 循环
  while (cv.wait_for(lock, std::chrono::seconds(2)) == std::cv_status::timeout) {
    std::cout << "." << std::endl;
  }
  std::cout << "You entered: " << count << std::endl;

  thread_a.join();
}

}  // namespace test_thread

int main() {
  std::cout << "this is main\n";
  // test_thread::Test0();
  // test_thread::Test1();
  // test_thread::Test2();
  // test_thread::Test3();
  // test_thread::Test4();
  // test_thread::Test5();
  test_thread::Test6();
}
