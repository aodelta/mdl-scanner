#include "resources.hpp"


std::shared_ptr<http_response> auth_check_resource::render(const http_request& req)
{
    if(!check_auth(req))
        return std::shared_ptr<http_response>(new string_response(
            "ERR", http::http_utils::http_ok
        ));

    return std::shared_ptr<http_response>(new string_response(
        "OK", http::http_utils::http_ok
    ));
}