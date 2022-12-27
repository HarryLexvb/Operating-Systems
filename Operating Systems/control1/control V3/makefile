OBJS	= control_v3.o
SOURCE	= control_v3.cpp
HEADER	= 
OUT	= run
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

control_v3.o: control_v3.cpp
	$(CC) $(FLAGS) control_v3.cpp 


clean:
	rm -f $(OBJS) $(OUT)
