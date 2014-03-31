all:
	g++ -c Peg.cpp -o Peg.o
	g++ Peg.o main.cpp -o pegs 
