#include "server/config/http/extauth_config.h"

#include "common/http/filter/extauth.h"

namespace Envoy {
namespace Server {
namespace Configuration {

const std::string EXTAUTH_HTTP_FILTER_SCHEMA(R"EOF(
  {
    "$schema": "http://json-schema.org/schema#",
    "type" : "object",
    "properties" : {
      "cluster" : {"type" : "string"},
      "timeout_ms": {"type" : "integer"},
      "allowed_headers": {
        "type": "array",
        "items": {
            "type": "string"
        },
        "minItems": 1,
        "uniqueItems": true
      },
      "path_prefix": {"type" : "string"}
    },
    "required" : ["cluster", "timeout_ms"],
    "additionalProperties" : false
  }
  )EOF");

HttpFilterFactoryCb ExtAuthConfig::tryCreateFilterFactory(HttpFilterType type,
                                                          const std::string& name,
                                                          const Json::Object& json_config,
                                                          const std::string& stats_prefix,
                                                          Server::Instance& server) {
  if (type != HttpFilterType::Decoder || name != "extauth") {
    return nullptr;
  }

  json_config.validateSchema(EXTAUTH_HTTP_FILTER_SCHEMA);

  std::string prefix =
      json_config.hasObject("path_prefix") ? json_config.getString("path_prefix") : "";

  Http::ExtAuthConfigConstSharedPtr config(new Http::ExtAuthConfig{
      server.clusterManager(), Http::ExtAuth::generateStats(stats_prefix, server.stats()),
      json_config.getString("cluster"),
      std::chrono::milliseconds(json_config.getInteger("timeout_ms")),
      json_config.getStringArray("allowed_headers", true), prefix});
  return [config](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(Http::StreamDecoderFilterSharedPtr{new Http::ExtAuth(config)});
  };
}

/**
 * Static registration for the extauth filter. @see RegisterHttpFilterConfigFactory.
 */
static RegisterHttpFilterConfigFactory<ExtAuthConfig> register_;

} // namespace Configuration
} // namespace Server
} // namespace Envoy
