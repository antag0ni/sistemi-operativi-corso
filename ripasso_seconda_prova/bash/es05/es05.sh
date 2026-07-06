#!/bin/bash

# Si scriva uno script che legga da un file con nome in.txt
# tutte le linee di testo e per ogni linea determini il numero
# della linea letta e stampi su un file con nome out.txt il
# numero di linea e la linea letta

> out.txt

cont=1

while read riga; do
    echo "$cont $riga" >> out.txt
    ((cont++))
done < ./in.txt
cat ./out.txt
exit 0