CC=g++

generateImage : src/Main.o
	$(CC) -o -std generateImage src/Main.cpp

src/Main.o : $(wildcard src/*.h)
	$(CC) src/Main.cpp
