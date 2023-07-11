#pragma once

#include <string>

#include <httpserver.hpp>

#include "../Database.hpp"


using namespace httpserver;


bool check_auth(const http_request &req);
void replace_url_char(std::string& str, std::string_view& vstr);
bool get_id(const http_request &req, ID_t* id);