#!/bin/bash

# Esercizio 05
# Progettare uno script che prende esattamente DUE parametri sulla linea di comando:
# 1. Il nome di un file di testo (che contiene una lista di studenti e i loro voti).
# 2. Un numero intero che rappresenta la soglia di sbarramento (es. 18).
#
# Lo script deve eseguire i seguenti controlli:
# - Verificare che siano stati passati esattamente due parametri.
# - Verificare che il primo parametro sia un file regolare (usa la variabile $FILE_DATI).
# 
# Se i controlli vanno a buon fine, lo script deve leggere il file riga per riga.
# Ogni riga del file è formattata con 3 campi separati da spazio: NOME COGNOME VOTO.
# Lo script deve valutare il voto di ciascuno studente:
# - Se il voto è MAGGIORE O UGUALE alla soglia (secondo parametro), lo studente è "Ammesso" 
#   e la riga "NOME COGNOME" deve essere salvata in un file chiamato "ammessi.txt".
# - Se il voto è MINORE della soglia, lo studente è "Respinto" e la riga "NOME COGNOME" 
#   deve essere salvata in un file chiamato "respinti.txt".
#
# SUGGERIMENTO
# Per leggere un file riga per riga estraendo i singoli campi separati da spazio, 
# puoi usare un ciclo while con il comando read in questa forma:
# while read -r nome cognome voto; do
#     ...
# done < "$FILE_DATI"
#
# Ricorda che in Bash, per confrontare due numeri interi, si usano gli operatori 
# come -ge (greater or equal) e -lt (less than).
# Esempio: [ "$a" -ge "$b" ]

# SETUP DELL'AMBIENTE DI TEST
# Creazione del file di input con i dati degli studenti
cat > studenti.txt << EOF
Mario Rossi 24
Luigi Bianchi 15
Anna Verdi 30
Giulia Neri 17
Paolo Gialli 18
EOF

# Per testare lo script, dovresti lanciarlo con: ./script.sh studenti.txt 18
FILE_DATI=\$1
SOGLIA=\$2

# YOUR CODE HERE

if [[ $# -ne 2 || ! -f "$1" ]]; then
    exit 1
fi

> ammessi.txt
> respinti.txt

while read -r nome cognome voto; do
    if [[ "$voto" -ge "$2" ]]; then
        echo "$nome $cognome" >> ammessi.txt
    else
        echo "$nome $cognome" >> respinti.txt
    fi
done < studenti.txt