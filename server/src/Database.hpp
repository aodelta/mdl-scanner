#pragma once

#include <memory>
#include <vector>
#include <chrono>
#include <mutex>

#include <sqlite3.h>

#include "./common.hpp"


enum EDatabaseError {
    NO_ERROR = 0,
    FILE_ACCESS,
    QUERY
};


class Database {
public:
    Database() = default;

    void setup();
    void close();

    bool add_user(std::string_view& name, std::string_view& first_name, std::string_view& class_);
    bool add_log(ID_t id, timestamp_t date, timestamp_t exit_date);
    bool update_last_date(ID_t id, timestamp_t date);
    bool find_adherant(const std::string_view& keyword, std::vector<adherant_t>* adherants_matches);
    bool get_all_log_entry(ID_t id, std::vector<log_entry_t>* adherant_log_entry);

    bool is_id_in_use(ID_t id);

    bool query(const char* sql_query, int (*callback)(void*, int, char**, char **) = nullptr, void* relay = nullptr);

    [[nodiscard]] EDatabaseError err() const { return this->error_status; };
    [[nodiscard]] bool is_last_query_successful() const { return this->err() == EDatabaseError::NO_ERROR; }

private:
    sqlite3* db{};
    EDatabaseError error_status;
};

extern std::shared_ptr<Database> db;
extern std::mutex db_mutex;