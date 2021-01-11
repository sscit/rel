load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "rel_cli",
    srcs = ["//rel-cli/src:rel_cli_main"],
    deps = [
            "//rel-cli/src:rel_cli_lib",
    ],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "rel_cli_script",
    srcs = ["rel_cli.sh"],
    visibility = ["//visibility:public"],
)
