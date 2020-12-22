load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

new_git_repository(
    name = "googletest",
    build_file = "gmock.BUILD",
    remote = "https://github.com/google/googletest",
    tag = "release-1.10.0",
)

new_git_repository(
    name = "json",
    build_file = "json.BUILD",
    remote = "https://github.com/nlohmann/json",
    tag = "v3.9.1",
)

new_git_repository(
    name = "pybind11",
    build_file = "pybind11.BUILD",
    remote = "https://github.com/pybind/pybind11",
    tag = "v2.6.1",
)

new_local_repository(
    name = "python_linux",
    path = "/usr/include/python3.8",
    build_file = "external/python3.BUILD",
)
