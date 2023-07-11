import sys

from rich import print
from rich.prompt import Prompt

from common import id_regex
from tests import test_auth, test_network
from check import check_event
from chelp import help_command
from cadd  import add_command
from cfind import find_command
from clog  import log_command


def main():
    should_exit = False

    if not "-no-check" in sys.argv:
        network_status = test_network()
        if network_status == "NO_INTERNET":
            print("❌ [bold][red]Internet indisponible.[/]")
            return 1
        elif network_status == "SERVER_UNREACHABLE":
            print("❌ [bold][red]Le serveur n'est pas atteignable.[/]")
            return 2

        print("✅ [bold][green]Connection établie.[/]")

        if not test_auth():
            print("[bold][red]L'authentification a échoué. Clé non valide ou réseau non disponible.[/]")
            return 3

        print("✅ [bold][green]Identité vérifiée.[/]")

    inp = None

    while not should_exit:
        try:
            inp = Prompt.ask("> ")
        except KeyboardInterrupt:
            inp = "exit"

        print()

        args = inp.split()
        if len(args) == 0:
            continue
            
        command = args[0]

        if id_regex.match(command):
            check_event(int(command))
        elif command == "help":
            help_command()
        elif command == "add":
            add_command()
        elif command == "find":
            find_command(args[1:])
        elif command == "log":
            log_command(args[1:])
        elif command == "exit" or command == "quit":
            should_exit = True
            print("[blue][bold]Fin de session.[/]")
        else:
            print("[red][bold]Commande non reconnue ([blue]help[/blue] pour plus de détails).[/]")

        print()


exit(main())