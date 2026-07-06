#!/bin/bash
#
# reset_test.sh
#
# Riporta l'ambiente di test allo stato iniziale, cancellando
# qualsiasi modifica fatta dal tuo script (rinomine, cancellazioni,
# nuovi file creati per errore, ecc.).
#
# Puoi lanciarlo tutte le volte che vuoi: ricrea sempre la stessa
# struttura da zero.
#
# Uso: ./reset_test.sh [directory_destinazione]
#      (default: ./test_rename)

DEST="${1:-test_rename}"

# Cancella completamente la cartella (se esiste) e la ricrea da zero
rm -rf "$DEST"
mkdir -p "$DEST/SottoDir_Maiuscola" "$DEST/sottodir_minuscola"

cd "$DEST" || exit 1

# --- File regolari con caratteri maiuscoli nel nome (devono essere rinominati) ---
touch "Pippo.txt"
touch "PROVA.C"
touch "MixedCase_File.LOG"
touch "UnFile Con Spazi.TXT"

# --- File regolari già tutti minuscoli (non devono cambiare) ---
touch "topolino.txt"
touch "minnie.c"

# --- File dentro le sottodirectory ---
touch "SottoDir_Maiuscola/Interno_Maiuscolo.txt"
touch "sottodir_minuscola/altro_minuscolo.txt"

cd - >/dev/null

echo "Ambiente ripristinato in: $DEST"
echo
find "$DEST"
