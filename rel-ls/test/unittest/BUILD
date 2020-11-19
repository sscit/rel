load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "RelLsUnitTest",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//rel-ls/src:rel_ls_lib",
        "@googletest//:gtest_main",
    ],
)
