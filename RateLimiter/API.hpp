#pragma once

/*
This is an example API with functions that can be called.
*/

#include <functional>

#include "Requests.hpp"

typedef std::function<Response(UserId, RequestBody)> Endpoint;

class API {
public:
    static auto post(UserId userId, RequestBody body) -> Response {
        // do something
        return Response{userId, "POST called with body: " 
            + body.getMessage()};
    }
    static auto get(UserId userId, RequestBody body) -> Response {
        // do something
        return Response{userId, "GET called with body: " 
            + body.getMessage()};
    }
    static auto put(UserId userId, RequestBody body) -> Response {
        // do something
        return Response{userId, "UPDATE called with body: " 
            + body.getMessage()};
    }
    static auto del(UserId userId, RequestBody body) -> Response {
        // do something
        return Response{userId, "DELETE called with body: " 
            + body.getMessage()};
    }
    static auto error(UserId userId, RequestBody body) -> Response {
        // do something
        return Response{userId, "ERROR"};
    }
};