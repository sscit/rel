load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "rel_cli_main",
    srcs = [
        "rel_cli.cpp",
    ],
    visibility = ["//rel-cli:__pkg__"],
)

cc_library(
    name = "rel_cli_lib",
    srcs = [
      "Cli.cpp"
    ],
    hdrs = [
      "Cli.h"
    ],
    deps = [
       "//rel-lib:rel_lib",
    ],
    visibility = ["//rel-cli:__subpackages__"],
)
