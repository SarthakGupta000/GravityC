CC = clang
CFLAGS = -Wall
SRC = main.c physics/util.c rendering/util.c

gravityc: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o gravityc -lm

run: gravityc
	./gravityc

clean:
	rm -f gravityc
