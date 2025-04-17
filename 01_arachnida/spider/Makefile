CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDLIBS = -lssl -lcrypto
NAME = spider

SRC_DIR = src
OBJ_DIR = obj
DEP_DIR = dep

SRC = src/arg_parsing.c \
	src/find_images.c \
	src/get_other_page.c \
	src/http_request.c \
	src/https_request.c \
	src/main.c \
	src/socket.c \
	src/tools.c \
	src/http_download.c \
	src/https_download.c

OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))
DEP = $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRC))


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@) $(DEP_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -MMD -MF $(DEP_DIR)/$*.d

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(OBJ) $(DEP)

fclean: clean
	rm -rf $(DEP_DIR) $(OBJ_DIR)
	rm -f $(NAME)

re: fclean all

-include $(DEP)