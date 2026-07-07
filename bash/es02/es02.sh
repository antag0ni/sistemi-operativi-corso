#!/bin/bash

# Progettare uno script che prende un parametro che indica il nome di una directory e sia in grado
# di esaminare tutti gli elementi della directory, controllando quanti file eseguibili e non eseguibili
# sono presenti nella cartella specificata da riga di comando.
#
# Le liste dei file devono poi essere salvate in due file di testo separati.
#
# Si controlli che il parametro passato sulla linea di comando sia uno e che sia il nome di una
# directory.
#
# SE SI NON FUNZIONA, DEFINIRE UNA VARIABILE CON IL NOME DELLA DIRECTORY
#
# ESEMPIO DI UTILIZZO:
# ./check_executable_files.sh myfiles
#
# ESEMPIO DI OUTPUT:
# Analisi completata per la directory: myfiles
# File eseguibili trovati: 2 (salvati in eseguibili.txt)
# File non eseguibili trovati: 1 (salvati in non_eseguibili.txt)
#
# NOTA BENE
# È possibile consultare le pagine di manuale di Linux (quelle visualizzate dal comando man)
# accedendo alla pagina:
#   http://man.he.net/

# ============================================================
# AMBIENTE DI TEST (setup, non fa parte della soluzione)
# Crea una directory di prova "myfiles" con un mix di file
# eseguibili e non eseguibili, per poter testare lo script
# check_executable_files.sh una volta scritto.
# ============================================================

setup_test_env() {
    TEST_DIR="myfiles"

    # Rimuove eventuale ambiente di test precedente
    rm -rf "$TEST_DIR"
    mkdir -p "$TEST_DIR"

    # File non eseguibili
    echo "contenuto file 1" > "$TEST_DIR/documento1.txt"
    echo "contenuto file 2" > "$TEST_DIR/appunti.md"
    echo "contenuto file 3" > "$TEST_DIR/dati.csv"

    # File eseguibili
    echo "#!/bin/bash
echo 'sono uno script eseguibile'" > "$TEST_DIR/avvio.sh"
    chmod +x "$TEST_DIR/avvio.sh"

    echo "#!/bin/bash
echo 'altro script eseguibile'" > "$TEST_DIR/backup.sh"
    chmod +x "$TEST_DIR/backup.sh"

    # Una sottodirectory, per verificare che venga gestita correttamente
    mkdir -p "$TEST_DIR/sottocartella"

    echo "Ambiente di test creato in '$TEST_DIR':"
    ls -l "$TEST_DIR"
}

# Decommentare la riga seguente per generare l'ambiente di test
#setup_test_env

if [[ $# -ne 1 || ! -d "$1" ]]; then
    echo "provide directory"
    exit 1;
fi

> eseguibili.txt
> non_eseguibili.txt

DIR="$1"

for f in "$DIR"/*; do
    if [[ -x "$f" && -f "$f" ]]; then
        echo "$(basename "$f")" >> eseguibili.txt
    elif [[ -f "$f" ]]; then
        echo "$(basename "$f")" >> non_eseguibili.txt
    fi
done

echo "Analisi completata per la directory: $DIR"
echo "File eseguibili trovati: $(wc -l < eseguibili.txt) (salvati in eseguibili.txt)"
echo "File non eseguibili trovati: $(wc -l < non_eseguibili.txt) (salvati in non_eseguibili.txt)"