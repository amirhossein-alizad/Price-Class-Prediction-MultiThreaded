CC = g++
FLAGS = -std=c++11
COMPILE_FLAGS = $(FLAGS) -c
all: PhonePricePrediction.out

PhonePricePrediction.out: main.o tools.o
	$(CC) main.o tools.o $(FLAGS) -o PhonePricePrediction.out -lpthread

Main.o: main.cpp tools.h
	$(CC) $(COMPILE_FLAGS) main.cpp

tools.o: tools.cpp tools.h
	$(CC) $(COMPILE_FLAGS) tools.cpp

.PHONY: clean

clean:
				rm *.o
				rm *.out
