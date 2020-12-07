load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "pybind11_lib",
    hdrs = glob([
        "include/pybind11/**/*.h",
    ]),
    includes = [
        "include",
    ],
    deps = [
        "@python_linux//:includes",
    ],
    visibility = ["//visibility:public"],
)
