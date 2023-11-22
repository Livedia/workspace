// Copyright 2023 Vipshop Inc. All Rights Reserved.
// Author: Weida Lei (edward.lei@vipshop.com)
//
#include <string.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

using std::cout, std::endl;

namespace cstring_test {

/* 打印字符串中的内容 */
void PrintStr(const char* name, const char* str) {
  // ascii 码中 0-31 和 127 是控制字符或通讯专用的字符，无法显示在控制台中，所以还需强转 int 类型来打印
  cout << "-- " << name << " sizeof() is:" << sizeof(str) << ", strlen() is" << strlen(str) << ", value is:" << str
       << endl;
  for (int i = 0; i < strlen(str); ++i) {
    cout << name << "[" << i << "]=" << str[i] << " -> int(" << int(str[i]) << ") | ";
    if ((i + 1) % 5 == 0) {
      cout << endl;
    }
  }
  cout << endl;
}

void InitChar() {
  // 定义一个字符串
  char c = 's';
  // strlen(&c); // 行为是未定义的，因为 c 是一个字符而不是字符串
  PrintStr("c", &c);
  printf("c is:%c|  &c is:%p| %%s is:%s\n", c, &c, &c);
  printf("(&c) + 1 is:%p|\n", &c + 1);
  printf("* ((&c) + 1) is:%c| after int() is:%d|\n", *(&c + 1), int(*(&c + 1)));
}

void InitCharArray() {
  // 声明一个字符数组，由于没有初始化，其中元素的值是未知的，使用 strlen() 来计算大小的时候遇到`\0`就截止了
  // 调用 PrintStr 函数的时候，str 代表的数组退化为了指针，所以函数中再次使用 sizeof 来计算的时候大小就变成指针的大小了
  char str[10];
  PrintStr("str", str);
  // 声明并初始化一个的字符数组，sizeof 该数组大小为元素个数 +1（末尾添加的`\0`)
  char str2[] = "strings";
  cout << "str2 sizeof() is:" << sizeof(str2);
  PrintStr("str2", str2);
  // 通过指针修改字符内容
  *(str2 + 2) = 'p';
  PrintStr("changed_str2", str2);

  // 声明并初始化一个字符数组
  char str3[] = {'t', 'e', 's', 't', '\0', 'e', 'x', 't', 'r'};
  PrintStr("str3", str3);
  *(str3 + 2) = '\0';
  PrintStr("changed_str3", str3);
}

void InitCharPointer() {
  // 字符串指针
  // 字符串常量赋值给 char* 指针可能会存在误修改的风险 (*cp=xx 是不合法的)
  char* cp = "pointer";
  cout << "cp sizeof() is:" << sizeof(cp) << ", strlen() is:" << strlen(cp) << ", value is:" << cp
       << ", *cp sizeof() is:" << sizeof(*cp) /*<< ", strlen() is:" << strlen(*cp)*/ << ", value is:" << *cp
       << ", 常量字符串最后一位 +1 字符为：" << cp[strlen(cp)] << "该字符 int 值为：" << int(cp[strlen(cp)]) << endl;

  const char* cp2 = "const";
  // sizeof(cp2) 计算的是指针的大小，cp2 所指对象占用内存空间为：strlen(cp2) + 1;
  cout << "cp2 sizeof() is:" << sizeof(cp2) << ", strlen() is:" << strlen(cp2) << ", value is:" << cp2
       << ", *cp2 sizeof() is:" << sizeof(*cp2) /*<< ", strlen() is:" << strlen(*cp2)*/ << ", value is:" << *cp2
       << ", 常量字符串最后一位 +1 字符为：" << cp2[strlen(cp2)] << "该字符 int 值为：" << int(cp2[strlen(cp2)])
       << endl;
}

void Init() {
  InitChar();
  InitCharArray();
  InitCharPointer();
}

void MemsetTest() {
  // 使用 memset 来初始化
  char str_memset[20];
  memset(str_memset, '$', sizeof(str_memset));
  PrintStr("str_memset", str_memset);
}

void MemcpyTest(const char* str) {
  // 使用 memcpy 来拷贝字符串
  char str_memcpy[15];
  memcpy(str_memcpy, str, sizeof(str_memcpy));
  PrintStr("str_memcpy", str_memcpy);
}

void StrcpyTest(const char* str) {
  // strncpy(char* dest, const char* src)
  // 将源字符串复制到目标字符串中，包括空字符 ('\0')。
  // 使用 strcpy 来复制字符串，不论空间够不够，都会往后写，这是很危险的行为！
  char str_strcpy[10];
  char str_strcpy2[30];
  char s;
  char* str_strcpy3 = &s;

  strcpy(str_strcpy, str);
  PrintStr("str_strcpy", str_strcpy);
  strcpy(str_strcpy2, str);
  PrintStr("str_strcpy2", str_strcpy2);
  strcpy(str_strcpy3, str);
  PrintStr("str_strcpy3", str_strcpy3);
}

void StrcatTest() {
  // strcat(char* dest, const char* src)
  // 将源字符串连接到目标字符串的末尾，包括空字符 ('\0')
  // 使用 strcat 来拼接字符串
  char str_strcat1[20] = "hello";
  char str_strcat2[20] = "world";
  strcat(str_strcat1, str_strcat2);
  PrintStr("str_strcat1", str_strcat1);

  // 目标字符串空间不足的情况，会导致缓冲区溢出，产生未定义的行为！
  char str_strcat3[6] = "hello";
  strcat(str_strcat3, str_strcat2);
  PrintStr("str_strcat3", str_strcat3);
  char c;
  char* str_strcat4 = &c;
  strcat(str_strcat4, str_strcat2);
  PrintStr("str_strcat4", str_strcat4);
}

void StrcmpTest() {
  // strcmp(const char* left, const char* right)
  // 比较两个字符串的大小，返回一个整数值，表示两个字符串的大小关系。
  // 返回一个整数值表示结果：
  //  1   -->  left > right
  //  -1  -->  left < right
  //  0   -->  left = right
  const char* str1 = "abcd";
  const char* str2 = "b";
  const char* str3 = "abcde";
  const char str4[] = {'a', 'b', 'c', 'd'};
  const char* str5 = "abcd";

  auto compare = [](const char* left, const char* right) {
    int result = strcmp(left, right);
    if (result == 0) {
      cout << left << " = " << right << endl;

    } else if (result > 0) {
      cout << left << " > " << right << endl;
    } else {
      cout << left << " < " << right << endl;
    }
  };

  compare(str1, str2);
  compare(str1, str3);
  compare(str1, str4);
  compare(str1, str5);

  // strncmp(left,right,n)  比较前 n 个字符串大小
}

void StrstrTest() {
  // strstr(const char* str1, const char* str2)
  // 在字符串 str1 中查找字符串 str2 的第一次出现，并返回指向该位置的指针。
  const char* str1 = "abcdefghcdefgh";
  const char* str2 = "cdef";
  const char* str3 = "xyz";

  const char* result1 = strstr(str1, str2);
  // 没有匹配到返回空指针
  const char* result2 = strstr(str1, str3);
  printf("result1 is:%p|, the char is:%c|,the position is:%ld|\n", result1, *result1, result1 - str1);
  printf("result2 is:%p|\n", result2);
}

void StrchrTest() {
  // strchr(const char* str, int c)
  // 在字符串 str 中查找字符 c 的第一次出现，并返回指向该位置的指针。
  const char* str = "test_string";
  const char i = 'i';
  const char x = 'x';
  // 小写字母 n ascii 码就是 110
  int n = 110;

  const char* result1 = strchr(str, i);
  // 没有匹配到会返回空指针
  const char* result2 = strchr(str, x);
  const char* result3 = strchr(str, n);

  printf("result1 is:%p|, the char is:%c|,the position is:%ld|\n", result1, *result1, result1 - str);
  printf("result2 is:%p|\n", result2);
  printf("result3 is:%p|, the char is:%c|,the position is:%ld|\n", result3, *result3, result3 - str);
}

void StrtokTest() {
  // strtok(char* str, const char* delimiters)
  // 将字符串 str 按照分隔符 delimiters 进行分割，并返回分割后的子字符串。使用了静态缓冲区该函数是线程不安全的，少用！

  auto PrintfByN = [](const char* str, int n) {
    printf("-- PrintfByN:");
    for (int i = 0; i < n; ++i) {
      printf("(%c)", *(str + i));
    }
    printf("\n");
  };

  char str[] = "hello|world|how|are|you";
  char str2[] = "keep|keep|love|--yourself";
  const char* delimiter = "|";
  // 分割符号可以是个字符串而不单单是一个字符
  const char* delimiter2 = "--";
  PrintfByN(str, 24);
  PrintStr("str", str);

  char *token1, *token2, *token3, *token4;
  // 没找到分隔符，则返回第一个非空子串的地址，即源字符串的地址
  token1 = strtok(str, delimiter2);
  if (!token1) {
    printf("not fonud %s in %s\n", delimiter2, str);
  } else {
    printf("token1 is: %s\n", token1);
  }

  // 会破坏原始的字符串，将分割符号替换为了空字符串`\0`
  token1 = strtok(str, delimiter);
  PrintStr("str_arter", str);
  PrintfByN(str, 24);
  PrintStr("token1", token1);

  token2 = strtok(nullptr, delimiter);
  PrintStr("token2", token2);

  token3 = strtok(str2, delimiter);
  PrintStr("token3", token3);

  // 该函数会记录上一次调用后所在的位置，如果第一个参数是 nullptr 就从上次调用的位置开始分割
  token4 = strtok(nullptr, delimiter);
  PrintStr("token4", token4);
  token4 = strtok(nullptr, delimiter2);
  PrintStr("token4", token4);
  PrintfByN(str, 24);

  // 常见用法：
  char example[] = "Hello,World,How,Are,You";
  const char* d = ",";
  char* token = strtok(example, d);
  while (token != nullptr) {
    printf("%s\n", token);
    token = strtok(nullptr, d);
  }
}

void FunctionUsage() {
  // 函数基本都位于 <string.h> 头文件中
  // MemsetTest();
  // MemcpyTest("####################");
  // StrcpyTest("!!!!!!!!!!!!!!!!!!!!");
  // StrcatTest();
  // StrcmpTest();
  // StrstrTest();
  // StrchrTest();
  StrtokTest();
}

void Test() {
  // Init();
  FunctionUsage();
}
}  // namespace cstring_test

namespace cppstring_test {

void PrintStr(const std::string& name, const std::string& str) {
  cout << "-- " << name << " size is:" << str.size() << ", value is:" << str << endl << "chars is:";
  for (char i : str) {
    cout << "(" << i << ")";
  }
  cout << endl;
}

void Init() {
  // 常量字符串初始化
  std::string str = "const string";
  // std::string 对象大小为 24 字节，其中包含一个 size ,不同编译器实现版本不同，gcc 是
  // 字符串长度 + 字符串容量 + 字符串内容
  cout << "sizeof(str)=" << sizeof(str) << endl;
  PrintStr("str", str);
  // 使用 [] 依旧可以访问到未知的内存
  cout << "out range:" << str[10] << "|" << endl;
  // 可以使用 [] 来直接进行修改，str 不是指针了 str+3 这种无法使用
  str[5] = '_';
  PrintStr("changed_str", str);
}

void Test() { Init(); }
}  // namespace cppstring_test

int main() {
  // cstring_test::Test();
  cppstring_test::Test();
  return 0;
}
