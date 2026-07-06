#!/bin/bash

# si scriva uno script bash che prende una directory come
# parametro.
#
# lo script deve cambiare il nome di tutti i file regolari
# (ovvero, non sottodirectory) che contengono nel loro nome
# caratteri maiuscoli rendendoli minuscoli.
#
# si controlli che sulla linea di comando ci sia uno ed un
# solo parametro, stampando un messaggio di errore in caso
# contrario.
#
# nota: è possibile utilizzare il comando tr a-z a-z per
# trasformare i caratteri da minuscolo a maiuscolo di una
# stringa ed il comando *[[:upper:]]* per il confronto con il
# nome del file e verificare se il nome del file contiene
# almeno un carattere maiuscolo.

#./reset.sh > /dev/null 2>&1

if [[ $# -ne 1 || ! -d "$1" ]]; then
    echo "provide a directory"
    exit 1
fi

cd "$1"

for f in *; do
    if [[ -f "$f" && "$f" == *[[:upper:]]* ]]; then
        echo "$f"
        new_name=$(echo "$f" | tr A-Z a-z)
        mv -n "$f" "$new_name"
    fi 
done

exit 0