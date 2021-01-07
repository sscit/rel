filegroup(
    name = "small_test_model",
    srcs = [
        "small/spec.rs",
        "small/data.rd",
        ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "invalid_test_model",
    srcs = [
        "invalid/spec.rs",
        "invalid/data.rd",
        ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "big_test_model",
    srcs = glob(["big/**/*.rd"]) + ["big/spec.rs"],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "huge_test_model",
    srcs = glob(["huge/**/*.rd"]) + ["huge/spec.rs"],
    visibility = ["//visibility:public"],
)