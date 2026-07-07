#!/bin/bash

# ==============================================================================
# ESERCIZIO 03 - Scripting Bash (Analisi Log e Permessi)
# 
# Progettare uno script che accetti come parametro $DIR il percorso di una directory. 
# Lo script deve:
# 1. Verificare che sia stato passato esattamente un parametro e che sia una directory valida.
# 2. Creare all'interno di $DIR una sottocartella chiamata `executables` (se non esiste già).
# 3. Analizzare ogni elemento presente nella directory principale $DIR (ignorando le sottodirectory).
# 4. Per ogni file regolare analizzato:
#    - Se il file termina con l'estensione `.log`, contare quante righe contengono la parola "FATAL" 
#      e salvare il risultato in un file `summary.txt` (nella directory in cui si esegue lo script) 
#      nel formato: `NomeFile: X errori critici`.
#    - Se il file ha i permessi di esecuzione (`-x`), copiarlo all'interno della cartella 
#      `executables` creata al punto 2.
# 
# Suggerimenti:
# - Per verificare se un file è eseguibile, puoi usare il test `[ -x NOMEFILE ]`.
# - Per contare le occorrenze di una parola in un file, puoi usare `grep -c "PAROLA" NOMEFILE`.
# - Per verificare se una stringa termina con `.log` in Bash, puoi usare una 
#   struttura `case` o un controllo con `[[ $file == *.log ]]`.
# ==============================================================================


# Setup ambiente di test
mkdir -p test_logs
echo "INFO: Avvio sistema" > test_logs/sys.log
echo -e "FATAL: Kernel panic\nFATAL: Memory leak" > test_logs/app.log
echo "Tutto ok" > test_logs/readme.txt
touch test_logs/script.sh
chmod +x test_logs/script.sh

DIR="test_logs"

# INSERIRE IL CODICE QUI

if [[ $# -ne 1 || ! -d "$1" ]]; then
    echo "provide a directory"
    exit 1
fi

> summary.txt
DIR="$1"
cd "$DIR"

if [[ ! -d "executables" ]]; then
    mkdir executables
else
    echo "executables already exist"
fi

for f in *; do
    if [[ -f "$f" ]]; then
        if [[ -x "$f" ]]; then
            cp "$f" ./executables > /dev/null 2>&1
            echo "$f executables copied in executables folder"
        fi
        if [[ "$f" == *.log ]]; then
            echo "$f: $(grep -c FATAL "$f") errori critici" >> ../summary.txt
        fi
    fi
done

cd ..
cat summary.txt

exit 0