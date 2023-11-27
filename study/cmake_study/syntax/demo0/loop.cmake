message("--1--")
set(MY_LIST 1 2 3)

foreach(VAR IN LISTS MY_LIST ITEMS e f)
  message(${VAR})
endforeach()

message("--2--")

foreach(VAR 1 2 3 e f)
  message(${VAR})
endforeach()

message("--3--")

# 声明两个具有相同数量元素的list
set(L1 "one;two;three;four")
set(L2 "1;2;3;4;5")

# 通过 ZIP_LISTS 关键子来“压缩“上面的两个列表，以在单次循环中处理相同索引的列表元素
# 示例1：通过num_<N> 变量存储获取元素
foreach(num IN ZIP_LISTS L1 L2)
  message("num_0=${num_0}, num_1=${num_1}")
endforeach()

message("--4--")

# 示例2：通过自定义传递两个<loop_var>变量，存储获取元素
foreach(word num IN ZIP_LISTS L1 L2)
  message("word=${word}, num=${num}")
endforeach()
