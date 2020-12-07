load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rel_py",
    srcs = [
      "src/example.cpp",
    ],
    deps = [
        "@pybind11//:pybind11_lib",
    ],
    linkstatic=False,
    copts = [
        "-fPIC",
    ],
    visibility = ["//visibility:public"],
)