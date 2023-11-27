# 定义了一个名为MyMacro的宏，参数为myVar
macro(MyMacro myVar)
  set(myVar "new value")
  message("argument: ${myVar}")
endmacro()

set(myVar "first value")
message("myVar is now: ${myVar}")

# 调用宏
MyMacro("called value")
message("myVar is now: ${myVar}")
