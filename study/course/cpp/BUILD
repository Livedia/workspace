cc_binary(
    name = "start",
    srcs = [
        "src/start.cc",
    ],
    copts = [
        "-std=c++17",
    ],
)

cc_library(
    name = "complex",
    hdrs = [
        "include/complex.h",
    ],
)

cc_binary(
    name = "complex_test",
    srcs = [
        "src/complex_test.cc",
    ],
    copts = [
        "-std=c++17",
    ],
    deps = [
        ":complex",
    ],
)
