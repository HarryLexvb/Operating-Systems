OBJS	= Mymkfifo.o
SOURCE	= Mymkfifo.cpp
HEADER	= 
OUT	= run
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

Mymkfifo.o: Mymkfifo.cpp
	$(CC) $(FLAGS) Mymkfifo.cpp 


clean:
	rm -f $(OBJS) $(OUT)
