#!/bin/bash

# Scrivere uno script bash in grado di:
# - ricevere due interi n1 e n2 sulla riga di comando oppure
#   leggerli da tastiera se non sono presenti sulla riga di
#   comando
# - visualizzare una matrice di n1 righe e n2 colonne di
#   valori interi crescenti a partire dal valore 0
#
# Esempio:
# > ./myScript 3 4
# 0 1 2 3
# 4 5 6 7
# 8 9 10 11

if [[ $# -ne 2 ]]; then
    read -p "Inserire n1: " n1
    read -p "Inserire n2: " n2
else
    n1=$1
    n2=$2
fi

i=0
num=0
while [[ $i -lt $n1 ]]; do
    j=0
    while [[ $j -lt $n2 ]]; do
        echo -n "$num "
        ((j++))
        ((num++))
    done
    ((i++))
    echo ""
done

exit 0