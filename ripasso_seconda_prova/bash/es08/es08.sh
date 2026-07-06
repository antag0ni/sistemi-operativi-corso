#!/bin/bash

# Creare uno script che abbia la sintassi:
#   ./line_counter.sh nome_directory direzione
#
# Lo script deve elencare i file contenuti nella directory con
# relativo numero di linee, ordinati in senso crescente
# (rispetto al numero di linee) se direzione è "up" e in senso
# decrescente se direzione è "down".
#
# Esempio: ./line_counter.sh mia_dir up
# potrebbe produrre come output:
#   27 mia_dir/prova.c
#   33 mia_dir/pippo.txt
#   142 mia_dir/programma.c
#
# NOTA: lo script deve controllare:
# - che il primo argomento sia effettivamente una directory
# - che il secondo argomento sia la stringa "up" o "down"
#
# Suggerimento: si usi il comando wc per ottenere i nomi dei
# file preceduti dal numero di linea e il comando sort per
# l'ordinamento.
# - wc -l file1 file2 ... stampa in output i nomi dei file
#   preceduti dal num. di linee
# - sort -n legge le linee del suo standard input, le ordina
#   in base al primo campo (interpretato come valore numerico)
#   e le stampa in output in ordine crescente
# - sort -nr fa la stessa cosa, ma in ordine decrescente

if [[ $# -ne 2 || ! -d "$1" ]]; then
    echo "Usage: $0 <directory> <up/down>"
    exit 1
fi

if [[ "$2" == "up" ]]; then
    for file in "$1"/*; do 
        if [[ -f "$file" ]]; then 
            wc -l "$file" 
        fi
    done | sort -n
elif [[ "$2" == "down" ]]; then
    for file in "$1"/*; do 
        if [[ -f "$file" ]]; then 
            wc -l "$file" 
        fi
    done | sort -nr
else
    echo "Invalid Parameter"
    exit 1
fi
exit 0