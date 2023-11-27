message("this iniclude cmake file")

include(test.cmake)

# 文件不存在的时候会直接报错
include(nofile.cmake)
