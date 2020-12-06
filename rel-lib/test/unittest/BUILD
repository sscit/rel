load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "RelLibUnitTest",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//rel-lib/src:rel_lib",
        "@googletest//:gtest_main",
    ],
)
