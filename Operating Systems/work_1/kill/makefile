SRC=main.cpp
LIBS=OStools.cpp
HEADS=OStools.h 
PP=g++
	
mykill: main.o OStools.o
	$(PP) main.o OStools.o -o mykill

main.o: main.cpp 
	$(PP) -c main.cpp 

OStools: OStools.cpp OStools.h
	$(PP) -c OStools.h 

clean: 
	rm *.o mykill 
