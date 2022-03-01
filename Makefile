CFLAGS += -Wall -ansi
SRC = tst.c
OBJ = $(SRC:.c=.o)
TARGET = tst

all: $(TARGET)

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

clean:
	rm $(TARGET) $(OBJ)

.PHONY: all clean
