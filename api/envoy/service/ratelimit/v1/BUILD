load("@envoy_api//bazel:api_build_system.bzl", "api_proto_library_internal", "api_proto_package")

licenses(["notice"])  # Apache 2

api_proto_library_internal(
    name = "rls",
    srcs = ["rls.proto"],
    has_services = 1,
    deps = ["//envoy/service/ratelimit/v2:rls"],
)
