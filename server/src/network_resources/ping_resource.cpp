#include "resources.hpp"


std::shared_ptr<http_response> ping_resource::render(const http_request&)
{
    return std::shared_ptr<http_response>(new string_response(
        "pong", http::http_utils::http_ok
    ));
}