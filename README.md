# par-achoo-t
Abschlussprojekt für die Informatikwerkstatt WS 2020.

Dieses Projekt entsteht als Teilnehmerprojekt im Zuge der Informatikwerkstatt der [TU Clausthal](https://www.tu-clausthal.de/).
## Vorraussetzungen
Das Projekt verwendet eine angepasste Version der von [Atmel START](https://start.atmel.com) generierten Makefile, um ein einfaches Verwalten von Haupt- und Testprogramm zu ermöglichen, wobei letzteres auf Quelldateien im Verzeichnis des Hauptprogramms zugreifen kann, damit keine doppelten Dateien entstehen.

Um das Projekt zu kompilieren, wird `make` benötigt, welches unter anderem in [Atmel/Microchip Studio](https://www.microchip.com/en-us/development-tools-tools-and-software/microchip-studio-for-avr-and-sam-devices) enthalten ist. Des Weiteren werden auch der Compiler `avr-gcc` und `avr-objcopy` vorrausgesetzt, diese sind ebenfalls in zuvor genanntem Softwarepaket enthalten.
## Kompilieren
Folgende Befehle müssen alle im Verzeichnis `gcc` ausgeführt werden, damit sie korrekt funktionieren.
### Hauptprogramm
Um eine Abfolge von Befehlen, die in der `Makefile` aufgelistet sind, ausführen zu lassen, die das Programm kompilieren, kann der Befehl
```
$ make all
```
ausgeführt werden. Nach Abschluss kann die Datei `AtmelStart.hex` dann wiederum auf den µController geladen werden. Dazu kann `avrdude` verwendet werden, was zum Beispiel in der [Arduino IDE](https://www.arduino.cc/en/software) mitgeliefert wird.
### Testprogramm
Zudem kann auch nur der Teil des Programms, um zum Beispiel Treiber zu testen mit 
```
$ make test
```
kompiliert werden. Hier wird dann die Datei `AtmelTest.hex` erstellt, die wie zuvor auch auf den µController geladen werden kann.
### Staubwischen
Die generierten Dateien können im Anschluss mit
```
$ make clean
```
wieder entfernt werden.
