#include <iostream>
#include <vector>
using namespace std;

struct A {
  int i;
  std::string s;
};

void TestVector() {
  std::vector<A> va;
  A a = {1, "tt"};
  va.push_back(a);
  va.emplace_back(a);
  va.emplace_back(a);

  for (const auto& it : va) {
    cout << it.i << it.s << endl;
  }
}

int main() {
  cout << "this is start file" << endl;
  int i = 10;
  i++;
  printf("%d", i);
  cout << i;

  std::vector<int> iv{1, 2, 3, 44, 5, 6};
  cout << "size is:" << iv.size() << endl;
  cout << "max size is:" << iv.max_size() << endl;

  TestVector();
  return 0;
}
