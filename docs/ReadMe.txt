A Funktion und Hinweise
  Stand: 03.03.2020
  Dies ist eine kleine OriginC-Bibliothek mit Skripten für Import und Verarbeitung spektroskopischer Messdaten.
  Zudem sind ein paar der simpelsten Analyse- und Auswertungsfunktionen implementiert.
  Für Funktion und Richtigkeit wird keine Gewähr übernommen, von daher bitte immer alle Ergebnisse kurz gegenchecken :)
  Alle Funktionen wurden in OriginPro 2018-2020 getestet, ältere Versionen sollten eh geupdated werden...

B Installation
  1.  Kopiert den Ordner "OriginC" (derzeit in "\wet-daten\2_Allgemein Lehrstuhl\Skripte\" auf euren Rechner.
  2.  Startet OriginPro und öffnet den Code Builder ("Ansicht" -> "Code Builder"; Shortcut: "Alt + 4")
  3.  Klickt mit der rechten Maustaste auf den Ordner "User [AutoLoad]" und wählt "Dateien hinzufügen ..." aus.
  4.  Navigiert zu eurer lokalen Kopie von "OriginC" und wählt dort die Datei "main.c" aus.
  5.  Wählt in der Menüleiste "Alles neu erstellen" aus (Shortcut: "Alt+ F8").
  6.  Zeigt das Ausgabefenster (unten links) einen Fehler an, wendet euch bitte an den Kummerkasten bei Fr. Merker.
  7.  Klickt mit der rechten Maustaste auf "Origin C Workspace" und deaktiviert die Option "Build on Startup".
  8.  Alle Skripte sind nun bei jedem Start von OriginPro verfügbar, der Code Builder kann geschlossen werden.

C Verwendung
  1.  Öffnet das Skiptfenster ("Fenster" -> "Skriptfenster"; Shortcut: "Alt + Shift + 3").
  2.  Gebt den Namen eines Skriptes ein (siehe "D") und bestätigt mit "Enter".
  (Jedes "Enter" im Skriptfenster wird als Befehl interpretiert. Steht in der Zeile kein Befehl, folgt eine Fehlermeldung ...)

D Aktuell verfügbare Funktionen
  Alle Hauptfunktionen können in der Datei "main.c" eingesehen werden und benötigen keine Parameter.
  Die Subfunktionen können in den Header-Dateien "*.h" eingesehen und prinzipiell ebenfalls direkt ausgeführt werden.
  Hierbei ist jedoch auf die bennötigten Parameter zu achten.
  1.  "import" <- automatisiert den Datenimport
        a  Parameter
           "Target Workbook":    Name der Arbeitsmappe, in die importiert werden soll (wird ggf. neu erstellt)
           "Target Worksheet":    Name des Arbeitsblattes, in das importiert werden soll (wird ggf. neu erstellt)
           "Data Type":          der Importmodus (siehe "b Datentypen")
           "Delimiter":          das Trennzeichen zwischen Datenpunkten (meist "Tab", manchmal "Whitespace")
           "Dec. Separator":      das Dezimaltrennzeichen (meist ".", beim UvVis ",")
           "Constant X-Axes":    nur für Import mehrerer Spektren; entfernt alle X-Achsen außer der ersten
           "Create Sparklines?":  erstellt Sparkllines für alle importierten Datenspalten (Achtung! Kann sehr lange dauern!)
        b  Datentypen
           "Spectra Files":  z. B. LabSpec- oder UVProbe-Spektren, die Dateinamen stehen in den Spaltenkommentaren
           "3D-Maps":        z. B. NTMDT-Auswertungen oder LabSpec-Maps (XY-I), die Dateinamen werden zu den Blattnamen
           "4D-Maps":        z. B. NT-MDT-Rohdaten (XY-l-I), die Dateinamen werden zu den Blattnamen
                             NT-MDT-Rohdaten müssen in "Image Analysis" als Matlab-Datei ("*.m"!!!) exportiert werden.
           "Tracks"          XML-Dateien mit Tracking-Informationen aus Analysen mit dem ImageJ TrackMate
           Die Meta-Daten aus Maps (z. B. Zeit-Achse und XY-Koordinaten werden automatisch als User-Parameter angelegt).
  2.  "correct" <- manipuliert die Daten in der aktuellen Arbeitsmappe/Arbeitsblatt
        a  Parameter (je nach Methode)
           "Data Source":  Auswahlfeld der Datenquelle (Arbeitsblatt)
           "Worksheet":    Auswahlfeld der Referenzquelle (Arbeitsblatt, nicht bei jeder Methode)
           "Parameter":    Auswahlfeld des verbindenden Nutzerparameters (muss in Quelle und Referenz vorhanden sein, nicht bei jeder Methode)
        b  Methoden (Auswahl mittels Checkboxen in Dialog)
           "Background Subtraction":  a)  "Reference" Mode: Subtrahiert die zugehörigen Hintergrundsprekten von den Daten (Parameter: "Worksheet", "Parameter").
                                          Subtraktion von ReferenzdatenDie Label-Daten in beiden Arbeitsblättern müssen exakt übereinstimmen!)
                                      b)  "Average" Mode: Subtrahiert den Mittelwert eines Teilspektrums von den Daten (Parameter: "Start", "Stop").
           "Spike Removal":           Entfernt automatisiert kosmische Spikes aus allen Spektren (Parameter: "Z-Threshold", "Averaging Width").
                                      (Basiert auf dem Whitaker-Hayer-Algorithmus, 10.1016/j.chemolab.2018.06.009. Vorsicht mit dem Threshold!)
           "Setup Correction":        Korrigiert die Daten mit der Kalibrationskurve des Messplatzes mittels Division (Parameter: "Worksheet").
                                      (Es werden die ersten beiden Spalten in "Worksheet" als XY-Datensatz ausgewertet. Verscheidene Korrekturkurven werden nicht unterstützt!)
           "Filter Correction":       Korrigiert die Daten mit den Transmissionsspektren von Detektionsfiltern (Parameter: "Worksheet", "Parameter").
                                      (Die Transmissionsdaten müssen absolut vorliegen, %-Werte werden nicht unterstützt.)
           "Integration Time":        Dividiert alle Daten durch ihre Integrationszeit (Parameter: "Parameter").
                                      (Das Label -muss- ausschließlich numerische Daten enthalten!)
           "Energy Transformation":   Führt für alle Daten die Jacobische Transformation in den Energieraum durch (Parameter: keine).
                                      (Achtung! Nicht für z.B. PLE und Absorptionsspektren geeignet!)
           "Data Normalisation":      Normalisiert alle Daten durch Division durch das Maximum jedes Datensatzes (Parameter: keine).
  3.  "analyse" <- führt simple Analysemethoden an der aktuellen Mappe/Blatt durch
        a  Parameter
           "Method":        Auswahlfeld der Auswertungsmethode
           "Source Type":   Auswahlfeld der Datenquelle (Arbeitsmappe oder Arbeitsblatt)
        "Source Type"
        b  Methoden
           "Spectra":     Analyse aller Y-Datenspalten (z. B. Peak Position, Fläche, FWHM, etc ...)
           "4D-Linescan": Extrahiert aus einer importierten 4D-Map (NT-MDT) einen Linescan entlang einer Achse (der Parameter "width" ist noch nicht implementiert).
  4.  "convert" <- wandelt die Daten des aktuellen Blattes in eine andere Darstellung um (z. B. XYZ-Spalten in 3D-Map)
        a  Parameter
           "Method":  Auswahlfeld der Konvertierungsmethode
        b  Methoden
           "XYZ-Data to Matrix":  wandelt eine Datenspalte in eine XYZ-Map um (XY kann manuell eingegeben oder ausgelesen werden)
  5.  "alignText" <- richtet den Text eines Textfeldes in einem Graphen (!) aus
        a  Parameter
           "Text object":  Auswahlfeld für das auszurichtende Textelement (XB = X-Bottom, YL = Y-Left, ...)
           "Alignment":    Auswahlfeld für die gewünschte Textausrichtung (wie in Word)
  6.  "renameWbs" <- benennt die Kurznamen aller Arbeitsmappen im aktiven Ordner um (generiert aus Langnamen)
        a  Parameter
           keine
  7.  "setLowerBound XXX" <- ersetzt alle Werte im aktiven Datenblatt, die kleiner als "XXX" sind, durch "0"
        a  Parameter
           "XXX":  Die Untergrenze für zu ersetzende Werte ("0", wenn nicht angegeben)
  8.  "reduce" <- Reduziert die CPU-Auslastung, indem alle Sparklines gelöscht (!) und alle Fenster versteckt werden.
        a  Parameter
           keine