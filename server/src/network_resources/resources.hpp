#pragma once

#include <list>
#include <tuple>
#include <string>

#include <fmt/format.h>
#include <httpserver.hpp>
#include <nlohmann/json.hpp>

#include "./network_common.hpp"


using namespace httpserver;
using json = nlohmann::json;
using namespace nlohmann::literals;

typedef std::tuple<ID_t, timestamp_t> check_t;


class ping_resource : public http_resource {
public:
    std::shared_ptr<http_response> render(const http_request&) final;
};

class log_resource : public http_resource {
public:
    std::shared_ptr<http_response> render(const http_request&) final;
};

class find_resource : public http_resource {
public:
    std::shared_ptr<http_response> render(const http_request&) final;
};

class check_resource : public http_resource {
public:
    check_resource();
    std::shared_ptr<http_response> render(const http_request& req) final;
};

class auth_check_resource : public http_resource {
public:
    std::shared_ptr<http_response> render(const http_request& req) final;
};

class add_resource : public http_resource {
public:
    std::shared_ptr<http_response> render(const http_request& req) final;
};