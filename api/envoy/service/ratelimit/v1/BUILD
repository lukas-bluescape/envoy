load("@envoy_api//bazel:api_build_system.bzl", "api_go_grpc_library", "api_proto_library_internal")

licenses(["notice"])  # Apache 2

api_proto_library_internal(
    name = "rls",
    srcs = ["rls.proto"],
    has_services = 1,
    deps = ["//envoy/service/ratelimit/v2:rls"],
)
