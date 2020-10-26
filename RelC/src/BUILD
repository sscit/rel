load("@rules_cc//cc:defs.bzl", "cc_library")

filegroup(
    name = "main",
    srcs = [
        "RelC.cpp",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "relc_lib",
    srcs = [
      "FileEngine.cpp",
      "Lexer.cpp",
      "Logger.cpp",
      "ParseException.cpp",
      "Parser.cpp",
      "RsParser.cpp",
      "RdParser.cpp",
      "SlidingWindow.cpp",
      "Token.cpp",
      "FileReader.cpp"
    ],
    hdrs = [
      "AST.h",
      "FileEngine.h",
      "Lexer.h",
      "Logger.h",
      "ParseException.h",
      "Parser.h",
      "RsParser.h",
      "RdParser.h",
      "SlidingWindow.h",
      "Token.h",
      "FileReader.h"
    ],
    visibility = ["//visibility:public"],
)
