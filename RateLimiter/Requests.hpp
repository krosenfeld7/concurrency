#pragma once

/*
Basic Request / Response implementation
*/

#include <iostream>
#include <string>

typedef unsigned UserId;

typedef std::string Body;
typedef std::string EndpointName;

class RequestBody {
public:
    RequestBody(std::string message)
    : message{message}
    { }

    auto getMessage() const -> std::string { return message; }

    friend auto operator<<(std::ostream& out, const RequestBody& body) 
    -> std::ostream& {
        out << body.getMessage();
        return out;
    }
private:
    std::string message;
};

class Request {
public:
    Request(UserId userId, EndpointName dest, Body body)
    : userId{userId}
    , dest{dest}
    , body{RequestBody{body}}
    { }

    auto getUserId() const -> UserId { return userId; }
    auto getDestination() const -> EndpointName { return dest; }
    auto getBody() const -> RequestBody { return body; }

    friend auto operator<<(std::ostream& out, const Request& request) 
    -> std::ostream& {
        out << "User: " << request.userId << ": " << request.dest << ": " 
            << request.getBody();
        return out;
    }

private:
    UserId userId;
    EndpointName dest;
    RequestBody body;
};

class ResponseBody {
public:
    ResponseBody(std::string message)
    : message{message}
    { }

    auto getMessage() const -> std::string { return message; }

    friend auto operator<<(std::ostream& out, const ResponseBody& body) 
    -> std::ostream& {
        out << body.getMessage();
        return out;
    }

private:
    std::string message;
};

class Response {
public:
    Response(UserId userId, Body body)
    : userId{userId}
    , body{ResponseBody{body}}
    { }

    auto getUserId() const -> UserId { return userId; }
    auto getBody() const -> ResponseBody { return body; }

    friend auto operator<<(std::ostream& out, const Response& response) 
    -> std::ostream& {
        out << "User: " << response.userId << ": " << response.getBody();
        return out;
    }

private:
    UserId userId;
    EndpointName dest;
    ResponseBody body;
};
