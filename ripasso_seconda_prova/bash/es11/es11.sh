# Scrivere uno script bash che effettui una ricerca di uno specifico score (ad es. F1) in
# ciascuno dei file salvando i singoli valori in file di uscita e ne calcoli la media,
# mostrandola a video.
#
# Lo script deve ricevere in ingresso il path della directory principale dove risiedono i
# file, il path della directory di uscita (se non esiste la crea), il nome del file di uscita e
# lo score su cui calcolare la media.
#
# Si usi il comando awk '{print $i}' che consente di stampare l'i-ma stringa di ogni riga
# di un file.

if [[ $# -ne 4 || ! -d "$1" ]]; then
    echo "Usage: $0 dir out_dir out_file score"
    exit 1
fi


DIR="$1"
O_DIR="$2"
O_FILE="$O_DIR/$3.txt"
SCORE="$4"

if [[ ! -d "$O_DIR" ]]; then
    mkdir "$O_DIR"
fi

> "$O_FILE"

for f in "$DIR"/*; do
    if [[ -f "$f" ]]; then
        var="$(grep "$SCORE" "$f" | awk '{print $2}')"
        echo "$var" >> "$O_FILE"
    fi
done

avg=$(awk '{sum+=$1} END {print sum/NR}' "$O_FILE")

echo "avg: $avg"

exit 0