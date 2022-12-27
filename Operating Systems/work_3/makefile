OBJS	= thread.o
SOURCE	= thread.cpp
HEADER	= 
OUT	= run
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = -lpthread

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

thread.o: thread.cpp
	$(CC) $(FLAGS) thread.cpp 


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
