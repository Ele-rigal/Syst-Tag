CC	:= gcc

CFLAGS	:= -Wall -Wextra -g3
LDFLAGS	:= -l json-c
EXEC	:= tagit
SRC	:= main.c tag.c parser.c interpreter.c
OBJ	:= $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

test: all 
	@rm -f *.txt *.json
	@rm -r test
	@mkdir test
	@touch machin.txt truc.txt dings.txt blabla.txt


CONF	:= /etc/$(EXEC)/tags.json
BIN	:= /usr/bin
install:
	@if ! [ "$(shell id -u)" = 0 ]; then echo "You are not root, run this target as root please"; exit 1; fi
	@make CFLAGS='$(CFLAGS) -D CONF_PATH="\"$(CONF)\""' all
	@mkdir -p $(shell dirname $(CONF))
	@cp "$(EXEC)" "$(BIN)/$(EXEC)"
	@echo "Installed !"
	@$(BIN)/$(EXEC) install
	@chmod 0777 $(CONF)
	@echo "Configured !"

re: mrproper $(EXEC)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)

