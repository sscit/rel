load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "main",
    srcs = [
        "rel_ls.cpp",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "rel_ls_lib",
    srcs = [
      "MsgBuffer.cpp",
      "LspEngine.cpp",
      "Workspace.cpp",
      "Uri.cpp",
    ],
    hdrs = [
      "MsgBuffer.h",
      "LspEngine.h",
      "Workspace.h",
      "Uri.h",
    ],
    deps = [
            "//rel-lib:rel_lib",
            "@json//:json_lib",
    ],
    includes = [""],
    visibility = ["//visibility:public"],
)