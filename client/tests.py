from common import send_request_to_server, ping


def test_network():
    urls = ["www.google.com", "www.facebook.com", "www.ovhcloud.com"]
    failcount = 0

    for url in urls:
        if not ping(url):
            failcount += 1

    if failcount == len(urls):
        return "NO_INTERNET"

    rep = send_request_to_server("GET", "/ping")
    if rep == None or rep.status_code != 200 or rep.content.decode() != "pong":
        return "SERVER_UNREACHABLE"


def test_auth():
    rep = send_request_to_server("GET", "/auth_check")

    if rep == None or rep.content.decode() != "OK":
        return False

    return True