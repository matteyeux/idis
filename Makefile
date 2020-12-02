TARGET = idis

CFLAGS = -Wall -I.
LDFLAGS = -lcapstone

SRC 	= $(wildcard src/*.c)
OBJ 	= $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	@$(CC) $(OBJ) -o $@ $(LDFLAGS)

src/%.o : src/%.c
	@echo "CC	$<"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)