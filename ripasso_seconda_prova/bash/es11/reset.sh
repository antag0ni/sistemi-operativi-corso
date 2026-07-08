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

echo ">>> Inizializzazione ambiente di test..."

# Definizione del nome della directory di test
TEST_IN_DIR="directory_input_test"

# Creazione della directory (se non esiste già)
mkdir -p "$TEST_IN_DIR"

# Creazione di 3 file di log fittizi, ognuno contenente diversi score
echo -e "Accuracy 0.88\nF1 0.85\nRecall 0.82" > "$TEST_IN_DIR/modello1_log.txt"
echo -e "Accuracy 0.92\nF1 0.90\nRecall 0.89" > "$TEST_IN_DIR/modello2_log.txt"
echo -e "Accuracy 0.75\nF1 0.78\nRecall 0.80" > "$TEST_IN_DIR/modello3_log.txt"
echo -e "Accuracy 0.80\nF1 0.81\nRecall 0.79" > "$TEST_IN_DIR/modello4_log.txt"

echo "Ambiente di test creato con successo nella cartella: '$TEST_IN_DIR'"
echo "Sono stati generati $(ls -1q "$TEST_IN_DIR" | wc -l) file di test."
echo "Valori di F1 inseriti: 0.85, 0.90, 0.78, 0.81"

