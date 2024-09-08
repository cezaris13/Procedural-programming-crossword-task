file = main.c
executable = main

build:
	gcc $(file) -o $(executable)

run:
	./$(executable)

test:
#todo