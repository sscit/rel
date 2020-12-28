load("@rules_cc//cc:defs.bzl", "cc_test")

cc_test(
    name = "RelCliUnitTest",
    srcs = glob(["**/*.cpp"]),
    deps = [
        "//rel-cli/src:rel_cli_lib",
        "@googletest//:gtest_main",
    ],
    data = [
        "//rel-cli/test/unittest/testdata_valid:valid_test_model",
        "//rel-cli/test/unittest/testdata_invalid:invalid_test_model"],
)
