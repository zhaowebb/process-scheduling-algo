#include "Task.h"
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct is_less {
	bool operator() (const Task& lhs, const Task& rhs) {
		return lhs.arrival < rhs.arrival;
	}
};
/*
 * Implement this function using the Round Robin algorithm in Part 2.
 */
void schedule(std::vector<Task> tasks, int quantum) {
	if(quantum <= 0 || tasks.size() == 0){
		cout << "Invalid parameter." << endl;
		return;
	}
	sort(tasks.begin(), tasks.end(), is_less());
	queue<Task> ready;
	int cur_time = 0;
	int ind = 0;
	int last_begin_time = 0;
	int time_eclipse = 0;
	float waiting = 0;
	bool is_running = false;
	//keep running until all tasks have executed
	while(ind < tasks.size() || !ready.empty()){
		//adding tasks arrive at this moment
		while(ind < tasks.size() && tasks[ind].arrival == cur_time){
			ready.push(tasks[ind]);
			cout << cur_time << ": Adding task " << tasks[ind].name << endl;
			ind++;
		}
		//execute the appropriate task
		if(!is_running && !ready.empty()){
			cout << cur_time << ": Running task " << ready.front().name << endl;
			is_running = true;
			last_begin_time = cur_time;
		}
		//check if the executing is done or slice done for it
		if(!ready.empty()){
			if(cur_time == last_begin_time + ready.front().remaining){
				cout << cur_time << ": Finished task " << ready.front().name << endl;
				waiting += (cur_time - ready.front().time - ready.front().arrival);
				ready.pop();
				is_running = false;

				if(!ready.empty()){
				cout << cur_time << ": Running task " << ready.front().name << endl;
				is_running = true;
				last_begin_time = cur_time;
			}
			}
			else if(cur_time == last_begin_time + quantum){
				cout << cur_time << ": Time slice done for task " << ready.front().name << endl;
				ready.front().remaining = ready.front().remaining - quantum;
				ready.push(ready.front());
				ready.pop();
				is_running = false;
				last_begin_time = cur_time;

				if(!ready.empty()){
				cout << cur_time << ": Running task " << ready.front().name << endl;
				is_running = true;
				last_begin_time = cur_time;
			}
			}
		}

		cur_time++;
	}

	cout << cur_time - 1 << ": All tasks finished" << endl;
	if(tasks.size() != 0){
		cout << "Average waiting time: " << waiting / tasks.size() << endl;
	}
}


