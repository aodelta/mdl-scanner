#include "network_common.hpp"


bool check_auth(const http_request &req)
{
    return req.get_header("Auth").compare("wP90m&tNlULJ") == 0;
}

void replace_url_char(std::string& str, std::string_view& vstr)
{
    str.resize(vstr.size());

    for(const char& c: vstr) {
        if (c == '+')
            str += '+';
        else
            str += c;
    }
}

bool get_id(const http_request &req, ID_t* id)
{
    auto vid = req.get_arg("id");
    *id = 0;

    if(vid.empty())
        return false;

    try {
        int tmp_id = std::stoi(vid.data()); // throw if fails
        if(tmp_id > 0 && tmp_id <= MAX_ID) // ID limitation, in line with type's
            *id = (ID_t)tmp_id;
    } catch(const std::exception&) {}

    if(*id == 0)
        return false;
    return true;
}