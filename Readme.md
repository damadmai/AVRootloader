# AVRootloader Version 6.0 by HR

Diese Datei beschreibt, wie man den Bootloader konfiguriert und benutzt.

1. `.zip` entpacken. Auf meinem Rechner habe ich alles im Ordner  
`C:\Programme\Atmel\AVRootloader\` installiert
2. [AVR-Studio 4.x](https://www.mikrocontroller.net/articles/Atmel_Studio) und AvrAssembler2 werden ben�tigt
3. AVRStudio starten und Projekt [AVRootloader.aps](./AVRootloader.aps) �ffnen
4. in [AVRootloader.asm](./AVRootloader.asm) m�ssen einige Konfigurationen vorgenommen werden:
    1. korrektes `.include` zum [AVR](https://www.mikrocontroller.net/articles/AVR) ausw�hlen, einfach `;` entfernen,  
    ACHTUNG nur eines der `.include` darf aktiv sein
    2. einige Defines m�ssen/k�nnen eingestellt werden:
        1. `UseWDR = 1`         aktiviert die Unterst�tzung des WatchDog Timers
        2. `UseAutobaud = 1`    aktiviert die automatische Baudraten Erkennung
        3. `UseVerify = 1`      aktiviert die separate �berpr�fung des FLASHs, deaktiviert ist empfohlen da das Schreiben/L�schen des FLASHs und Schreiben des EEPROMs schon eine implizite �berpr�fung drinnen haben, zudem ist die Daten�bertragung immer CRC abgesichert
        4. `UseE2Write = 1`     aktiviert den EEPROM Schreiben Befehl
        5. `UseE2Read = 1`      aktiviert den EEPROM Lesen Befehl
        6. `UseSRAM = 1`        aktiviert den SRAM Lesen/Schreiben Befehl, nicht empfohlen, wenn Verschl�sselung benutzt wird, da dies dann unsicher ist
        7. `UseCrypt = 1`       aktiviert die [XTEA](https://de.wikipedia.org/wiki/Extended_Tiny_Encryption_Algorithm) Entschl�sselung im AVR,  
        man kann also verschl�sselte Daten f�r das FLASH/EEPROM senden
        8. `UseCryptFLASH = 1`  Daten, die ins FLASH geschrieben werden, m�ssen immer verschl�sselt sein. Sollte `UseCryptFLASH = 0` sein, so kann man Daten ins FLASH mit oder ohne Verschl�sselung schreiben, nat�rlich ist `UseCryptFLASH=0` dann unsicher, da ein Angreifer auf diese Weise ein Spionageprogram installieren k�nnte, das den Bootloader und somit das Passwort ausliest
        9. `UseCryptEEPROM = 1` Hier gilt das Gleiche wie bei `UseCryptFLASH`,  
        nur eben f�r das EEPROM Schreiben
        10. `UartInvert = 0`     invertiert die UART Pegel
        11. `RX_PORT/RX`         den RX Empfangsport und Pin einstellen
        12. `TX_PORT/TX`         den TX Sendeport und Pin einstellen. Wenn beide gleich sind, wird der 1-Wire-Modus benutzt, sind sie unterschiedlich, der 2-Wire-Modus.  
        M�chte man z.B. RS-232 mit MAX232 Treiber benutzen, so stellt man separate Pins f�r RX und TX ein und setzt `UartInvert=1`
        13. `XTAL`               auf Taktfrequenz in Hz des AVRs setzen,  
        sollte `UseAutobaud=0` sein so ist dieser Wert wichtig
        14. `BootDelay`          die Wartezeit bis der Bootloader die App anspringt,  
        wenn gleich `XTAL` eine Sekunde, ansonsten Bruchteil von `XTAL`
        15. `BootBaudrate`       fixe Baudrate falls `UseAutobaud=0` ist
        16. `BootVersion = 6`    Bootloaderversion, nicht ver�ndern
        17. `BootCodeSize = x`   **Zuerst auf 0 setzen**. Nachdem alles konfiguriert wurde, **kompiliert** ihr mit AVRStudio den Source. Im Messagewindow schaut ihr dann den Wert f�r `[cseg] used` an und **tragt diesen bei `BootCodeSize` ein**. Danach **nochmal komplieren**. Der Wert kann sich nun um 2 Byte gegen�ber zuvor unterscheiden (`avr-size.exe`-Ausgabe)
        18. `BootSign = "BOOT"`  steht am Ende des Source. Ihr k�nnt diesen Wert �ndern, er dient zur Identifizierung eines Verbindungsaufbaus und kann als Zugangspasswort betrachtet werden. Dieser String sollte immer eine *geradzahlige Anzahl an Zeichen* haben. Solltet ihr diesen Wert ab�ndern, so m�sst ihr in der Datei [AVRootloader.ini](./AVRootloader.ini) in Sektion `[System]` den Wert `Sign=BOOT` ebenfalls anpassen.
        19. `BootMsg`            hier k�nnt ihr eine Nachricht eintragen, welche im Page "Device Information" abrufbar ist
        20. `BootInfo`           nicht �ndern wird alles autom. gemacht
        21. `BootKey: = 16 Bytes` Das Passwort f�r die [XTEA](https://de.wikipedia.org/wiki/Extended_Tiny_Encryption_Algorithm) Entschl�sselung und nur aktiv, wenn `UseCrypt=1` ist. Ihr k�nnt mit der PC-Software [AVRootloader.exe](./AVRootloader.exe) diesen Key per Zufall erzeugen lassen. Startet [AVRootloader.exe](./AVRootloader.exe) und dr�ckt "make password". Die Software modifiziert dann den Source in [AVRootloader.asm](./AVRootloader.asm)
5. das `.hex` File auf den AVR flashen
6. das [Testprojekt](../../tree/test) f�r ATMega162, dieses �ffnen und Makefile ab�ndern f�r das Ziel AVR, ebenfalls PORT und PIN an dem der 1-Wire h�ngt anpassen, falls dies in [AVRootloader.asm](./AVRootloader.asm) ge�ndert wurde
7. [AVRootloader.exe](./AVRootloader.exe) starten
    1. in der FLASH ComboBox das `.hex` File ausw�hlen, das man flashen m�chte
    2. COM-Port und Baudrate einstellen, COM-Port kann auch direkt eingegeben werden
    3. auf Button "Program" klicken
    4. AVR reset'en oder Spannung aus/einschalten
    5. AVRootloader sollte jetzt flashen und wenn fertig sofort die Anwendung starten
8. Z.B. beim ATMega128 flashen dauert mit l�schen bei 115200 Baud  
16 Sekunden, mit Verschl�sselung 22 Sekunden.

## Schaltplan f�r 1-Wire

![1-Wire](./1-Wire.png)

## [Testprojekt](../../tree/test)

Wenn ihr das [test.c](../test/test.c) compiliert und geflasht habt, so �berwacht dieses den 1-Wire Pin.  
Sobald also der [AVRootloader.exe](./AVRootloader.exe) versucht eine Verbindung zum Bootloader aufzubauen, wird der IRQ ausgel�st und in den Bootloader gesprungen.  
Ein Dr�cken des RESET oder Spannung aus/einschalten entf�llt, alles geht vollautomatisch.

## Kommandozeilenparameter

Die [AVRootloader.exe](./AVRootloader.exe) kann mit folgenden Parametern aufgerufen werden:

```
AVRootloader.exe -PCOM1 -B115200 -SBOOT -Dc:\programme\atmel\AVRootloader\Test\ -FTest.hex -ETest.eep -K5441C8CA4DDF8EEA19AAAFD877AEB488 -Aepvc

