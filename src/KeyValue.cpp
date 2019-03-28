#include <nlohmann/json.hpp>
#include "VaultClient.h"

KeyValue::KeyValue(VaultClient &client) :
  version_(KeyValue::Version::v2), client_(client) { }

KeyValue::KeyValue(VaultClient &client, KeyValue::Version version) :
  version_(version), client_(client) {}

std::string KeyValue::getUrl(std::string path) {
  return client_.getUrl("/v1/secret/", path);
}

std::string KeyValue::get(std::string path) {
  return client_.getHttpClient().get(getUrl(path), client_.getToken());
}

std::string KeyValue::put(std::string path,
			  std::unordered_map<std::string, std::string> map) {
  nlohmann::json j;
  j["data"] = nlohmann::json::object();
  std::for_each(map.begin(), map.end(),
		[&](std::pair<std::string, std::string> pair) {
    j["data"][pair.first] = pair.second;
  });

  return client_.getHttpClient().post(getUrl(path), client_.getToken(), j.dump());
}

std::string KeyValue::del(std::string path) {
  return client_.getHttpClient().del(getUrl(path), client_.getToken());
}
