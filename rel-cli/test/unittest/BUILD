load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "RelCliUnitTest",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//rel-cli/src:rel_cli_lib",
        "@googletest//:gtest_main",
    ],
    data = [
        "//test:small_test_model",
        "//test:invalid_test_model"],
)
