all: compile link

compile:
	g++ -c main.cpp -o main.o

link:
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f main *.o
