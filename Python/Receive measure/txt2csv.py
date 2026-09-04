import pandas as pd
import serial

# Configura la porta seriale
print('Scrivi il nome del file .txt da trasformare')
alpha = input();
input_file = alpha + ".txt"  # Nome del file di testo di input
print('Scrivi il nome del file .csv dove salvare il risultato')
beta = input();
output_file = beta + f".csv"  # Nome del file CSV di output
# Leggi il file di testo utilizzando Pandas e specifica lo spazio come delimitatore
data = pd.read_csv(input_file, sep=' ', header=None)

# Scrivi i dati nel file CSV utilizzando Pandas
data.to_csv(output_file, index=False)