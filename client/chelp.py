from rich import print
from rich.table import Table, box


def help_command():
    commands_table = Table(box=box.SIMPLE, show_lines=True)

    commands_table.add_column("Syntaxe", justify="center", style="bold green")
    commands_table.add_column("Description", justify="center", style="white")

    commands_table.add_row("add", "Ajouter un adhérant en fournissant le prénom, nom, et classe")
    commands_table.add_row("find \[mot-clé]", "Liste tous les adhérants trouvé grâce au mot-clé")
    commands_table.add_row("log (id)", "Liste toutes les entrées de l'adhérant.")

    print(commands_table)

    print("\n[italic]Notes[/italic] : - [] signifie que le paramètre est obligatoire, () optionnel.\n"
    +                      "        - [] et () ne doivent pas être écrit dans la commande.\n"
    +                      "        - Si une commande s'accompagne de (id), cela signifie que l'ID peut-être scanné.")