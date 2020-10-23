load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "RelCUnitTest",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//RelC/src:relc_lib",
        "@googletest//:gtest_main",
    ],
)
