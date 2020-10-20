cc_binary(
    name = "relc",
    srcs = ["src/RelC.cpp"],
    deps = [
        ":relc_lib",
    ],
)


cc_library(
    name = "relc_lib",
    srcs = [
      "src/FileEngine.cpp",
      "src/Lexer.cpp",
      "src/Logger.cpp",
      "src/ParseException.cpp",
      "src/Parser.cpp",
      "src/RsParser.cpp",
      "src/RdParser.cpp",
      "src/SlidingWindow.cpp",
      "src/Token.cpp"
    ],
    hdrs = [
      "src/AST.h",
      "src/FileEngine.h",
      "src/Lexer.h",
      "src/Logger.h",
      "src/ParseException.h",
      "src/Parser.h",
      "src/RsParser.h",
      "src/RdParser.h",
      "src/SlidingWindow.h",
      "src/Token.h"
    ],
)
