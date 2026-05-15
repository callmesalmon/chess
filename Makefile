all: build

build: chess.c
	gcc -o chess chess.c
