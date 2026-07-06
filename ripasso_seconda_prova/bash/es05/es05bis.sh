#!/bin/bash

# Si scriva lo script dell'esercizio 5 passando i nomi dei
# file come argomenti dello script

> out.txt

if [[ $# != 1 ]]; then
    echo "provide file name"
    exit 1
fi

cont=1

while read riga; do
    echo "$cont $riga" >> out.txt
    ((cont++))
done < "$1"
cat ./out.txt

exit 0