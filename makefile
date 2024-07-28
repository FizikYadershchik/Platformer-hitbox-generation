CC = clang
CFLAGS = -Wall -Wextra -O2
SRC = platforms.c main.c
OBJ = $(SRC:.c=.o)
TARGET = maze_generator

# Правила по умолчанию
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

platforms.o: platforms.c
	$(CC) $(CFLAGS) -c platforms.c -o platforms.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Чистка
clean:
	rm -f $(OBJ) $(TARGET)

# Чистка и удаление исполняемого файла
cleanall: clean
	rm -f $(TARGET)

.PHONY: all clean cleanall

