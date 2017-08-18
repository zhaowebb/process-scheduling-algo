#include "Task.h"
#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>
using namespace std;
/*
 * Implement this function using the FCFS algorithm in Part 1.
 * Note that the second parameter is unused. Literally. You should not use it.
 */
struct is_less {
	bool operator() (const Task& lhs, const Task& rhs) {
		return lhs.arrival < rhs.arrival;
	}
};

void schedule(std::vector<Task> tasks, int unused) {
	if(tasks.size() == 0){
		cout << "Invalid parameter." << endl;
		return;
	}
	sort(tasks.begin(), tasks.end(), is_less());
	queue<Task> q;
	int cur_time = 0;
	int ind = 0;
	int last_begin_time = 0;
	float waiting = 0;
	bool is_running = false;
	
	//keep running until all tasks have executed
	while(ind < tasks.size() || !q.empty()){
		//adding tasks arrive at this moment
		while(ind < tasks.size() && tasks[ind].arrival == cur_time){
			q.push(tasks[ind]);
			cout << cur_time << ": Adding task " << tasks[ind].name << endl;
			ind++;
		}
		//execute the appropriate task
		if(!is_running && !q.empty()){
			cout << cur_time << ": Running task " << q.front().name << endl;
			is_running = true;
			last_begin_time = cur_time;
			waiting += (cur_time - q.front().arrival);
		}
		//check if the executing task is done
		if(!q.empty() && cur_time == last_begin_time + q.front().time){
			cout << cur_time <<": Finished task " << q.front().name << endl;
			q.pop();
			is_running = false;
			//is there a following task to be executed
			if(!q.empty()){
				cout << cur_time << ": Running task " << q.front().name << endl;
				is_running = true;
				last_begin_time = cur_time;
				waiting += (cur_time - q.front().arrival);
			}
		}

		cur_time++;

	}

	cout << cur_time - 1 << ": All tasks finished" << endl;
	if(tasks.size() != 0){
		cout << "Average waiting time is " << waiting / tasks.size() << endl;
	}
	
}


