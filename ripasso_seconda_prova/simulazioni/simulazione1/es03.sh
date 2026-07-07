#!/bin/bash
#
# Progettare uno script che prende un parametro che indica il nome di una directory e sia in grado di esaminare tutti gli
# elementi della directory, senza entrare nelle eventuali sotto-directory; lo script, per ciascun elemento trovato nella
# directory, deve scrivere in un file denominato "output.txt" (nella directory corrente), un "sommario" dell'elemento secondo
# le seguenti modalità:
#
# 1. se l'elemento è un file, lo script deve riportare nel sommario il nome del file e la sua prima riga
# 2. se l'elemento è una sottodirectory, lo script deve riportare nel sommario il nome della sottodirectory e il numero di file
#    in essa contenuti.
#
# Si controlli che il parametro passato sulla linea di comando sia uno e che sia una directory. SI USI la variabile $DIR per il
# controllo della directory.
#
# SUGGERIMENTO
#
# - Per stampare la prima riga di un file, si può usare il comando head -n 1 NOMEFILE
# - Per ottenere il numero di elementi di una directory si può usare il comando ls (che scrive in output i contenuti di una
#   directory) in combinazione con il comando wc -l (che scrive in output il numero di linee del suo input).
#
# NOTA BENE È possibile consultare le pagine di manuale di Linux (quelle visualizzate dal comando man) accedendo alla
# pagina: http://man.he.net/

mkdir myfiles
mkdir myfiles/myfiles2
cat > myfiles/input.dat << EOF
0 12
12 0
2 1
EOF

cat > myfiles/file1.txt << EOF
0 12
44 33
2 1
EOF

cat > myfiles/myfiles2/file2.txt << EOF
22 11
12 0
2 1
EOF

DIR="$1"

# YOUR CODE HERE

if [[ $# -ne 1 || ! -d "$DIR" ]]; then
	echo "fornire una directory"
	exit 1
fi

> output.txt

for f in "$DIR"/*; do
	if [[ -f "$f" ]]; then
		echo "Nome file: $f" >> output.txt
		echo "Prima riga di $f: $(cat "$f" | head -n 1)" >> output.txt
	elif [[ -d "$f" ]]; then
		echo "$f è una directory" >> output.txt
		echo "Numero di file contenuti in $f: $(ls "$f" | wc -l)" >> output.txt
	fi
done

exit 0