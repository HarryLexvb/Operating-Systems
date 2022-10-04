OBJS	= pipess.o
SOURCE	= pipess.cpp
HEADER	= 
OUT	= run
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

pipess.o: pipess.cpp
	$(CC) $(FLAGS) pipess.cpp 


clean:
	rm -f $(OBJS) $(OUT)
