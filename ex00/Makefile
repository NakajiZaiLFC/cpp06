NAME := ex00
CXX := c++
CXXFLAGS := -Wall -Werror -Wextra -std=c++98 -pedantic
FILE := main.cpp \
		Bureaucrat.cpp
OBJDIR := obj

OBJ := $(addprefix $(OBJDIR)/, $(FILE:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I. -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

run:re
	clear
	valgrind --leak-check=full --show-leak-kinds=all -q ./$(NAME)

.PHONY: re clean fclean all run