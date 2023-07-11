#include "resources.hpp"

//#include <regex>

//static std::regex id_reg("^[1-9]([0-9]?){3}$");

std::shared_ptr<http_response> log_resource::render(const http_request& req)
{
    if(!check_auth(req))
        return std::shared_ptr<http_response>(new string_response(
                "", http::http_utils::http_unauthorized
        ));

    ID_t id = 0; // since 0 is a forbidden ID, it's used to check if a correct ID will be assigned right after

    if(!get_id(req, &id))
        return std::shared_ptr<http_response>(new string_response(
                "ID not valid", http::http_utils::http_bad_request
        ));

    std::vector<log_entry_t> adherant_log_entry;
    db->get_all_log_entry(id, &adherant_log_entry);

    json jentries;
    for(const auto& entry: adherant_log_entry) {
        json jentry = json::parse(fmt::format(R"(
            {{
                "ID"         : {id},
                "DATE"       : {date},
                "EXIT_VISIT" : {exit_date}
            }}
        )", fmt::arg("id", id), fmt::arg("date", std::get<1>(entry)),
            fmt::arg("exit_date", std::get<2>(entry))));

        jentries.push_back(jentry);
    }

    return std::shared_ptr<http_response>(new string_response(
            to_string(jentries), http::http_utils::http_ok
    ));
}