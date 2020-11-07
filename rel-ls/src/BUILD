load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "main",
    srcs = [
        "rel_ls.cpp",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rel_ls_lib",
    srcs = [
      "MsgBuffer.cpp",
    ],
    hdrs = [
      "MsgBuffer.h",
    ],
    includes = [""],
    visibility = ["//visibility:public"],
)