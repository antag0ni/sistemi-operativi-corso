#!/bin/bash

# ==============================================================================
# ESERCIZIO 03 - Scripting Bash
# 
# Progettare uno script che accetti come parametro $DIR il percorso di una directory. 
# Lo script deve:
# 1. Verificare che sia stato passato esattamente un parametro e che sia una directory valida.
# 2. Creare una sottocartella chiamata `backup_txt` all'interno di $DIR.
# 3. Analizzare ogni elemento presente nella directory principale $DIR.
# 4. Se l'elemento è un file regolare, termina con `.txt` e ha una dimensione MAGGIORE di zero (non è vuoto):
#    - Copiare il file nella cartella `backup_txt`.
# 5. Alla fine, stampare a video il messaggio: 
#    "Backup completato: N file copiati", dove N è il numero di file
#    effettivamente presenti nella cartella `backup_txt`.
# 
# Suggerimenti:
# - Il test `[ -s NOMEFILE ]` verifica se un file esiste e ha una dimensione > 0.
# - Per contare i file in una cartella puoi usare `ls CARTELLA | wc -l`.
# ==============================================================================

# Setup ambiente di test
mkdir -p test_backup
echo "Contenuto importante" > test_backup/doc1.txt
touch test_backup/vuoto.txt  # File vuoto, dimensione 0
echo "Altro testo" > test_backup/doc2.txt
echo "Questo è un log" > test_backup/sys.log
mkdir -p test_backup/sub_dir

DIR="test_backup"

# INSERIRE IL CODICE QUI

if [[ $# -ne 1 || ! -d "$1" ]]; then
    echo "provide directory"
    exit 1
fi

DIR="$1"
cd "$DIR"

mkdir backup_txt > /dev/null 2>&1 

for f in *; do
    if [[ -s "$f" && -f "$f" && "$f" == *.txt ]]; then
        cp "$f" ./backup_txt
    fi
done

echo "Backup completato: $(ls backup_txt | wc -l ) file copiati" 

exit 0