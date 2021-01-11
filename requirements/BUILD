filegroup(
    name = "rel_requirements",
    srcs = glob(["*.rd"]) + ["spec.rs"],
    visibility = ["//visibility:public"],
)

sh_binary(
    name = "validate_rel_requirements",
    srcs = ["//rel-cli:rel_cli_script"],
    args = ["-r", "-v", "./requirements"],
    data = [":rel_requirements", "//rel-cli:rel_cli"],
    visibility = ["//visibility:public"],
)