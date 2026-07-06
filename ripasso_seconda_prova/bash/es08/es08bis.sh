#!/bin/bash

# Modificare lo script precedente facendo in modo da inserire
# tutte le linee lette dai file in un file secondo
# l'ordinamento prescelto e, successivamente, si effettui il
# conteggio.
#
# Si usi il comando cat che visualizza a video il contenuto
# di un file.

if [[ $# -ne 2 || ! -d "$1" ]]; then
    echo "Usage: $0 <directory> <up/down>"
    exit 1
fi

> output.txt

if [[ "$2" == "up" ]]; then
    for file in "$1"/*; do 
        if [[ -f "$file" ]]; then 
            cat "$file" 
        fi
    done | sort -n > output.txt
elif [[ "$2" == "down" ]]; then
    for file in "$1"/*; do 
        if [[ -f "$file" ]]; then 
            cat "$file" 
        fi
    done | sort -nr > output.txt
else
    echo "Invalid Parameter"
    exit 1
fi

cat output.txt
wc -l output.txt

exit 0