main: main.c
	gcc -o main main.c -Wall -Wextra -DDEBUG_INFO

clean:
	rm main
