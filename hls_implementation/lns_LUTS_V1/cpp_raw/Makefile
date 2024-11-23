# Makefile
programs/main	:	programs/main.cpp includes/ Makefile
	g++ -I includes/ -o bin/main programs/main.cpp

.PHONY: clean
clean:
	rm -f bin/main
