all: main

main: main.c
	gcc -o main main.c

test: main
	./main 0 input.txt output.txt

clean:
	rm main