#include "Task.h"
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;

struct Node{
	Task data;
	Node *next;
};
struct is_less {
	bool operator() (const Task& lhs, const Task& rhs) {
		return lhs.arrival < rhs.arrival;
	}
};

/*
 * Implement this function using the Priority Round Robin algorithm in Part 3.
 */
void schedule(std::vector<Task> tasks, int quantum) {
	if(quantum <= 0 || tasks.size() == 0){
		cout << "Invalid parameter." << endl;
		return;
	}

	sort(tasks.begin(), tasks.end(), is_less());
	
	Node* head = NULL;
	int ind = 0;
	int cur_time = 0;
	int last_begin_time = 0;
	float waiting = 0;
	bool is_running = false;
	bool pre_empted = false;

	while(ind < tasks.size() || head != NULL){
		Node* pre_empted_node;
		//insert into the linked list
		while(ind < tasks.size() && tasks[ind].arrival == cur_time){
			//if the linked list is empty
			if(head == NULL){
				head = new (struct Node);
				head->data = tasks[ind];
				head->next = NULL;
				cout << cur_time << ": Adding task " << tasks[ind].name << endl;
			}
			//if it's executing a lower priority and the current executing has not been interrupted
			else if(tasks[ind].priority > head->data.priority){
				cout << cur_time << ": Adding task " << tasks[ind].name << endl;
				//markdown it's interrupted
				if(is_running){
					is_running = false;
					pre_empted_node = head;
					pre_empted = true;
				}
				
				//create a new node
				Node* add = new(struct Node);
				add->data = tasks[ind];
				add->next = head;
				head = add;
				head->next->data.remaining = head->next->data.remaining - (cur_time - last_begin_time);
				Node* temp = head->next;
				//if the current process happened to finish at this moment
				if(head->next->data.remaining == 0){
					cout << cur_time <<": Finished task " << head->next->data.name << endl;
					waiting += (cur_time - head->next->data.time - head->next->data.arrival);
					head->next = temp->next;
					delete temp;
					pre_empted = false;
				}
				
			}
			//its priority is not higher than the executing one or the current executing cannot be interrupted
			else{
				Node* cur = head;
				while(cur->next != NULL && tasks[ind].priority <= cur->next->data.priority){
					cur = cur->next;
				}
				Node* add = new(struct Node);
				add->data = tasks[ind];
				add->next = cur->next;
				cur->next = add;
				cout << cur_time << ": Adding task " << tasks[ind].name << endl;

			}
			ind++;
		}

		if(pre_empted){
			pre_empted = false;
			cout << cur_time << ": Pre-empting task " << pre_empted_node->data.name << endl;
			Node* cur = head;
			while(cur->next != pre_empted_node){
				cur = cur->next;
			}
			cur->next = pre_empted_node->next;
			cur = head;
			while(cur->next != NULL && pre_empted_node->data.priority <= cur->next->data.priority){
				cur = cur->next;
			}
			pre_empted_node->next = cur->next;
			cur->next = pre_empted_node;
		}

		//execute the head
		if(!is_running && head != NULL){
			cout << cur_time << ": Running task " << head->data.name << endl;
			is_running = true;
			last_begin_time = cur_time;
		}
		//finished or slice done
		if(head != NULL){
			//check if the current executing is finished
			if(cur_time == last_begin_time + head->data.remaining){
				cout << cur_time << ": Finished task " << head->data.name << endl;
				waiting += (cur_time - head->data.time - head->data.arrival);
				Node* temp = head;
				head = head->next;
				is_running = false;
				//if there is a following one to be executed
				if(head != NULL){
					cout << cur_time << ": Running task " << head->data.name << endl;
					is_running = true;
					last_begin_time = cur_time;
				}
				delete temp;
			}
			//it's not finished but slice done
			else if(cur_time == last_begin_time + quantum){
				cout << cur_time << ": Time slice done for task " << head->data.name << endl;
				head->data.remaining = head->data.remaining - quantum;
				
				//if it's the only process at its priority
				if(head->next == NULL || head->next->data.priority < head->data.priority){
					cout << cur_time << ": Running task " << head->data.name << endl;
					last_begin_time = cur_time;
				}
				else{
					Node* temp = head;
					cout << cur_time << ": Running task " << head->next->data.name << endl;
					//push it back to the queque
					Node* cur = head;
					head = head->next;
					last_begin_time = cur_time;
					while(cur->next != NULL && cur->data.priority <= cur->next->data.priority){
						cur = cur->next;
					}
					temp->next = cur->next;
					cur->next = temp;
					}
				
			}
		}
		cur_time++;
	}
	cout << cur_time - 1 << ": All tasks finished" << endl;

	if(tasks.size() != 0){
		cout << "Average waiting time: " << waiting / tasks.size() << endl;
	}

	Node* prev = head;

	while(prev != NULL){
		head = head->next;
		delete prev;
		prev = head;
	}

}
