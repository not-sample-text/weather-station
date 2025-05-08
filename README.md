# Stație Meteo cu Arduino

## Descriere Proiect

Acest proiect reprezintă o stație meteo compactă bazată pe Arduino, care măsoară temperatura, presiunea atmosferică și altitudinea folosind senzorul BMP280. Datele sunt afișate pe un ecran TFT ILI9341 color, inclusiv grafice pentru istoricul valorilor măsurate.

Stația meteo este dezvoltată de Pal Antonio și Hausi Alexandru ca parte a unui proiect educațional pentru monitorizarea parametrilor atmosferici.

## Caracteristici

- Măsurarea în timp real a temperaturii (°C)
- Măsurarea presiunii atmosferice (hPa)
- Calcularea altitudinii bazată pe presiunea la nivelul mării
- Afișare grafică a istoricului temperaturii și presiunii
- Interfață vizuală intuitivă cu coduri de culori
- Actualizare automată a datelor la fiecare 2.5 secunde

## Schema Electronică

Acest proiect include o schemă electronică completă creată în KiCad, disponibilă în directorul `/schematics`. Schema detaliază toate conexiunile necesare între Arduino, senzorul BMP280 (conectat prin adaptorul de nivel logic TXS0108E) și ecranul TFT ILI9341.

## Structura Codului

Codul sursă este organizat în mai multe fișiere pentru o mai bună modularitate:

- **DisplayManager**: Gestionează afișajul TFT și toate elementele grafice
- **SensorManager**: Gestionează comunicarea cu senzorul BMP280 și procesarea datelor
- **main.ino**: Fișierul principal care coordonează toate componentele

## Dependențe

Acest proiect necesită următoarele biblioteci:

- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library) (v1.11.5+)
- [Adafruit ILI9341 Library](https://github.com/adafruit/Adafruit_ILI9341) (v1.5.12+)
- [Adafruit BMP280 Library](https://github.com/adafruit/Adafruit_BMP280_Library) (v2.6.6+)
- [Adafruit Sensor Library](https://github.com/adafruit/Adafruit_Sensor) (v1.1.5+)
- Arduino Wire Library (inclusă în Arduino IDE)
- Arduino SPI Library (inclusă în Arduino IDE)

### Instalarea Bibliotecilor

1. Deschideți Arduino IDE
2. Mergeți la Sketch > Include Library > Manage Libraries...
3. Căutați și instalați fiecare dintre bibliotecile Adafruit menționate mai sus
4. Reporniți Arduino IDE
5. Deschideți proiectul și încărcați-l pe placa dvs. Arduino

## Componente Hardware Necesare

- Arduino (recomandat Arduino Uno/Nano/Mega)
- Senzor BMP280 de temperatură și presiune
- Ecran TFT ILI9341 (240x320 pixeli)
- Adaptor de nivel logic TXS0108E (pentru comunicarea cu senzorul BMP280)
- Cabluri de conexiune
- Sursă de alimentare corespunzătoare

## Instrucțiuni de Utilizare

1. Conectați toate componentele conform schemei KiCad din directorul `/schematics`
2. Instalați toate bibliotecile necesare
3. Încărcați codul pe placa Arduino
4. Stația meteo va începe să afișeze date imediat după pornire

## Contribuții

Contribuțiile sunt binevenite! Vă rugăm să citiți ghidul de contribuție pentru mai multe detalii.

## Licență

Acest proiect este distribuit sub licența MIT. Consultați fișierul `LICENSE` pentru detalii complete.

## Contact

Pentru întrebări sau sugestii, vă rugăm să deschideți un issue în acest repository sau să contactați autorii:

- Pal Antonio
- Hausi Alexandru
