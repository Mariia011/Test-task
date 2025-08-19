CC      := cc
CFLAGS  := -lpthread -lrt -Wall -Wextra -Wpedantic -Werror
LDFLAGS := -Iinclude -DPROJECT_NAME=\"DemoApp\"

SRC := src/main.c
OBJ := $(SRC:.c=.o)
BIN := app

all: $(BIN)

$(BIN): $(OBJ)
	@ $(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $@

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

