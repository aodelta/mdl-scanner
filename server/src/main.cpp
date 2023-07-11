#include <memory>

#include <httpserver.hpp>

#include "./Database.hpp"
#include "./network_resources/resources.hpp"


static std::shared_ptr<webserver>           ws;
static std::shared_ptr<ping_resource>       ping_check_rsc;
static std::shared_ptr<auth_check_resource> auth_check_rsc;
static std::shared_ptr<check_resource>      check_rsc;
static std::shared_ptr<add_resource>        add_rsc;
static std::shared_ptr<find_resource>       find_rsc;
static std::shared_ptr<log_resource>       log_rsc;

void setup_network()
{
    ws = std::make_shared<webserver>(create_webserver(8080));

    ping_check_rsc = std::make_shared<ping_resource>();
    ping_check_rsc->disallow_all();
    ping_check_rsc->set_allowing("GET", true);
    ws->register_resource("/ping", &(*ping_check_rsc));

    auth_check_rsc = std::make_shared<auth_check_resource>();
    auth_check_rsc->disallow_all();
    auth_check_rsc->set_allowing("GET", true);
    ws->register_resource("/auth_check", &(*auth_check_rsc));

    check_rsc = std::make_shared<check_resource>();
    check_rsc->disallow_all();
    check_rsc->set_allowing("POST", true);
    ws->register_resource("/check", &(*check_rsc));

    add_rsc = std::make_shared<add_resource>();
    add_rsc->disallow_all();
    add_rsc->set_allowing("POST", true);
    ws->register_resource("/add", &(*add_rsc));

    find_rsc = std::make_shared<find_resource>();
    find_rsc->disallow_all();
    find_rsc->set_allowing("GET", true);
    ws->register_resource("/find", &(*find_rsc));

    log_rsc = std::make_shared<log_resource>();
    log_rsc->disallow_all();
    log_rsc->set_allowing("GET", true);
    ws->register_resource("/log", &(*log_rsc));
}

int main()
{
    db = std::make_shared<Database>();

    db->setup();

    if(!db->is_last_query_successful())
        return 1;

    setup_network();

    if(!ws->start(true))
        return 2;

    db->close();
    
    return 0;
}