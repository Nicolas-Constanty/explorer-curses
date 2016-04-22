SRC			=	main.cpp		\
                Explorer.cpp    \
                Menu.cpp        \


CC			=	g++

RM			=	rm -rf

NAME		=	explorer

CPPFLAGS	=	-W -Wall -Wextra -Werror -g

CPPFLAGS	+=	-std=c++11

LDFLAGS		= 	-lncurses -lmenu -lpanel

OBJ			=	$(SRC:%cpp=%o)

$(NAME)	:	$(OBJ)
	@$(CC) $(OBJ) $(LDFLAGS) -o $(NAME)

all		:	$(NAME)

clean	:
	$(RM) $(OBJ)

fclean	:	clean
	$(RM) $(NAME)

re		: fclean all

.PHONY	: all clean fclean re
