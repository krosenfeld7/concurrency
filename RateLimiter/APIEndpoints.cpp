
#include "APIEndpoints.hpp"

std::unordered_map<EndpointName, Endpoint> endpoints = {
    { "POST", API::post },
    { "GET", API::get },
    { "PUT", API::put },
    { "DELETE", API::del }
};

auto APIEndpoints::getEndpoint(EndpointName endpointName) -> Endpoint {
    auto endpoint = endpoints.find(endpointName);

    if (endpoint != endpoints.end()) {
        return endpoint->second;
    }
    return API::error;
}