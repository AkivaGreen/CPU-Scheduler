/*
This Class is for the purpose of having a ready queue that will have processes going into and out from it. 
Due to this there needs to be pointers to avoid copying the values.
*/
#include<iostream>
#include<string>
#include "queue.h"

using namespace std;

queue::queue() {
	name = "";
	Tq = 5;
	count = 0;
	size = 10;
	q = new process*[size];
}

queue::queue(const queue & org) {
	name = org.name;
	Tq = org.Tq;
	count = org.count;
	size = org.size;
	q = new process*[size];
	for (int i = 0; i < count; i++) {
		q[i]->name = org.q[i]->name;
		q[i]->setWaiting(org.q[i]->getWaiting());
		q[i]->setTurnaround(org.q[i]->getTurnaround());
		q[i]->setResponse(org.q[i]->getResponse());
		q[i]->setCPUPos(org.q[i]->getCPU());
		q[i]->setIOPos(org.q[i]->getIO());
		q[i]->setCPUTotal(org.q[i]->getCPUTotal());
		q[i]->setIOTotal(org.q[i]->getIOTotal());
		for (int j = 0; j < org.q[i]->getCount(); j++) {
			q[i]->add(org.q[i]->getData(j));
		}
	}
}

queue::queue(string new_name, int new_tq, int new_size) {
	name = new_name;
	Tq = new_tq;
	count = 0;
	size = new_size;
	q = new process*[size];
}

void queue::enqueue(process *& data) {
	q[count] = data;
	count++;
}

process* queue::dequeue() {
	if (isEmpty())
		return NULL;
	else {
		process *value = q[0];
		for (int i = 0; i < count - 1; i++) {
			q[i] = q[i + 1];
		}
		count--;
		return value;
	}
}

process* queue::peek() {
	if (isEmpty())
		return NULL;
	else
		return q[0];
}

int queue::getCount() {
	return count;
}

int queue::getSize() {
	return size;
}

int queue::getTq() {
	return Tq;
}

bool queue::isEmpty() {
	return count == 0;
}

bool queue::isFull() {
	return count == size;
}

void queue::bubble_sort() {
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count-i-1; j++) {
			if (q[j]->getData(q[j]->getCPU()) >
				q[j + 1]->getData(q[j + 1]->getCPU())) {
				process *temp = q[j];
				q[j] = q[j + 1];
				q[j + 1] = temp;
			}
		}
	}

}

//Increments waiting time for all processes in queue
void queue::incrementWaiting() {
	for (int i = 0; i < count; i++) {
		q[i]->incrementWaiting();
	}
}

//Test method to see all the elements in the dynamic array
void queue::displayArray() {
	for (int i = 0; i < count; i++) {
		q[i]->displayArray();
	}
}

//Prints Context Switch for all elements in the queue
void queue::printContextSwitch() {
	cout << name << endl;
	if (!isEmpty()) {
		for (int i = 0; i < count; i++) {
			cout << q[i]->name << ":\t" << q[i]->getData(q[i]->getCPU())
				<< endl;
		}
		cout << endl;
	}
	else {
		cout << "Empty!\n\n";
	}
}

queue::~queue() {
	delete[] q;
}