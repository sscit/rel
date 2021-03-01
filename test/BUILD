filegroup(
    name = "small_test_model",
    srcs = [
        "small/spec.rs",
        "small/data.rd",
        ],
    visibility = ["//visibility:public"],
)
sh_binary(
    name = "validate_small_testmodel",
    srcs = ["//rel-cli:rel_cli_script"],
    args = ["-r", "-v", "./test/small"],
    data = [":small_test_model", "//rel-cli:rel_cli"],
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
    srcs = glob(["big/**/*.rd"]) + ["big/spec.rs"] + ["big/spec_camera.rs"],
    visibility = ["//visibility:public"],
)
sh_binary(
    name = "validate_big_testmodel",
    srcs = ["//rel-cli:rel_cli_script"],
    args = ["-r", "-v", "./test/big"],
    data = [":big_test_model", "//rel-cli:rel_cli"],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "huge_test_model",
    srcs = glob(["huge/**/*.rd"]) + ["huge/spec.rs"],
    visibility = ["//visibility:public"],
)
sh_binary(
    name = "validate_huge_testmodel",
    srcs = ["//rel-cli:rel_cli_script"],
    args = ["-r", "-v", "./test/huge"],
    data = [":huge_test_model", "//rel-cli:rel_cli"],
    visibility = ["//visibility:public"],
)
