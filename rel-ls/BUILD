load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
    name = "rel_ls",
    srcs = ["//rel-ls/src:main"],
    deps = [
            "//RelC/src:relc_lib",
            "//rel-ls/src:rel_ls_lib",
            "@json//:json_lib",
    ],
)



