main: main.c
	gcc -o main main.c -Wall -Wextra

clean:
	rm main
	rm *.out