-P COM-Port, oder AUTO f�r Autodetektion z.B. -PCOM1
-B Baudrate, z.B. -B115200
-S BootSign, z.B. -SBOOT
-D Arbeitsverzeichnis in dem das .hex/.eep File liegt
-F .hex File f�r den FLASH oder ein .acy File (precompiliertes und eventuell verschl�sseltes bin�res Script)
-E .eep File f�r das EEPROM
-K HEXadezimales Passwort falls UseCrypt=1 im Bootloader
-V Versionsnummer, z.B. -V010203040C, bedeutet Version 1.2.3.4 in HEX sowie Bitmaske 0C in HEX um nur 1.2. der Versionsnummer zu �berpr�fen
-A Automatikmodus/Befehle f�r den Bootloader
 e erase, l�sche FLASH vor dem Programmieren, wird diese Option nicht angegeben so gelten die Einstellungen in AVRootloader.asm
 p program, beschreibe FLASH/EEPROM mit der Datei die bei -F/-E angegeben wurde
 v verify, �berpr�fe den FLASH Inhalt, wird diese Option nicht angegeben, so gilt das, was in AVRootloader.asm eingestellt wurde
 c close, beende AVRootloader nach den Aktionen, aber nur falls keine Fehler aufgetreten sind
```

Falls mit `-F` eine `.acy`-Datei angegeben wurde, so enth�lt diese sowohl den FLASH als auch den EEPROM Inhalt, wenn das so beim Kompilieren der Datei angegeben wurde. Wurde der Bootloader im AVR mit expliziter Verschl�sselung kompiliert, so erzeugt der PC-Bootloader immer `.acy`-Dateien mit verschl�sselten Daten. Somit kann man beim Endkunden den PC-Bootloader ausliefern ohne ein Passwort mitzugeben. Der Endkunde spielt die verschl�sselte `.acy`-Datei per PC Software auf den AVR, ohne das Passwort zu kennen. ACHTUNG! in diesem Fall sollte nat�rlich in [AVRootloader.ini](./AVRootloader.ini) der Eintrag `Password=XYZ` auch gel�scht werden ;)

Eine schon laufende Instanz von [AVRootloader.asm](./AVRootloader.asm) wird beim zweiten Aufruf die Parameter �bernehmen.
Das hei�t, immer nur eine Instanz von AVRootloader ist aktiv.
Ihr k�nnt also eine Verkn�pfung auf AVRootloader ziehen und darin diese Parameter f�r euer Projekt einstellen.

Sollten einige der Parameter oben nicht in der Kommandozeile definiert sein, so nimmt die PC-Software die letzten gespeicherten Einstellungen.

Wichtige Dateien:
- [AVRootloader.asm](./AVRootloader.asm), Assembler Datei mit dem Bootloader, der auf dem AVR l�uft
- [AVRootloader.inc](./AVRootloader.inc), [Crypt.inc](./Crypt.inc), [Special.inc](./Special.inc), Include zur `.asm`-Datei
- [AVRootloader.exe](./AVRootloader.exe), Windows Programm des Bootloaders
- [AVRootloader.ini](./AVRootloader.ini), Konfigurationsdatei des Programms, speichert alle Einstellungen ab
- [AVRootloader.dev](./AVRootloader.dev), Device Liste, enth�lt alle Informationen zu allen AVRs die es gibt. Diese Liste wird immer ben�tigt, f�r ein automatisches Erzeugen dieser Datei sollte sie gel�scht werden, wenn auf dem Rechner die neueste Version von AVRStudio installiert ist. Aus den `.xml`-Dateien im Ordner `\AVR Tools\PartDescriptionFiles\` wird dann diese Datei erneut erzeugt
- [AVRootloader.dll](./AVRootloader.dll), Dynamisch linkbare Bibliothek mit den Interfaces der PC-Software f�r eigene Anwendungen, ist nicht zwingend erforderlich und kann gel�scht werden
- [AVRootIntf.pas](./AVRootIntf.pas),   Delphi Source zum Zugriff auf die `.dll`.  
Ist nicht zwingend erforderlich und kann auch gel�scht werden

Features:
- Autobaudrate Detektion
- feste Baudrate m�glich
- 1-Wire, nur ein Portpin wird ben�tigt
- 2-Wire, normales RS-232
- PC-Software erkennt automatisch ob 1-Wire oder 2-Wire Modus
- f�r beide Modis sind die Pegel invertierbar
- zu schreibende/lesende Daten und Befehle sind per 16 Bit CRC abgesichert
- sehr kompakt
- passwortgesch�tzt (Zugang sowie Verschl�sselung)
- optimiertes Programmieren des FLASHs
- implizites Verify beim Schreiben/L�schen des FLASHs und Schreiben des EEPROMs
- Verschl�sselung der FLASH/EEPROM Daten per [XTEA](https://de.wikipedia.org/wiki/Extended_Tiny_Encryption_Algorithm)  
mit modifiziertem doppelten Cipher Block Chaining Feedback Modus
- HEX-Editor zum Lesen/Schreiben/�ndern des EERPOMs
- HEX-Editor zum Lesen/Schreiben/�ndern des SRAMs, **VORSICHT** ist gef�hrlich
- falls AVRStudio korrekt installiert wurde, so zeigt der SRAM-HEX-Editor zu den Speicherzellen den Namen der Register/Ports usw. an. Farblich ist:
  - Pink..Registerfile
  - Blau..IO Bereich/Ports
  - Gr�n..Speicherbereich des Buffers, den der Bootloader benutzt
  - Wei�..Stack
- unterst�tzt alle 8Bit-AVRs die self-programierbar sind und einen SRAM mit mehr als `PageSize*2` Bytes haben (siehe [AVRootloader.asm](./AVRootloader.asm))
- Versionsverwaltung der eigenen Software.  
**Downgrades k�nnen mit der Bitmaske verhindert werden!**

## Verschl�sselung

### XTEA_Enc Code

```delphi
procedure TEAEnc(const Source; var Dest; var Feedback; const Key);
const
  TEA_Delta: Cardinal = $9E3779B9;
