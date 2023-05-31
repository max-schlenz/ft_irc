# ft_irc

### Needs Fixing
clean up codebase
toupper implementierung fuer invite

### Must be fixed
- [X]  wenn wir als letzter /part aus nem channel machen bekommen wir eine desync message (von channel der wohl nichtmehr existiert)
- [X]  man kann bei nc immernoch im Channel schreiben, auch wenn man nicht im Channel ist
- [X]  duplicate nickname doesnt work anymore bc nc
- [X]  TONI IS CUTE

### Needs to be implemented 
- [ ]  alles einmal auf Fehler durchchecken → parsing + commands
- [X]  modes: wenn modes +k auf 123 gesetzt wird und man es dann mit dem falschen Passwort wieder löschen will e.g. -k 12 → diese Fehlermeldung fehlt noch. (mode.cpp Zeile 89 das was im else steht)
- [ ]  schauen was im parsing an sich noch fehlt
- [ ]  alle command funktionen so umschreiben, dass entweder true oder false returned wird
- [ ]  commands und parsing alle responses durch Macros ersetzten
- [ ]  topic mit nem leeren String muss das topic loeschen -> "TOPIC #test :"

### Implemented
- [X]  nickname wenn doppelt am anfang - entweder kicken oder richtig handlen
- [X]  operator sollte operator bleiben nach dem joinen von anderen personen
- [X]  kick parsing
- [X]  quit parsing
- [X]  quit implementierung
- [X]  PRIVMSG funktioniert @Max? (i think so 👀)
- [X]  parting multiple channels
- [X]  das topic beim joinen von channels anzeigen
- [X]  wer ist op wenn der op den channel verlaesst?
- [X]  Nachrichten im Channel werden bei mehreren Usern vorne abgeschnitten

### Could get implemented
INFO  
TIME  
LIST  
VERSION  
MOTD  

### Ressources
***[rfc 1459](https://www.rfc-editor.org/rfc/rfc1459) Internet Relay Chat Protocol (1993)***  
[rfc 2810](https://www.rfc-editor.org/rfc/rfc2810) Internet Relay Chat: Architecture (2000)  
[rfc 2811](https://www.rfc-editor.org/rfc/rfc2811) Internet Relay Chat: Channel Management (2000)  
[rfc 2812](https://www.rfc-editor.org/rfc/rfc2812) Internet Relay Chat: Client Protocol (2000)  
[rfc 2813](https://www.rfc-editor.org/rfc/rfc2813) Internet Relay Chat: Server Protocol (2000)  

[Modern IRC Client Protocol](https://modern.ircdocs.horse/)  
[Response codes list](https://www.alien.net.au/irc/irc2numerics.html)  
