NAME		=	philo

FLAGS		=	-Wall -Wextra -Werror -pthread -fsanitize=thread -g3
RM_DIR		=	rm -rf
RM_FILE		=	rm
INCLUDES	= 	-I .

HEADER		=	./philo.h
SRC			=	philo.c \
				init_philo.c \
				utiles.c

OBJ_DIR		=	obj
OBJ 		=	$(addprefix $(OBJ_DIR)/,$(SRC:.c=.o))

all:	$(NAME)

${NAME}:	${OBJ}
			$(CC) -o $(NAME) $(OBJ) $(FLAGS)

$(OBJ)		:	| $(OBJ_DIR)

$(OBJ_DIR)	:
			@mkdir -p $(OBJ_DIR)
			
$(OBJ_DIR)/%.o :	%.c $(HEADER) Makefile
					${CC} $(FLAGS) $(INCLUDES) -o $@ -c $<

clean:
		@${RM_DIR} $(OBJ_DIR)
				
fclean:	clean
		@${RM_FILE} $(NAME)
				
re:		fclean all

.PHONY: all bonus clean fclean re
