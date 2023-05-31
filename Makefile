AUTHORS			=	mschlenz, dantonik, tdehne
MAKEFLAGS 		=	--no-print-directory
DATE			=	$$(date +%Y/%m/%d)

CXX				=	c++
CXXFLAGS		=	-std=c++98 -MMD -Wall -Wextra -Werror 

NAME			=	ircserv

SRC_NAME      	=	main \
					Server \
					ServerConnect \
					ServerRequest \
					ServerSetup \
					Client \
					Bot \
					misc \
					Channel \
					parse \
					commands/capreq \
					commands/invite \
					commands/join \
					commands/kick \
					commands/mode \
					commands/nick \
					commands/notice \
					commands/part \
					commands/ping \
					commands/privmsg \
					commands/quit \
					commands/topic \
					commands/user \
					commands/who \
					commands/whois \
					commands/dcc \
					commands/pass \
					commands/bot \

INC_NAME		=	Channel Client Bot Server common

SRC_DIR			=	src/
OBJ_DIR			=	.obj/
INC_DIR			=	include/

OBJ				=	$(addsuffix .o, $(SRC))
OBJ_FILES		=	$(addsuffix .o, $(addprefix $(OBJ_DIR), $(SRC_NAME)))
DEP_FILES		=	$(addsuffix .d, $(addprefix $(OBJ_DIR), $(SRC_NAME)))
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

all:
	@$(MAKE) $(NAME) -j
	$(info Running on $(OS))

$(NAME): $(OBJ_DIR) $(OBJ_FILES) 
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(NAME) -I $(INC_DIR)

-include $(DEP_FILES)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/commands

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	@$(CXX) $(CXXFLAGS) -c $< -I$(INC_DIR) -o $@

clean c:
	@rm -rf $(OBJ_DIR)
	@rm -rf .vscode
	@rm -rf *.dSYM
	@rm -rf *.gch

fclean f: clean
	@rm -rf $(NAME)

re r: fclean all

.PHONY: all clean fclean re
