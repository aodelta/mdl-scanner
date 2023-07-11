//
// Created by guill on 06/01/2023.
//

#ifndef MDL_SCANNER_COMMON_HPP
#define MDL_SCANNER_COMMON_HPP

#include <cstdint>
#include <string>
#include <tuple>
#include <chrono>

auto constexpr MAX_ID = 999999; // 10^6 - 1

typedef uint64_t ID_t;
typedef std::chrono::seconds timestamp_t;
typedef std::chrono::milliseconds::rep timestamp_count_t;

typedef std::tuple<ID_t, std::string, std::string, std::string, timestamp_count_t, timestamp_count_t> adherant_t;
typedef std::tuple<ID_t, timestamp_count_t, timestamp_count_t> log_entry_t;


ID_t get_random_id();
timestamp_t get_null_timestamp();
timestamp_t get_timestamp();
timestamp_t get_seconds_before_midnight();

#endif //MDL_SCANNER_COMMON_HPP
