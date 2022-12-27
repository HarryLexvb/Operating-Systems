signal: signal.o 
	g++ -o signal signal.o -lm
signal.o: signal.cpp
	g++ -O -Wall -c signal.cpp

clean:
	rm -f signal.o signal
