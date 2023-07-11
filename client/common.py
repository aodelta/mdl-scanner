import platform
import subprocess
import requests
import re
import keyboard
from math import log10, ceil
from datetime import datetime
from collections.abc import Callable

from rich import print
from rich.console import Group
from rich.progress import Progress, SpinnerColumn, TimeElapsedColumn
from rich.prompt import Prompt
from rich.text import Text
from rich.table import Table
from rich.live import Live


id_regex = re.compile("^([0-9]){6}$") # "xxxxxx", x E [0;9]


AUTH_KEY = "wP90m&tNlULJ"
SERVER_IP = "localhost:8080"


def get_auth_header():
    return {"Auth" : AUTH_KEY}


def send_request_to_server(method: str, url: str):
    f = None

    if method == "GET":
        f = requests.get
    elif method == "POST":
        f = requests.post
    else:
        return None


    with Progress(
        SpinnerColumn(),
        *Progress.get_default_columns(),
        TimeElapsedColumn(),
        transient=True,
    ) as progress:

        progress.add_task("[cyan]Chargement ([red][bold]{}[/bold][cyan])...".format(method), total=None)

        try:
            return f(url="http://{}{}".format(SERVER_IP, url), headers=get_auth_header())
        except:
            return None


# <3 https://stackoverflow.com/questions/2953462/pinging-servers-in-python
def ping(host: str):
    param = '-n' if platform.system().lower() =='windows' else '-c'

    command = ['ping', param, '1', host]

    with Progress(
        SpinnerColumn(),
        *Progress.get_default_columns(),
        TimeElapsedColumn(),
        transient=True,
    ) as progress:
        progress.add_task("[blue]Chargement ([red]ping [bold]{}[/bold][blue])...".format(host), total=None)

        try:
            subprocess.check_output(command, timeout=0.5) # Raise CalledProcessError on non-zero exit status
            return True
        except:
            return False
        # , '''stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, stdin=subprocess.DEVNULL'''


def ask_non_empty(question):
    answer = Prompt.ask(question)
    while answer == "":
        print("[magenta][bold]Le champ ne peut pas être laissé vide.[/]")
        answer = Prompt.ask(question)
    return answer


def timestamp_to_date(timestamp: int):
    date = datetime.fromtimestamp(timestamp)

    day     = date.day    if date.day    >= 10 else "0{}".format(date.day)
    month   = date.month  if date.month  >= 10 else "0{}".format(date.month)
    hour    = date.hour   if date.hour   >= 10 else "0{}".format(date.hour)
    minute  = date.minute if date.minute >= 10 else "0{}".format(date.minute)

    return "{}/{}/{} {}h{}".format(day , month, date.year, hour, minute)


def format_id(id: int):
    zeroes = ""
    for _ in range(6 - ceil(log10(id))):
        zeroes += "0"
    return "{}{}".format(zeroes, id)


def is_yes(ans: str):
    return ans == "o" or ans == "oui" or ans == "y" or ans == "yes"


def is_no(ans: str):
    return ans == "n" or ans == "non" or ans == "no"


def print_tabs(array, add_function: Callable[[str, Table], None], table_function: Callable[[], Table]):
    table = table_function()

    ntab = ceil(len(array) / 10)

    if ntab <= 1:
        for element in array:
            add_function(element, table)

        print(table)

    else:
        i = 0
        with Live() as live:
            while True:
                #nelement = 10 if i != ntab - 1 else len(logs) - 10*(ntab-1)

                table = table_function()

                for j in range(0, 10):
                    if (10*i + j) >= len(array):
                        table.add_row("", "", "")
                        continue

                    element = array[10*i + j]
                    add_function(element, table)

                warning_text = "{}[bold][yellow]MODE INTERACTIF[/]\n".format(
                                    "".join([" " for _ in range(0, 5)]),
                )

                
                query_result_text = "\n[#BBBBBB][not bold]{} résultat(s) trouvé(s).[/] [cyan]{}[/]/{}".format(len(array), i+1, ntab)

                group = Group(warning_text, table, query_result_text)
                live.update(group)

                quit_printing = False

                while True:
                    ev = keyboard.read_event()

                    if ev.scan_code == 77:
                        if ev.event_type == "up" or i == (ntab - 1):
                            continue
                        i += 1
                    elif ev.scan_code == 75:
                        if ev.event_type == "up" or i == 0:
                            continue
                        i -= 1
                    else:
                        quit_printing = True
                    
                    break
                
                if quit_printing:
                    group = Group(table, query_result_text)
                    live.update(group)
                    return