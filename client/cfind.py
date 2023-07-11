import json

from rich import print
from rich.table import Table, box

from common import send_request_to_server, timestamp_to_date, format_id, print_tabs


def find_command(args: str):
    if len(args) == 0:
        print("[bold][magenta]Aucun argument détecté ([blue]help[/blue] pour plus détails)[/]")
        return

    matches = []

    rep = send_request_to_server("GET", "/find?keyword={}".format(args[0]))
    if rep == None:
        print("❌ [bold][red]La requête a échouée.[/]")
        return
    if rep.status_code != 200:
        print("❌ [bold][red]Le serveur a refusé la demande ([blue]{}[/blue]).[/]".format(rep.status_code))
        return

    json_rep = json.loads(rep.content.decode())

    if json_rep != None:
        matches = json_rep

    matches = sorted(matches, key=lambda adherant: adherant["NAME"].lower())
    
    def add_adh_to_table(adh: str, matches_table: Table) -> None:
        id_str            = format_id(adh["ID"])
        creation_date_str = timestamp_to_date(adh["CREATION_DATE"])
        last_visit_str    = timestamp_to_date(adh["LAST_VISIT"])
        
        if last_visit_str.startswith("01/01/1970"):
            last_visit_str = "---"
        
        matches_table.add_row(id_str, adh["NAME"], adh["FIRST_NAME"],
                              adh["CLASS"], creation_date_str, last_visit_str)

    def get_matches_table() -> Table:
        matches_table = Table(box=box.SIMPLE, show_lines=True)
        
        matches_table.add_column("ID",     justify="center", style="bold white")
        matches_table.add_column("Nom",    justify="center", style="bold white")
        matches_table.add_column("Prénom", justify="center", style="bold white")
        matches_table.add_column("Classe", justify="center", style="bold white")
        matches_table.add_column("Date de création", justify="center", style="bold white")
        matches_table.add_column("Dernière visite",  justify="center", style="bold white")

        return matches_table

    print_tabs(matches, add_adh_to_table, get_matches_table)