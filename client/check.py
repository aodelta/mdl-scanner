from rich import print

from common import send_request_to_server, format_id


def check_event(id: int):
    rep = send_request_to_server("POST", "/check?id={}".format(id))
    if rep == None:
        print("[bold][red]La requête a échouée.[/]")
        return

    if rep.status_code != 200:
        print("[bold][red]Le serveur a refusé la demande ([blue]{}[/blue]).[/]".format(rep.status_code))
        return

    print("✅ [bold][green]ID [blue]{}[/blue] scanné.[/]".format(format_id(id)))