#pragma once

/*
Provides endpoint mapping for easy access to API functions.
*/

#include <unordered_map>

#include "API.hpp"

extern std::unordered_map<EndpointName, Endpoint> endpoints;

class APIEndpoints {
public:
    /*
     * Provides the API function for the requested endpoint.
     */
    static auto getEndpoint(EndpointName endpointName) -> Endpoint;
};