type
  TLongArray = array[0..3] of Cardinal;
  PLongArray = ^TLongArray;
var
  I: Integer;
  A,B,Sum: Cardinal;
  S,D,F,K: PLongArray;
begin
  Sum := 0;
  S := @Source;
  D := @Dest;
  F := @Feedback;
  K := @Key;
  A := S[0] xor F[0];
  B := S[1] xor F[1];
  for I := 0 to 15 do
  begin
    Inc(A, (((B shl 4) xor (B shr 5) + B) xor (Sum + K[Sum and 3])));
    Inc(Sum, TEA_Delta);
    Inc(B, (((A shl 4) xor (A shr 5) + A) xor (Sum + K[(Sum shr 11) and 3])));
  end;
  A := A xor F[0];
  B := B xor F[1];
  for I := 0 to 15 do
  begin
    Inc(A, (((B shl 4) xor (B shr 5) + B) xor (Sum + K[Sum and 3])));
    Inc(Sum, TEA_Delta);
    Inc(B, (((A shl 4) xor (A shr 5) + A) xor (Sum + K[(Sum shr 11) and 3])));
  end;
  F[0] := F[0] xor A;
  F[1] := F[1] xor B;
  D[0] := A;
  D[1] := B;
end;

Dest = Source xor Feedback
Dest = XTEA(Dest) die ersten 16 Runden
Dest = Dest xor Feedback
Dest = XTEA(Dest) die zweiten 16 Runden
Feedback = Feedback xor Dest
```

Das zweifache XOR'ing des `Feedback` vor den ersten 16 Runden und vor den zweiten 16 Runden ver�ndert den [XTEA](https://de.wikipedia.org/wiki/Extended_Tiny_Encryption_Algorithm) auf eine Weise, das eine "Alles oder Nichts" Entschl�sselung entsteht. �ndert sich also nur 1 Bit in den Daten, so wird der komplette Datenblock und alle auf ihn nachfolgenden Datenbl�cke falsch entschl�sselt. Der letzte Datenblock enth�lt immer eine Signatur und die Adresse an die die Daten im FLASH oder EEPROM geschrieben werden sollen. Eine fehlerhafte Entschl�sselung aufgrund eines Bitfehlers bei der �bertragung oder der Manipulation der Daten durch Dritte wird also zuverl�ssig erkannt.

### XTEA_Dec Code

```delphi
Buffer = Source xor Feedback
Dest = XTEA_dec(Source) die ersten 16 runden
Dest = Dest xor Feedback
Dest = XTEA_dec(Dest) die zweiten 16 runden
Dest = Dest xor Feedback
Feedback = Buffer
```

Der Entschl�sselungscode f�r den AVR ist so aufgebaut, das im SRAM folgende Blockstruktur vorliegt: (1 Block = 8 Bytes)

`[Feedback] [Buffer] [Source 0] [Source 1] [Source 2] [Source x]`

zuerst wird `Source` in 8 Register geladen -> `A,B`
nun wird `Buffer = Feedback xor A,B` berechnet. 
Dann `A,B` inplaced 16 Runden entschl�sselt. 
Dann `A,B` mit `Feedback xor` verk�npft. 
Dann `A,B` inplaced 16 Runden entschl�sselt. 
Dann `Feedback` mit `A,B xor` verkn�pft,
`Feedback` enth�lt nun den entschl�sselten Datenblock
`Buffer` wird zum neuen `Feedback`.

im Speicher lag folgendes

`[Feedback] [Buffer] [Source 0] [Source 1] [Source 2] [Source x]`

und jetzt

`[Dest 0] [Buffer] [Source 0] [Source 1] [Source 2] [Source x]`

`[Buffer]` wird zum n�chsten `[Feedback]` f�r `[Source 1]`

`[Dest 0] [Feedback] [Buffer] [Source 1] [Source 2] [Source x]`

nun beginnt obiger Vorgang erneut und es entsteht

```
[Dest 0] [Dest 1] [Feedback] [Buffer  ] [Source 2] [Source x]
[Dest 0] [Dest 1] [Dest 2  ] [Feedback] [Buffer  ] [Source x]
[Dest 0] [Dest 1] [Dest 2  ] [Dest X  ] [Feedback] [Source x]
```

Der `Dest X` Block = letzte Block enth�lt 8 zus�tzliche Datenbytes in denen die ersten 4 Bytes aus den ersten 4 Bytes des `BootKey:` bestehen. Die n�chsten 4 Bytes vom `Dest X` Block enthalten die Adressinformationen, an die im FLASH/EEPROM geschrieben werden soll.

Bei der n�chsten Nachricht wird der Inhalt vom `[Feedback]` wieder an den `SRAM_START` kopiert.

Bevor das System �berhaupt die verschl�sselten Datenbl�cke sendet, wird es einen 16 Bytes verschl�sselten Initialisierungsrecord senden. Dieser besteht unverschl�sselt aus 8 Bytes Zufallsdaten, 4 Bytes mit den Werten in `BootInfo` und die ersten 4 Bytes des Passwortes. Das ist der Kommandocode `$FE $FE`. Das Feedbackregister dieses 1. Nachrichtenblocks wird mit `0` gef�llt.

Durch diesen speziellen Doppel-CBC-Modus mit R�ckverkn�pfung der verschl�sselten Daten per XOR in das n�chste Feedback wird eine Verkettung aller Bl�cke untereinander erreicht. Da wir den allerersten Datenblock mit 8 Zufallsbytes bef�llen, bedeutet dies, dass die komplette Nachricht immer pseudozuf�llig ist, selbst wenn sie immer mit gleichen Passwort und gleicher Nachricht verschl�sselt wurde.

## Verschl�sselte Updatedateien

1. starte [AVRootloader.exe](./AVRootloader.exe)
2. dr�cke "make password"
3. w�hle im Dateidialog die Datei [AVRootloader.asm](./AVRootloader.asm) aus
4. entscheide ob das per Zufall erzeugte Passwort in [AVRootloader.ini](./AVRootloader.ini) gespeichert werden soll. Das erzeugte Passwort wird auch in die **Zwischenablage** kopiert und kann so anderenorts gespeicheret werden. Wird das Passwort in [AVRootloader.ini](./AVRootloader.ini) gespeichert, so wird es beim Flashen von dort geladen, falls n�tig. Wird das Passwort nicht in [AVRootloader.ini](./AVRootloader.ini) gespeichert, so fragt [AVRootloader.exe](./AVRootloader.exe) es jedesmal ab. M�chte man verschl�selte Offlineupdates f�r Kunden erzeugen, so darf das **Passwort** nat�rlich **nicht in** [AVRootloader.ini](./AVRootloader.ini) **gespeichert** werden!
5. �ffne mit AVRStudio die Datei [AVRootloader.aps](./AVRootloader.aps)
6. konfiguriere [AVRootloader.asm](./AVRootloader.asm), setze `UseCrypt=1` und `UseCryptFLASH=1` und `UseCryptE2=1` und `UseSRAM=0`. Das ist die sicherste Konfiguration. M�chtest du, dass man den EEPROM nicht auslesen kann, so setze `UseE2Read=0`. M�chtest du, dass man den EEPROM auslesen und schreiben kann ohne Verschl�sselung so setze `UseE2Write=1`, `UseE2Read=1` und `UseCryptE2=0`. M�chtest du das man den EEPROM auslesen aber nur verschl�sselt schreiben kann so setze `UseE2Write=1`, `UseE2Read=1` und `UseCryptE2=1`. Wenn du `UseSRAM=1` und/oder `UseCryptFLASH=0` setzt, so kann ein Angreifer die Sicherheit umgehen, indem er diese Funktionen benutzt um eine eigene Anwendung unverschl�sselt zu flashen und mit Hilfe dieser Anwendung das Passwort in `BootKey:` ausliest. Wichtig ist also, dass man `UseSRAM=0` und `UseCryptFASH=1` setzt, wenn man es sicher haben m�chte.
7. das vorher erzeugte Passwort wurde schon in `Bootkey:` eingetragen.
8. kompiliere das Projekt wie in oben beschrieben
9. flashe `AVRootloader.hex` in den AVR
10. starte [AVRootloader.exe](./AVRootloader.exe) und konfiguriere alle Parameter (COM-Port, Baudrate etc.pp.)
11. w�hle im Eingabefeld "FLASH" eine `.hex`-Datei, die sp�ter in den FLASH gespeichert werden soll
12. w�hle im Eingabefeld "EEPROM" eine `.eep`-Datei, die sp�ter ins EEPROM gespeichert werden soll
13. dr�cke Button "compile"
14. w�hle im Dateidialog den Namen und Speicherort der `.acy`-Datei aus
15. falls der Passwortdialog erscheint, so gebe das vorher erzeugte Passwort ein, z.B. paste �ber die Zwischenablage. Das Passwort sollte in HEXadezimal eingegeben werden, Kommatas oder �hnliche Sonderzeichen werden ignoriert, die Buchstaben A-F k�nnen auch klein geschrieben werden
16. dr�cke OK und das Programm erzeugt eine `.acy`-Datei, die alle Daten der FLASH/EEPROM Dateien verschl�sselt enth�lt
17. nun testen wir, indem wir im Eingabefeld "FLASH" diese `.acy`-Datei ausw�hlen und den Button "Program" dr�cken. Die Eingabe im Edit "EEPROM" ist dabei irrelevant.
18. Wird bei einem Kunden [AVRootloader.exe](./AVRootloader.exe) so installiert, dass in [AVRootloader.ini](./AVRootloader.ini) bei `[System]` -> `Password=` nichts drinnen steht, so kann man eine verschl�sselte `.acy`-Datei dem Kunden ausliefern und diese kann er auf den AVR mit dem Bootloader einspielen.
19. man kann auch `.acy`-Dateien unverschl�sselt erzeugen. Dazu darf der Bootloader eben keine Entschl�sselungsfunktion aktiviert haben.
20. Die verschl�sselten `.acy`-Dateien laufen nur auf dem Bootloader der beim Erzeugen der `.acy`-Dateien verbunden war. Eine `.acy`-Datei enth�lt also die Informationen aus `BootInfo:` -> die Signatur des AVRs, die Version des Bootloaders und die Anzahl der FLASH Pages des Bootloaders. Bei verschl�sselten `.acy`-Dateien liegen diese Informationen im ersten verschl�sselten 16 Bytes Datenblock, der an den Bootloader gesendet wird. Der Bootloader entschl�sselt diesen 1. Datenblock und vergleicht diese Daten dann mit seiner `BootInfo:` und den ersten 4. Bytes des `BootKey:` Die ersten 8 Bytes dieses 1. Datenblockes bestehen immmer aus Zufallsdaten. Durch den verwendeten CBC Feedback Modus bei der Verschl�sselung propagieren sich diese 8 Zufallsbytes durch alle nachfolgenden verschl�sselten Datenbl�cke. Dies verhindert effizient verschiedene kryptographische Angriffe wie z.B. Choosen Plain Text Attack, Replyattack, Differentielle Kryptoanalyse, Blockvertauschungen, Blockersetzungen. Die Speicheradressen der verschl�sselten FLASH/EEPROM Daten werden ebenfalls nur verschl�sselt gespeichert und �bertragen. Dies erfolgt bei solchen Datenpaketen im letzten 8 Bytes Datenblock. In diesem Datenblock liegen verschl�sselt die ersten 4 Bytes des Passwortes als Signatur, 1 Byte Gr��enkorrektur des Datenblockes das von 0 bis 7 gehen kann, und 3 Bytes Adresse an denen diese Daten im FLASH/EEPROM gespeichert werden sollen. Die ersten 4 Bytes werden mit dem internen ersten 4 Bytes des `BootKey:` verglichen und dienen als Signatur. Sollte bei der Daten�bertragung oder durch Manipulation in den Daten Modifikationen aufgetreten sein so wird diese Signatur nicht mehr stimmen k�nnen. Der verwendete CBC-Doppel-Feedback-Modus des Ciphers arbeitet also wie eine 64Bit Checksumme mit 32Bit Pr�fcode.

## Selektive EEPROM Updates

1. [AVRootloader.exe](./AVRootloader.exe) starten und AVR mit RS-232 verbinden.  
Es wird vorausgesetzt, dass auf diesem AVR schon der Bootloader l�uft
2. EEPROM aus dem AVR auslesen �ber Page "EEPROM Content" und Button "read from Device"
3. nun im HEX-Editor die Zellen, die man �ndern m�chte, mit neuen Werten modifizieren
4. nun mit der Maus alle Speicheradressen, die man updaten m�chte, markieren (schwarz)
5. rechten Mausklick in den Editor und im Popupmenu "select Cells" ausw�hlen, danach sollten alle markierten Bereiche rot hervorgehoben sein
6. wiederhole Schritt 5. je nach Bedarf
7. auf Button "save to File" dr�cken und `.eep`-Datei ausw�hlen
8. fertig, die `.eep`-Datei enth�lt nun nur die vorher markierten Speicherzellen, sie kann nun auf der ersten Page der Software ausgew�hlt werden, von dort direkt im AVR gespeichert werden oder als verschl�sselte `.acy`-Datei kompiliert werden.

## Danksagung

Danksagung geht an Peter Dannegger. Von ihm habe ich die UART delay, UART getc + CRC und meine Baudraten Erkennung angelehnt.  
Der HEX-Editor stammt von Mirkes.de wurde aber nachgebessert.

## Versionsgeschichte

### Version 6.0
- Updates/Bugfix zur Anpassung an [AVR-Studio 4.16](https://www.mikrocontroller.net/articles/Atmel_Studio)

### Version 5.0
- einige Verbesserungen beim Verbindungsaufbau, speziell Timeout Behandlung.  
Mit `UseResetDelay=0` oder `1` stellt man das Timeout Verhalten des Bootloaders ein.
- neue Spezial Funktionen "read_flash" und "write_flash", um den FLASH aus der eigenen Anwendung lesen und auch schreiben zu k�nnen,  
aktivierbar mit `UseSpecialWrite, UseSpecialWriteBoot, UseSpecialRead, UseSpecialMsg`
- `write_flash()` arbeitet dabei ohne Restriktionen in der Ausrichtung der Speicheradressen und Gr��en. Man kann also durchaus einige Bytes aus einem SRAM Buffer mitten in den FLASH schreiben ohne dabei R�cksicht auf die Pages nehmen zu m�ssen.
- das Test Projekt wurde dementsprechend angepasst und demonstriert nun auch, wie man aus der Anwendung heraus das Passwort in `BootKey:` neu setzen kann.
- Bugfix: Das Problem mit extrem geringen `XTAL` Werten (128kHz) und den Timeout Schleifen ist ebenfalls gefixt. Desweiteren wurden einige Watchdog Reset Aufrufe anders platziert.

### Version 4.0
- Verbindungsaufbau wurde ge�ndert, l�uft nun wesentlich stabiler und sicherer
- anderes Verhalten bei `UseWDT=1`, nun wird der Watchdog explizit aktiviert und auf 2 Sekunden Timeout programmiert. Sollte innerhalb des Bootloaders ein Timeout entstehen, so wird ein WDT Reset durchgef�hrt. Die PC Software sendet dazu bei Inaktivit�t ein Idle Kommando damit der Bootloader den WDT zur�cksetzen kann. Somit erkennt sowohl der Bootloader und auch die PC-Software, wenn es zu einem ungewollten Verbindungsabbruch kommt. Bei einem solchen wird der Bootloader nach max. 2 Sekunden einen RESET durchf�hren und die PC-Software die Verbindung automatisch beenden.
- `UseBootVector` eingebaut. Damit kann man am FLASH-Ende einen Sprung zum Bootloader Start eingef�gt werden. Dieser Vektor ist hilfreich, wenn man den Bootloader direkt per `jump` aufrufen m�chte. 
- `UseSpecialFuncs` eingebaut. Bei ATMegas mit gro�er Bootsection verbraucht der Bootloader sehr wenig Platz im FLASH. Somit steht in der Bootloader Sektion noch sehr viel ungenutzer FLASH zur Verf�gung. Diesen kann man mit eigenen Tabellen oder Code ausf�llen. Einige Beispiele f�r solche Funktionen findet man im Include [Special.inc](./Special.inc). Aufrufen aus WinAVR GCC kann man sie wie in [AVRootloader.h](./AVRootloader.h) gezeigt.
- Versionsnummernverwaltung eingebaut. Damit kann man zur eigenen Anwendung eine Versionsnummer verwalten lassen. Diese Versionsnumer kann bei Updates mit aktivierter Verschl�sselung auch gepr�ft werden. In einem solchen Fall akzeptiert der Bootloader nur verschl�sselte Daten deren Versionnummer **gleich oder h�her** der im AVR programmierten ist.
- die frei definierbare `BootMsg` geht jetzt auch mit aktivierter Verschl�sselung
- neue M�glichkeiten beim Verbindungsaufbau. Die PC-Software kann vor einem Verbindungsaufbau einen frei definierbaren String senden. Dieser wird in der [AVRootloader.ini](./AVRootloader.ini) im Wert `AppCmd=` gespeichert. Auf diesen String kann die Anwendungssoftware in deren UART Funktionen reagieren und den Bootloader starten (per WDT oder direktem `jump`). Zus�tzlich kann die Anwendung vorher auch noch einen String an die PC-Software zur�ck senden. Wenn dieses Feature benutzt werden soll, so muss in [AVRootloader.ini](./AVRootloader.ini) in `AppCmdResponse=` dieser String gespeicheret werden. Die PC-Software wartet dann nach Senden von `AppCmd=` auf dieses `AppCmdResponse=`, bevor sie weitermacht. Wurde dieser Wert empfangen, so kann die PC-Software einige Zeit warten, wenn in der `.ini` bei `[Timeouts] AppCmd=` gesetzt wird. Ein Wert von z.B. 50 bedeutet eine Wartezeit von 50 Millisekunden. Erst nach all diesen Schritten wird mit dem Bootloader ein Verbindungsaufbau versucht.
- `[Timeouts] KeepAlive=` ist neu. Dieser Wert stellt das Intervall in Millisekunden dar, das benutzt wird, wenn die PC-Software mit dem Bootloader verbunden ist und periodisch �berpr�ft, ob der AVR noch verbunden ist. Dieser Wert sollte auf 500 stehen und garantiert bei `UseWDT=1`, dass der Bootloader somit periodisch seinen Watchdog Timer zur�cksetzt. Siehe oben WDT Verhalten.
- `[Timeouts] MaxPacketSize=` neu eingebaut. Damit wird die maximale Anzahl an Bytes festgelegt die beim Senden gro�er Datenb�cke benutzt werden soll. Sendet man also z.B. 1024 Bytes f�r das EEPROM und hat `MaxPacketSize=256` so wird dieser Datenblock in Bl�cke a 256 gesendet und jeder einzeln per CRC abgesichert und auf die Empfangsbest�tigung vom AVR gewartet. Normalerweise sollte dieser Wert nicht festgelegt werden. Bei **Bluetooth**-Adaptern aber evtl. notwendig, wegen **Buffergr��e**.
- `[Timeouts] Connect=` ist neu. Dieser Timeout stellt die Wartezeit der PC-Software dar, die sie auf eine R�ckmeldung vom AVR bei beim Verbindungsaufbau warten soll.

### Version 3.0
- Versionsverwaltung f�r eure eigene Software in [AVRootloader.asm](./AVRootloader.asm) eingebaut. Die Versionsnummer ist 4 Bytes gro� und wird in den letzten 4 Bytes im Anwendungsflash gespeichert. Bei AVRs mit Bootsektion exakt die 4 Bytes vor BootStart, also letzten 4 Bytes des nutzbaren FLASHs f�r eure Anwendung. Bei AVRs ohne Bootsektion steht in den letzten 2 Bytes der JMP zu Main. Deshalb steht die Version in den 4 Bytes davor. Ihr k�nnt also in eurer Software per LPM auf diese Versionsnummer zugreifen. Der Bootloader sendet diese Versionsnumer beim Connect zur PC-Software. Diese stellt die Versionsnummer im "Device Information" Fenster dar. In [AVRootloader.asm](./AVRootloader.asm) k�nnt ihr per `UseVersioning=1` die Versionsverwaltung aktivieren. Wird die Verschl�sselung als Updatem�glichkeit benutzt, kann bei einem Update eurer AVRs mit einem kompiliertem `.acy` File eine Versionsnummernpr�fung erfolgen. Dabei vergleicht der Bootloader im AVR die im `.acy` verschl�sselt gespeicherte neue Versionsnummer mit der aktuell installierten Versionsnummer. Ist die neue kleiner als die alte wird ein Fehler zur�ckgemeldet und der AVR kann nicht programmiert werden. In der PC-Software sind 4 Eingabefelder, in denen ihr eure Versionsnummer eingeben k�nnt die bei der Kompilation einer `.acy`-Datei benutzt werden sollen. �ber die "Verify Mask" k�nnt ihr bestimmen, welche Teile der Versionsnummer im AVR �berpr�ft werden sollen. Somit k�nnt ihr ein "Back-Update" des AVRs durchf�hren indem ihr einfach eine `.acy`-Datei kompiliert und alle 4 Checkboxen abhackt. Diese Versionsverwaltung ist kryptographisch sicher, also nur ihr als Besitzer des Pa�wortes bestimmt das Verhalten der Versionsverwaltung. Alle Daten sind verschl�sselt, sowohl in der erzeugten `.acy`-Datei, wie auch bei der Daten�bertragung zum AVR. Nur der Bootloader im AVR entschl�sselt und vergleicht diese Versionsnummern. Diese gesch�tzte Versionsverwaltung funktioniert beim Schreiben des FLASHs wie auch EEPROMs. Daf�r muss aber die zwingende Verschl�sselung des FLASHs und EEPROMs in [AVRootloader.asm](./AVRootloader.asm) aktiviert sein.
- Autodetektion der COM-Ports auf dem PC. Dabei werden die installierten und verf�gbaren COM-Ports im System ermittelt und mit ihren FriendlyName angezeigt.
- Autodetektion des COM-Ports an dem der Bootloader angeschlossen ist. In der COM-Port Auswahlliste einfach "AUTO" ausw�hlen. Diese k�nnt ihr auch in der Kommandozeile nutzen. Ist AUTO gew�hlt und es wird eine Verbindung aufgebaut dann versucht die PC-Software jeden COM-Port in der Auswahlliste. Die Anzahl der Trials bei diesem Versuch pro COM-Port kann in der [AVRootloader.ini](./AVRootloader.ini) -> `[System]` -> `ConnectTrials=?` eingestellt werden. Damit dies aber funktioniert muss euer Ger�t autom. in den Bootloader wechseln k�nnen. Es gibt mehrere Wege, z.B. PinChange ISR am RX Pin des Bootloader, oder RTS-Leitung der RS-232 am RESET des AVRs und `RTSPulse=/RTSInterval=` in [AVRootloader.ini](./AVRootloader.ini) entsprechend konfiguriert.
- Neu ist die M�glichkeit, dass die PC-Software bei der Verbindung einen definierbaren String sendet. Eure Anwendung im AVR, die selber die UART benutzt, kann diesen Befehl dann benutzen, um in den Bootloader zu springen. Dazu konfiguriert ihr in [AVRootloader.ini](./AVRootloader.ini) -> `[System]` -> `AppCmd=` Der Wert in `AppCmd` kann Sonderzeichen enthalten z.B. so `AppCmd=RunBoot/0ATest//` Ein Slash mit anschlie�end einem HEXadezimalen Wert, immer zwei Zeichen lang. Ein Doppelslash hebt den ESCapecode auf. VORSICHT! ihr k�nnt nicht das Zeichen #13 -> /0D benutzen, leider. Das liegt daran, dass die Baudrate Detection im Bootloader dieses Sonderzeichen auswertet. Wollt ihr es denoch benutzen so m�sst ihr den Bootloader ohne "Baudrate Detection" konfigurieren, also mit fest eingestellter Baudrate (was meistens dann auch Sinn macht).
- Auswahlbox f�r das `BootSign` in PC-Software eingebaut. Man kann, wenn man 1-Wire Kommunikation benutzt, alle AVRs eines Ger�tes am gleichen 1-Wire-Pin verdrahten. Alle AVRs, k�nnen unterschiedliche Typen sein, benutzen aber den AVRootloader. Nun kann man mit der PC-Software all diese AVRs updaten.
- Fehler beim Auswerten der Kommandozeile beseitigt, Leerzeichen im Pfad wurde als Separator erkannt.
- Fehler bei der `BootMsg` beseitigt. Die `BootMsg` wurde in [AVRootloader.asm](./AVRootloader.asm) verschoben und kann nun auch mit der Verschl�sselung benutzt werden.
- Der Wert im Eingabefeld "ACY Info" in der PC-Software wird beim Kompilieren einer `.acy`-Datei in diese lesbar gespeichert. Neben diesem Wert auch die lesbare Versionsnummer. Mit einem einfachen Texteditor kann man diese Infos aus einer `.acy`-Datei auslesen.
- Kompatibilit�t zum Bootloader Version 1 und 2 sollte gew�hrleistet sein.
- einige kleinere Darstellungsfehler beseitigt
- alle Dateien wie [AVRootloader.dev](./AVRootloader.dev) und [AVRootloader.asm](./AVRootloader.asm) wurden auf die neuesten AVRs aktualisiert (AVRStudio 4.15 Build 623)
- neuer Parameter `-SBOOT` f�r die Kommandozeile eingebaut, damit kann man das `BootSign` f�r den Bootloader-Connect vorgeben
- f�r diejenigen die die [AVRootloader.dll](./AVRootloader.dll) benutzen. Diese ist nicht kompatibel zur alten Version!

