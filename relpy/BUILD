load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "rel_py",
    srcs = [
      "//relpy/src:rel_py_main",
      "//rel-lib/src:rel_lib_src",
    ],
    hdrs = [
      "//rel-lib/src:rel_lib_hdr",
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
