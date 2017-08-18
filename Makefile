
all: fcfs RR priorityRR

fcfs:
	c++ -std=c++11 -o fcfs Driver.cpp Fcfs.cpp

RR:
	c++ -std=c++11 -o roundrobin Driver.cpp RoundRobin.cpp

priorityRR:
	c++ -std=c++11 -o priorityRR PriorityDriver.cpp PriorityRR.cpp

clean: 
	rm -rf fcfs RR priorityRR *.o

