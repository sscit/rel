load("@rules_cc//cc:defs.bzl", "cc_library")
# c++ -O3 -Wall -shared -std=c++11 -fPIC example.cpp -o example.so -I./../external/pybind11/include -I/usr/include/python3.7m
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