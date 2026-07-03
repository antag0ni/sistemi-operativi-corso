#!/bin/bash

# Si modifichi lo script dell'Esercizio 2 in modo da
# passare allo script lo username come argomento dello script.

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <username>"
    exit 1
fi

username="$1"

n_login=$(who | grep -w "$username" | wc -l)
echo "Numero di login effettuati dall'utente: $n_login"
exit 0