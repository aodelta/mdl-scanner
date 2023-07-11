#include "resources.hpp"


std::shared_ptr<http_response> find_resource::render(const http_request& req)
{
    if(!check_auth(req))
        return std::shared_ptr<http_response>(new string_response(
            "", http::http_utils::http_unauthorized
        ));

    auto vkeyword = req.get_arg("keyword");
    if(vkeyword.empty())
        return std::shared_ptr<http_response>(new string_response(
            "Empty keyword", http::http_utils::http_bad_request
        ));

    std::vector<adherant_t> adherant_matches;
    db->find_adherant(vkeyword, &adherant_matches);

    json jadherants;
    for(const auto& adherant: adherant_matches) {
        json jadherant = json::parse(fmt::format(R"(
            {{
                "ID"            : {},
                "NAME"          : "{}",
                "FIRST_NAME"    : "{}",
                "CLASS"         : "{}",
                "CREATION_DATE" : {},
                "LAST_VISIT"    : {}
            }}
        )", std::get<0>(adherant), std::get<1>(adherant), std::get<2>(adherant),
            std::get<3>(adherant), std::get<4>(adherant), std::get<5>(adherant)));

        jadherants.push_back(jadherant);
    }

    return std::shared_ptr<http_response>(new string_response(
        to_string(jadherants), http::http_utils::http_ok
    ));
}