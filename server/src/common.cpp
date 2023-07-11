#include "common.hpp"

#include <effolkronium/random.hpp>


timestamp_t get_timestamp()
{
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()
    );
}

timestamp_t get_null_timestamp()
{
    return std::chrono::seconds(0);
}

timestamp_t get_seconds_before_midnight()
{
    using namespace std::chrono;

    typedef duration<int, std::ratio_multiply<hours::period, std::ratio<24>>::type> days;
    system_clock::duration now = get_timestamp();

    now -= duration_cast<days>(now);

    return days(1) - duration_cast<seconds>(now);
}

#include <fmt/format.h>
ID_t get_random_id()
{
    using Random = effolkronium::random_static;

    ID_t id = 0;
    int nnumber = ceil(log10(MAX_ID));

    fmt::print("nnumber : {}\n", nnumber);

    while(id == 0) // if id = 0, start again
        for(int i = 0; i < nnumber; i++)
            id += Random::get(0, 9) * pow(10, i);

    return id;
}