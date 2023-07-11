#include "./resources.hpp"

#include <algorithm>
#include <thread>
#include <mutex>

using namespace std::chrono;

[[noreturn]] void cleaner_thread_function();

std::list<check_t> active_accesses; // list of current adherants who are believed to be inside
std::mutex active_accesses_mutex;
std::thread cleaner_thread(cleaner_thread_function);


[[noreturn]] void cleaner_thread_function()
{
    while(true) {
        timestamp_t countdown = get_seconds_before_midnight();
        std::this_thread::sleep_for(countdown);

        const std::lock_guard<std::mutex> lock(active_accesses_mutex);

        for(const auto& adherant: active_accesses) {
            ID_t id = std::get<0>(adherant);
            timestamp_t date = std::get<1>(adherant);

            db->add_log(id, date, get_null_timestamp());
            db->update_last_date(id, date);
        }

        std::this_thread::sleep_for(5s); // avoid multiple cycle at midnight if cleaning to fastly

        active_accesses.clear();
    }
}

check_resource::check_resource()
{
    cleaner_thread.detach();
}

std::shared_ptr<http_response> check_resource::render(const http_request &req)
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

    // adherant currently inside ?
    auto adherant = std::find_if(active_accesses.begin(), active_accesses.end(),
                                 [&id](check_t entry){return std::get<0>(entry) == id; });

    // not registered in active accesses
    if(adherant == active_accesses.end()) {
        // if ID actually exists
        if(!db->is_id_in_use(id)) {
            if(!db->is_last_query_successful()) // database error
                return std::shared_ptr<http_response>(new string_response(
                    "Unable to check ID", http::http_utils::http_internal_server_error
                ));
            // client error, invalid ID according to database
            return std::shared_ptr<http_response>(new string_response(
                "ID doesn't exist", http::http_utils::http_bad_request
            ));
        }

        // add adherant to active accesses
        active_accesses.emplace_back(id, get_timestamp());
    }
    else { // adherant is registered in active accesses
        timestamp_t date = std::get<1>(*adherant);

        bool add_query    = db->add_log(id, date, get_timestamp());
        bool update_query = db->update_last_date(id, date);
        active_accesses.erase(adherant);

        if(!add_query || !update_query) {
            return std::shared_ptr<http_response>(new string_response(
                "Couldn't save exit", http::http_utils::http_internal_server_error
            ));
        }
    }

    return std::shared_ptr<http_response>(new string_response(
        "", http::http_utils::http_ok
    ));
}