#!/bin/bash

# Si scriva uno script che legga da tastiera il nome di un utente
# e visualizzi quanti login ha effettuato
#
# Si usi il comando who per ottenere la lista degli utenti ed
# il comando grep <stringa> per cercare stringa in un testo
# e wc -l per contare le linee.

read -p "Inserire nome utente: " username
n_login=$(who | grep -w "$username" | wc -l)
echo "Numero di login effettuati dall'utente: $n_login"