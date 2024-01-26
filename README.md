# ft_irc Project

## Introduction
ft_irc is an IRC (Internet Relay Chat) server implementation, part of the 42 curriculum. It focuses on network programming, client-server architecture, and adhering to the IRC protocol.

## Features
- **Multiple Client Handling**: Supports multiple simultaneous client connections.
- **TCP/IP Communication**: Implements client-server communication using TCP/IP (v4 or v6).
- **Authentication**: Password-based client authentication.
- **Nicknames and Usernames**: Allows setting and changing nicknames and usernames.
- **Channels**: Join, message, and manage channels.
- **Private Messaging**: Send and receive private messages.
- **Operators and Users**: Differentiation between operators and regular users.
- **Channel Commands**: Includes commands like `KICK`, `INVITE`, `TOPIC`, and `MODE`.
- **File Transfer**: Capability for handling file transfers (Bonus).
- **IRC Bot**: Incorporates an IRC bot for additional functionalities (Bonus).

## Installation and Usage
1. Clone the repository
2. Compile the program: `make`
3. Run the server: `./ircserv <port> <password>`

## Technologies Used
- C++ programming language
- Networking protocols and socket programming

### Ressources
***[rfc 1459](https://www.rfc-editor.org/rfc/rfc1459) Internet Relay Chat Protocol (1993)***  
[rfc 2810](https://www.rfc-editor.org/rfc/rfc2810) Internet Relay Chat: Architecture (2000)  
[rfc 2811](https://www.rfc-editor.org/rfc/rfc2811) Internet Relay Chat: Channel Management (2000)  
[rfc 2812](https://www.rfc-editor.org/rfc/rfc2812) Internet Relay Chat: Client Protocol (2000)  
[rfc 2813](https://www.rfc-editor.org/rfc/rfc2813) Internet Relay Chat: Server Protocol (2000)  

[Modern IRC Client Protocol](https://modern.ircdocs.horse/)  
[Response codes list](https://www.alien.net.au/irc/irc2numerics.html)  
