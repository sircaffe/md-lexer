lexer: lexer.c
	gcc -o lexer lexer.c -Wall -Wextra -DDEBUG_INFO

clean:
	rm lexer
