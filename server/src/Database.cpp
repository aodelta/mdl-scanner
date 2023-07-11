#include "./Database.hpp"

#include <fmt/format.h>

std::shared_ptr<Database> db;
std::mutex db_mutex;

void Database::setup()
{
    int exit_code;
    exit_code = sqlite3_open("adherants.db", &this->db);

    if (exit_code != 0) {
        fmt::print("Error open DB {}\n", sqlite3_errmsg(this->db));
        this->error_status = EDatabaseError::FILE_ACCESS;
        return;
    }
    
    fmt::print("Opened Database Successfully\n");

    std::string query = R"(
        create table if not exists ADHERANTS(
                ID            int               primary key,
                NAME          text              not null,
                FIRST_NAME    text,
                CLASS         text,
                CREATION_DATE timestamp,
                LAST_VISIT    timestamp
        );
    )";

    if(!this->query(query.c_str())) {
        this->close();
        return;
    }

    query = R"(
        create table if not exists LOGS(
                ID            int               not null,
                DATE          timestamp         not null,
                EXIT_DATE     timestamp
        );
    )";

    if(!this->query(query.c_str())) {
        this->close();
        return;
    }

    this->error_status = EDatabaseError::NO_ERROR;
}

void Database::close()
{
    sqlite3_close(this->db);
}

bool Database::add_user(std::string_view &name, std::string_view &first_name, std::string_view &class_)
{
    ID_t id;
    do {
        if(!this->is_last_query_successful())
            return false;
        id = get_random_id();
    } while(this->is_id_in_use(id));

    auto creation_date = get_timestamp();

    std::string query = fmt::format(R"(
        insert into ADHERANTS (ID, NAME, FIRST_NAME, CLASS, CREATION_DATE)
        values ({}, "{}", "{}", "{}", {})
    )", id, name, first_name, class_, creation_date.count());

    if(!this->query(query.c_str()))
        return false;
    return true;
}

bool Database::is_id_in_use(ID_t id)
{
    std::string query = fmt::format(R"(
        select ID from ADHERANTS where ID = {}
    )", id);

    int count = 0;
    bool query_result = this->query(query.c_str(), [](void* count, int, char**, char**) -> int {
        (*(int*)count)++;
        return 0;
    }, &count);

    if(!query_result || count == 0)
        return false;
    return true;
}

bool Database::query(const char *sql_query, int (*callback)(void *, int, char **, char **), void *relay)
{
    const std::lock_guard<std::mutex> lock(db_mutex);

    char* errmsg = nullptr;
    sqlite3_exec(this->db, sql_query, callback, relay, &errmsg);

    if(errmsg != nullptr) {
        fmt::print("Error : {}\n", errmsg);
        this->error_status = EDatabaseError::QUERY;
        return false;
    }
    this->error_status = EDatabaseError::NO_ERROR;
    return true;
}

bool Database::add_log(ID_t id, timestamp_t date, timestamp_t exit_date)
{
    std::string query = fmt::format(R"(
        insert into LOGS (ID, DATE, EXIT_DATE)
        values ({}, {}, {})
    )", id, date.count(), exit_date.count());

    if(!this->query(query.c_str()))
        return false;
    return true;
}

bool Database::update_last_date(ID_t id, timestamp_t date)
{
    std::string query = fmt::format(R"(
        update ADHERANTS
        set
            LAST_VISIT = {}
        where
            ID = {}
    )", date.count(), id);

    if(!this->query(query.c_str()))
        return false;
    return true;
}

bool Database::find_adherant(const std::string_view &keyword, std::vector<adherant_t>* adherants_matches)
{
    std::string query;
    if(keyword.compare("*") == 0) {
        query = R"(
            select *
            from ADHERANTS
        )";
    }
    else {
        query = fmt::format(R"(
            select *
            from ADHERANTS
            where
                   (instr(ID, '{keyword}') > 0)
                or (instr(NAME, '{keyword}') > 0)
                or (instr(FIRST_NAME, '{keyword}') > 0)
                or (instr(CLASS, '{keyword}') > 0);
        )", fmt::arg("keyword", keyword));
    }

    if(!this->query(query.c_str(), [](void* vec, int, char** data, char**) -> int {
        ID_t id = std::stoi(data[0]);
        std::string name(data[1]);
        std::string first_name(data[2]);
        std::string class_(data[3]);
        timestamp_count_t creation_date = std::stoi((data[4]));

        timestamp_count_t last_visit = 0;
        if(data[5] != nullptr)
            last_visit = std::stoi((data[5]));

        fmt::print("{} / {} / {} / {} / {} / \n", id, name, first_name, class_, creation_date, last_visit);
        ((std::vector<adherant_t>*)vec)->push_back(
            std::make_tuple(id, name, first_name, class_, creation_date, last_visit)
        );
        return 0;
    }, adherants_matches))
        return false;
    return true;
}

bool Database::get_all_log_entry(ID_t id, std::vector<log_entry_t>* adherant_log_entry)
{
    std::string query = fmt::format(R"(
        select *
        from LOGS
        where
               ID = {id}
    )", fmt::arg("id", id));

    struct vec_id_struct {
        std::vector<log_entry_t>* adherant_log_entry;
        ID_t id;
    };

    vec_id_struct vec_id{adherant_log_entry, id};

    fmt::print("id : {}\n", id);

    if(!this->query(query.c_str(), [](void* vec_id, int, char** data, char**) -> int {
        timestamp_count_t date = std::stoi((data[1]));

        timestamp_count_t exit_date = 0;
        if(data[2] != nullptr)
            exit_date = std::stoi((data[2]));

        fmt::print("{} / {}\n", date, exit_date);
        ((vec_id_struct*)vec_id)->adherant_log_entry->push_back(
                std::make_tuple(((vec_id_struct*)vec_id)->id, date, exit_date)
        );
        return 0;
    }, &vec_id))
        return false;
    return true;
}
