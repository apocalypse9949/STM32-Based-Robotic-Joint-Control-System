import serial

# Configura la porta seriale
ser = serial.Serial('COM9', baudrate=115200)

# Apri il file in modalità scrittura
with open('output_offset_fixed35.txt', 'w') as file:
    # Loop principale
    while True:
        # Legge i dati dalla porta seriale
        data = ser.readline().decode().strip()
        data = data.strip('\x00').strip()

        # Stampa i dati

        print(data)
        # Scrivi i dati nel file
        file.write(data + '\n')

# Chiude la porta seriale
ser.close()
