#!/bin/bash

# Si scriva uno script bash che prende una stringa come
# parametro.
#
# Lo script deve stampare il nome di tutti i file regolari
# (ovvero, non sottodirectory) che contengono nel loro nome la
# stringa passata come parametro e il numero totale di file
# trovati.
#
# Si controlli che sulla linea di comando ci sia uno ed un
# solo parametro, stampando un messaggio di errore in caso
# contrario.
#
# NOTA: e' possibile utilizzare il comando find con l'opzione
# -name "*$string*" per trovare tutti i file nel cui nome è
# contenuta la stringa string.

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 <string>"
    exit 1
fi

string="$1"

cd test_es

find . -type f -name "*$string*"
cont=$(find . -type f -name "*$string*" | wc -l)
echo "totale: $cont"

exit 0