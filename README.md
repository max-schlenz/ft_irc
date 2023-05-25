# ft_irc

### Needs Fixing
clean up codebase
toupper implementierung fuer invite

### Needs to be implemented
MODES command + parsing. 
INVITE command. 
KICK parsing?  
Operator when joining channel. 
Only operator priv in channel (implementation std::vector std::string ops). 
PRIVMSG sending msg to multiple channels + parsing. 
QUIT. 

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
