#  JSON
find_package(nlohmann_json REQUIRED)
set(JSON_LIBS "nlohmann_json::nlohmann_json")


#  {fmt}
find_package(fmt REQUIRED)
set(FMT_LIBS "fmt::fmt")


#  Spdlog
find_package(spdlog REQUIRED)
set(SPDLOG_LIBS "spdlog::spdlog")


#  SQLite 3
find_package(SQLite3 REQUIRED)
set(SQLITE3_LIBS "${SQLite3_LIBRARIES}")


#  libhttpserver
find_package(LibHttpServer REQUIRED)
set(LIBHTTPSERVER_LIBS "${LIBHTTPSERVER_LIBRARIES}")


#  Random
find_package(effolkronium_random REQUIRED)

set(RANDOM_LIBS effolkronium_random)