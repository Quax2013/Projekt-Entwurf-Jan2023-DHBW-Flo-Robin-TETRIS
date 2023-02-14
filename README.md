# Projekt-Entwurf-Jan2023-DHBW

## Zusammenfassung
Tetris ist ein Spiel entwickelt 1984 von Alexei Paschitnow und veröffentlicht von Nintendo.
Im Spiel fallen 7 verschieden Blöcke von oben nach unten und das Ziel ist es möglichst viele Punkte durch Füllen ganzer Reihen mit den Blöcken zu erreichen. Dabei gibt es Bonuspunkte für diverse aktionen, wie z.B. das vervollständigen mehrerer Reihen mit einem Block und anderem.

## Steuerung
Die Steuerung ist W um den Block zu drehen, A und D um ihn nach links und rechts zu bewegen und S um ihn für weitere Extrapunkte schneller nach unten zu bewegen.
Zusätzlich enthält unsere Version eine Funktion mit der man mit F das Spiel beliebig beenden kann.

## Entwicklung 
Um das Programm zu Compiliernen brauch man einen C-Compiler (z.B. MinGW).
Dann kann man das Programm mit 
    gcc dateiName.c -o dateiName
compilieren.
Das Programm besteht aus einem globalen, 2-dimensionalen Array, in welchem alle Daten gespeichert werden (Spielfeld, Blöcke, ...). Die verschiedenen Funktionen bearbeiten das Array.
