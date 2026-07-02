#!/bin/bash

# Si scriva uno script di shell che a seguito di due messaggi
# legga da tastiera due valori interi e ne visualizzi la somma
# e il prodotto
#
# Si faccia uso del comando read <var> per la lettura dei valori

REGEX='^-?[0-9]+$'

while true; do
    read -p "Inserire il primo valore intero: " num1

    if [[ $num1 =~ $REGEX ]]; then
        break
    else
        echo "ERROR"
    fi
done

while true; do
    read -p "Inserire il secondo valore intero: " num2

    if [[ $num2 =~ $REGEX ]]; then
        break
    else
        echo "ERROR"
    fi
done

SOMMA=$((num1 + num2))
PRODOTTO=$((num1 * num2))

echo "$SOMMA, $PRODOTTO"

exit 0