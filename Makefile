SRC=$(shell find . -name \*.cpp -type f -print)
OBJ=$(patsubst %.cpp, %.o,$(SRC))
NAME=ircserv
FLAGS=-Wall -Werror -Wextra -std=c++98

all:
	make $(NAME)

%.o: %.cpp
	c++ -c $(FLAGS) -g -o $@ $<

$(NAME): $(OBJ)
	c++ $(FLAGS) $(OBJ) -g -o $@

clean:
	find . -name \*.o -type f -delete

fclean: clean
	rm -rf $(NAME)

re: clean all

.PHONY: all clean fclean re