### Version 2.0
- die `SIGNATURE_CRC` wurde entfernt. Der neue ATtiny88 �berschneidet sich mit dem ATMega161 beim Verfahren der Version 1.0
- n�tige �nderung daf�r ist in [AVRootloader.asm](./AVRootloader.asm) -> `BootSign:` die 4 Bytes
- die PC-Software kann aber weiterhin mit der Version 1.0 benutzt werden, ist also abw�rtskompatibel
- [AVRootloader.dev](./AVRootloader.dev) aktualisert auf die neuesten AVRs und beim ATMega161/ATMega161comp. (ATMega162 im 161 Kompatibilit�tsmodus) Fehler aus den AVRStudio XML Dateien beseitigt. ATMEL hat dort falsche Angaben gemacht.
- die PC-Software ist intern komplett neu geschrieben worden. Wer Interesse hat, kann die [AVRootloader.dll](./AVRootloader.dll) benutzen, um die komplette Funktionalit�t der PC-Software in seine eigene Anwendung zu integrieren. Diese DLL implementiert die Funktionalit�t �ber Interfaces, Delphi Header in [AVRootIntf.pas](./AVRootIntf.pas). �ber diese Schnittstelle kann man auch eigene Communications-Interfaces vorgeben, �ber die dann die AVRootloader Schnittstelle kommuniziert, z.B. AVR309 USB o�.
- �ber [AVRootloader.ini](./AVRootloader.ini) in Section `[Timeouts] RTSPulse=` kann man das Verhalten beim Verbindungsaufbau der PC-Software von der RTS-Leitung der RS-232 einstellen. Man kann �ber diese Option also den RTS PIN der RS-232 z.B. f�r x Millisekunden von HIGH auf LOW und wieder HIGH ziehen. Damit k�nnte man, �ber Serienwiderstand und Schottky Diode den RTS Pin auf den RESET Pin des AVRs klemmen. Bei jedem Verbindungsaufbau der PC-Software zum Bootloader wird somit autom. der AVR in den Reset versetzt.
- Wer m�chte kann ein Copyright String o�. in [AVRootloader.asm](./AVRootloader.asm) einbauen. Dieser String wird dann in der PC-Software in der Device Information angezeigt.
  Das geht so:
