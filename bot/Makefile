NAME = bot

all:
	g++ -c DiceBot.cpp -o DiceBot.o -I/.
	g++ -c main.cpp -o main.o
	g++ DiceBot.o main.o -o $(NAME)

clean c:
	@rm -rf *.o

fclean f: clean
	@rm -rf $(NAME)

re r: fclean all

.PHONY: all clean fclean re
