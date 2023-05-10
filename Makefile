AUTHORS			=	mschlenz, dantonik, tdehne
DATE			=	$$(date +%Y/%m/%d)

CXX				=	c++
MAKEFLAGS		=	#j
CXXFLAGS		=	-std=c++98 -g #-Wall -Wextra -Werror 

NAME			=	server
SRC_NAME		=	main exiting Server Client
INC_NAME		=	Channel Client Server irc

SRC_DIR			=	src/
OBJ_DIR			=	.obj/
INC_DIR			=	Classes/

OBJ				=	$(addsuffix .o, $(SRC))
OBJ_FILES		=	$(addsuffix .o, $(addprefix $(OBJ_DIR), $(SRC_NAME)))
SRC_FILES		=	$(addsuffix .cpp, $(addprefix $(SRC_DIR), $(SRC_NAME)))
INC_FILES		=	$(addsuffix .hpp, $(addprefix $(INC_DIR), $(INC_NAME)))

ifeq ($(shell uname -s),Linux)
	OS := Linux
else ifeq ($(shell uname -s),Darwin)
	ifeq ($(shell uname -p),arm)
		OS := MacOS silicone
	else
		OS := MacOS intel
	endif
else
	OS := Unknown
	exit 1
endif

$(info Running on $(OS))

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ_FILES) $(INC_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(NAME) -I$(INC_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -I$(INC_DIR) -o $@

clean c:
	@rm -rf $(OBJ_DIR)

fclean f: clean
	@rm -rf $(NAME)

re r: fclean all

.PHONY: all clean fclean re
