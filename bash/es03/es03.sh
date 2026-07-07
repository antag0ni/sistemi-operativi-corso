#!/bin/bash

# ==============================================================================
# TRACCIA D\'ESAME: FILTRO E CONTEGGIO FILE PER DIMENSIONE
# ==============================================================================
#
# Progettare uno script Bash che prende esattamente DUE parametri sulla linea 
# di comando:
# 1. Il nome di una directory da esaminare.
# 2. Un numero intero che rappresenta una dimensione soglia in BYTE (es. 5000).
#
# Lo script deve eseguire i seguenti controlli iniziali:
# - Verificare che siano stati passati esattamente due parametri.
# - Verificare che il primo parametro sia una directory esistente. 
#   Si usi la variabile $DIR per questo controllo.
#
# Se i controlli vanno a buon fine, lo script deve esaminare tutti gli elementi 
# presenti nella directory specificata (senza entrare nelle sotto-directory) 
# e dividere solo i FILE REGOLARI in due categorie:
#
# 1. Se la dimensione del file in byte è MAGGIORE O UGUALE alla soglia 
#    specificata (secondo parametro), il nome del file deve essere salvato 
#    in un file denominato "file_grandi.txt".
# 2. Se la dimensione del file è MINORE della soglia, il nome del file deve 
#    essere salvato in un file denominato "file_piccoli.txt".
#
# Al termine dell\'analisi, lo script deve stampare a schermo un riepilogo con 
# il numero totale di file trovati per ciascuna categoria.
#
# ------------------------------------------------------------------------------
# SUGGERIMENTI
# ------------------------------------------------------------------------------
# - Per ripulire o creare i file di output all\'inizio dello script puoi usare 
#   l\'operatore >.
# - Per ottenere al volo la dimensione in BYTE di un file dentro al ciclo, 
#   puoi usare questo comando:
#   dimensione=$(wc -c < "$elemento")
#
# ------------------------------------------------------------------------------
# ESEMPIO DI UTILIZZO:
# ./filtra_dimensioni.sh myfiles 4096
#
# ESEMPIO DI OUTPUT A SCHERMO:
# Analisi completata per la directory: myfiles
# File grandi (>= 4096 byte) trovati: 3 (salvati in file_grandi.txt)
# File piccoli (< 4096 byte) trovati: 2 (salvati in file_piccoli.txt)
# ==============================================================================

# SETUP DELL'AMBIENTE DI TEST (Opzionale: scommenta le righe sotto per creare i file di test)
# mkdir -p myfiles
# echo "Piccolo" > myfiles/file1.txt
# dd if=/dev/zero of=myfiles/file_grande.dat bs=1024 count=5 2>/dev/null # Crea un file di 5120 Byte

# YOUR CODE HERE

if [[ $# -ne 2 || ! -d "$1" ]]; then
    exit 1
fi

> file_grandi.txt
> file_piccoli.txt

DIR="$1"
soglia="$2"

for f in "$DIR"/*; do
    if [[ -f "$f" && $(wc -c < "$f") -ge "$soglia" ]]; then
        echo "$(basename "$f")" >> file_grandi.txt
    elif [[ -f "$f" && $(wc -c < "$f") -lt "$soglia" ]]; then
        echo "$(basename "$f")" >> file_piccoli.txt
    fi
done

echo "Analisi completata per la directory: myfiles"
echo "File grandi (>= $soglia byte) trovati: $(wc -l < file_grandi.txt) (salvati in file_grandi.txt)"
echo "File piccoli (< $soglia byte) trovati: $(wc -l < file_piccoli.txt) (salvati in file_piccoli.txt)"