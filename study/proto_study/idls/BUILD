# Author: edward.lei
#
load("@rules_cc//cc:defs.bzl", "cc_proto_library")
load("@rules_proto//proto:defs.bzl", "proto_library")


licenses(["notice"])

proto_library(
    name = "start_proto",
    srcs = [
        "start.proto"
    ],
    visibility = [
        "//visibility:public",
    ]
)

cc_proto_library(
    name = "start_proto_cc",
    visibility = [
        "//visibility:public",
    ],
    deps = [
        ":start_proto",
    ],
)