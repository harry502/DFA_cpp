DIR=main.cpp
CC=g++
CFG=-std=c++11 -O3
TAG=DFA

test:
	$(CC) $(DIR) $(CFG) -o $(TAG)