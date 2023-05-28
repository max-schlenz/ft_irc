# ft_irc

### Needs Fixing
clean up codebase
toupper implementierung fuer invite

### Needs to be implemented 
- [ ]  nickname wenn doppelt am anfang - entweder kicken oder richtig handlen
- [X]  operator sollte operator bleiben nach dem joinen von anderen personen
- [ ]  alles einmal auf Fehler durchchecken → parsing + commands
- [ ]  modes wenn würder modes k auf 123 gesetzt wird und man es dann mit dem falschen Passwort wieder löschen will e.g. -k 12 → diese Fehlermeldung fehlt noch. (mode.cpp Zeile 89 das was im else steht)
- [ ]  kick parsing
- [ ]  quit parsing
- [ ]  schauen was im parsing an sich noch fehlt
- [ ]  quit implementierung → alle funktionen so umschreiben, dass
- [ ]  commands und parsing alle responses durch Macros ersetzten
- [X]  PRIVMSG funktioniert @Max? (i think so 👀)
- [X]  parting multiple channels

### Could get implemented
INFO
TIME
LIST
VERSION
MOTD

### Currently implemented
Authentication
PASS  
USER
NICK
PING
PRIVMSG
JOIN

### Ressources
***[rfc 1459](https://www.rfc-editor.org/rfc/rfc1459) Internet Relay Chat Protocol (1993)***  
[rfc 2810](https://www.rfc-editor.org/rfc/rfc2810) Internet Relay Chat: Architecture (2000)  
[rfc 2811](https://www.rfc-editor.org/rfc/rfc2811) Internet Relay Chat: Channel Management (2000)  
[rfc 2812](https://www.rfc-editor.org/rfc/rfc2812) Internet Relay Chat: Client Protocol (2000)  
[rfc 2813](https://www.rfc-editor.org/rfc/rfc2813) Internet Relay Chat: Server Protocol (2000)  

[Modern IRC Client Protocol](https://modern.ircdocs.horse/)  
[Response codes list](https://www.alien.net.au/irc/irc2numerics.html)  
