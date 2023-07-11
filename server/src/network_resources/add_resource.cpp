#include "./resources.hpp"

#include "../Database.hpp"


std::shared_ptr<http_response> add_resource::render(const http_request& req)
{
    if(!check_auth(req))
        return std::shared_ptr<http_response>(new string_response(
            "", http::http_utils::http_unauthorized
        ));

    auto vname       = req.get_arg("name");
    auto vfirst_name = req.get_arg("first_name");
    auto vclass_     = req.get_arg("class");

    std::string name;
    std::string first_name;
    std::string class_;

    replace_url_char(name, vname);
    replace_url_char(first_name, vfirst_name);
    replace_url_char(class_, vclass_);

    if(vname.empty() || vfirst_name.empty())
        return std::shared_ptr<http_response>(new string_response(
            "name or first_name empty", http::http_utils::http_bad_request
        ));

    fmt::print("name : {} / first_name : {} / class : {}\n", vname, vfirst_name, vclass_);

    if(!db->add_user(vname, vfirst_name, vclass_))
        return std::shared_ptr<http_response>(new string_response(
            "", http::http_utils::http_internal_server_error
        ));

    return std::shared_ptr<http_response>(new string_response(
        "", http::http_utils::http_ok
    ));
}