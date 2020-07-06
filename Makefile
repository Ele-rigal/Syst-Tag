CC		:= gcc

CFLAGS	:= -Wall -Wextra -g3
LDFLAGS	:= -l json-c
EXEC	:= main
SRC		:=main.c tag.c parser.c interpreter.c
OBJ		:= $(SRC:.c=.o)

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

re: mrproper $(EXEC)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)

