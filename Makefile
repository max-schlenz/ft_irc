
CXX				=	c++
MAKEFLAGS		=	#j
CXXFLAGS		= 	-std=c++98 #-Wall -Wextra -Werror 

NAME			=	server

SRC				= 	server

OBJ				=	$(addsuffix .o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
	@if find ${OBJ} > /dev/null 2>&1; then \
		rm -f $(OBJ); \
	fi

fclean: clean
	@if [ -f "${NAME}" ]; then \
		rm -f ${NAME}; \
	fi

re: fclean all

.PHONY: all clean fclean re