```
BootSign:	.db		"BOOT"
BootInfo:	.db		SIGNATURE_001, SIGNATURE_002, BootVersion, BootPages
BootMsg:    .db     SUCCESS, "Hello World"
BootEnd:
```
Wichtig ist, dass das erste Byte immer `SUCCESS` ist.

- Das Timeout-Handling in der Baudratedetektion wurde ge�ndert. Sollte innerhalb des Timeouts, z.B. 250ms, eine g�ltige Baudrate detektiert worden sein, so verl�ngert sich der Timeout ab diesem Moment um weitere 250ms. Entweder der Bootloader empf�ngt das korrekte `BootSign:` (z.B. `BOOT`) und springt in den Kommandomodus oder aber die Baudrate-Detektion startet erneut mit weiteren 250ms. Wenn also Ruhe auf der RX-Leitung ist, so startet die Applikation wie gewohnt nach dem Timeout. Wenn aber eine PC-Software versucht eine Verbindung aufzubauen so bleibt der Bootloader solange in der Baudrate-Detektion bis die PC-Software aufh�rt eine Verbindung aufzubauen. Diese Vorgehensweise macht das Verfahren wesentlich robuster. In Version 1.0 war der Timeout ein Gesamt-Timeout des Verbindungsversuches. Nach den z.B. 250ms wurde immer die Applikation gestartet, egal ob eine PC-Software einen Verbindungsaufbau versuchte oder nicht.

### Version 1.0
Siehe auch [Historie auf �C.net](https://www.mikrocontroller.net/articles/AVR-Bootloader_mit_Verschl�sselung_von_Hagen_Re#Historie)

## Links

[AVR-Bootloader mit Verschl�sselung von Hagen Re](https://www.mikrocontroller.net/articles/AVR-Bootloader_mit_Verschl�sselung_von_Hagen_Re)

[Forenthread auf �C.net](https://www.mikrocontroller.net/topic/95839#new)

&copy; Hagen Reddmann 2008..10