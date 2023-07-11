from rich import print
from rich.prompt import Prompt

from common import send_request_to_server, ask_non_empty, is_yes, is_no


def check_exit(answer: str):
    if answer == "exit" or answer == "quit" or answer == "cancel" or answer == "back" or answer == "retour":
        print("[bold][blue]Enregistrement annulé[/]")
        return True
    return False


def add_command():
    first_name = ask_non_empty("[bold]Prénom ")
    if check_exit(first_name):
        return

    name = ask_non_empty("[bold]Nom    ")
    if check_exit(name):
        return

    class_ = Prompt.ask("[bold]Classe ")
    if check_exit(class_):
        return

    ans = None

    while not is_yes(ans) and not is_no(ans):
        ans = Prompt.ask("\n[bold]{} {} {sep} {} {sep}[/bold] [[underline]O[/underline]ui/[underline]N[/underline]on] ?" 
                        .format(first_name, name, class_, sep="[blue not bold]|[/blue not bold]")).strip().lower()

    if is_no(ans):
        print("[bold][blue]Enregistrement annulé[/]")
        return

    first_name = first_name.strip().replace(" ", "+")
    name       = name.strip().replace(" ", "+")
    class_     = class_.strip().replace(" ", "+")
    
    url = "/add?name={}&first_name={}&class={}".format(name, first_name, class_)
    
    rep = send_request_to_server("POST", url)
    if rep == None:
        print("❌ [bold][red]La requête a échouée.[/]")
        return

    if rep.status_code != 200:
        print("❌ [bold][red]Le serveur a refusé la demande ([blue]{}[/blue]).[/]".format(rep.status_code))
        return

    print("✅ [bold][green]Adhérant ajouté avec succès[/]")