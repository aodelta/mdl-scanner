import json

from rich import print
from rich.table import Table, box

from common import id_regex, send_request_to_server, timestamp_to_date, format_id, print_tabs


def log_command(args):
    if len(args) > 1:
        # trop d'argument
        pass

    ID = None

    for arg in args:
        if id_regex.match(arg) != None:
            ID = int(arg)
            break
    
    if ID == None:
        print("bip bip scan")
        pass

    rep = send_request_to_server("GET", "/log?id={}".format(ID))

    if rep == None:
        print("❌ [bold][red]La requête a échouée.[/]")
        return
    if rep.status_code != 200:
        print("❌ [bold][red]Le serveur a refusé la demande ([blue]{}[/blue]).[/]".format(rep.status_code))
        return

    logs = []

    json_rep = json.loads(rep.content.decode())

    if json_rep != None:
        logs = json_rep
    
    logs_table = Table(box=box.SIMPLE, show_lines=True)
    
    logs_table.add_column("ID",             justify="center", style="bold white")
    logs_table.add_column("Date d'entrée",  justify="center", style="bold white")
    logs_table.add_column("Date de sortie", justify="center", style="bold white")

    logs = sorted(logs, key=lambda log: int(log["DATE"]))
                
    def add_log_to_table(log: str, logs_table: Table) -> None:
        id_str        = format_id(log["ID"])
        date_str      = timestamp_to_date(log["DATE"])
        exit_date_str = timestamp_to_date(log["EXIT_VISIT"])
        
        if exit_date_str.startswith("01/01/1970"):
            exit_date_str = "---"
        
        logs_table.add_row(id_str, date_str, exit_date_str)

    def get_log_table() -> Table:
        logs_table = Table(box=box.SIMPLE, show_lines=True)
        
        logs_table.add_column("ID",             justify="center", style="bold white")
        logs_table.add_column("Date d'entrée",  justify="center", style="bold white")
        logs_table.add_column("Date de sortie", justify="center", style="bold white")

        return logs_table

    print_tabs(logs, add_log_to_table, get_log_table)