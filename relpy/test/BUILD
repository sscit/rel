load("@rules_cc//cc:defs.bzl", "cc_library")

py_test(
    name = "integration_test",
    srcs = ["integration_test.py"],
    deps = [":rel_py_tmp"],
    data = ["//test:small_test_model"],
    visibility = ["//visibility:public"],
)

py_test(
    name = "integration_test_small_big",
    srcs = ["integration_test_small_big.py"],
    deps = [":rel_py_tmp"],
    data = ["//test:big_test_model", "//test:small_test_model"],
    visibility = ["//visibility:public"],
)

py_test(
    name = "integration_test_big",
    srcs = ["integration_test_big.py"],
    deps = [":rel_py_tmp"],
    data = ["//test:big_test_model"],
    visibility = ["//visibility:public"],
)

py_test(
    name = "integration_test_huge",
    srcs = ["integration_test_huge.py"],
    deps = [":rel_py_tmp"],
    data = ["//test:huge_test_model"],
    timeout = "long",
    visibility = ["//visibility:public"],
)

py_library(
    name = "rel_py_tmp",
    data = [":rel_py"],
    imports = ["."],
    visibility = ["//visibility:private"],
)

# duplication of rule from relpy/BUILD necessary
# but as cc_binary, as this seems to be a bazel limitation
# https://github.com/bazelbuild/bazel/issues/1475
# https://github.com/bazelbuild/bazel/issues/701
cc_binary(
    name = "rel_py",
    srcs = [
      "//relpy/src:rel_py_main",
      "//rel-lib/src:rel_lib_src",
      "//rel-lib/src:rel_lib_hdr",
    ],
    deps = [
        "@pybind11//:pybind11_lib",
    ],
    linkshared = 1,
    copts = [
        "-fPIC",
    ],
    visibility = ["//visibility:private"],
)