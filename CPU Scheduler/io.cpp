#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "io.h"

using namespace std;

io::io() {
	count = 0;
	size = 10;
	io_list = new process*[size];
}

io::io(queue *& new_ready, int new_size) {
	count = 0;
	size = new_size;
	io_list = new process*[size];
	ready = new_ready;
}

process & io::getData(int loc) {
	if (loc < 0 || loc > count) {
		cout << "Bad location value!\n";
		exit(1);
	}
	return *io_list[loc];
}

//Add process to dynamic array
void io::add(process *& org) {
	io_list[count] = org;
	count++;
}

//Remove process from dynamic array at 'loc'
void io::remove(int loc) {
	for (int i = loc; i < count - 1; i++) {
		io_list[i] = io_list[i + 1];
	}
}

bool io::isEmpty() {
	return count == 0;
}

bool io::isFull() {
	return count == size;
}

//Searches array for process that has finished its IO burst
int io::search() {
	int finished_loc = -1;
	for (int i = 0; i < count; i++) {
		int io_loc = io_list[i]->getIO();
		if (io_list[i]->getData(io_loc) == 0) {
			finished_loc = i;
			break;
		}
	}
	return finished_loc;
}

//Decrements the IO bursts of all process in the dynamic array
void io::decrement() {
	int io_loc = search();
	while (io_loc != -1) {
		process *temp = io_list[io_loc];
		ready->enqueue(temp);
		io_list[io_loc]->incrementCPUPos();
		io_list[io_loc]->incrementIOPos();
		remove(io_loc);
		count--;
		io_loc = search();
	}
	for (int i = 0; i < count; i++) {
		io_loc = io_list[i]->getIO();
		io_list[i]->incrementIO();
		io_list[i]->decrement(io_loc);
	}
}

//Decrements the IO bursts of all process in the dynamic array
//Also sets interrupt to true if there are IO bursts that have finished
void io::decrement(bool & interrupt) {
	int io_loc = search();
	if(!interrupt && io_loc != -1)
		interrupt = true;
	while (io_loc != -1) {
		process *temp = io_list[io_loc];
		ready->enqueue(temp);
		io_list[io_loc]->incrementCPUPos();
		io_list[io_loc]->incrementIOPos();
		remove(io_loc);
		count--;
		io_loc = search();
	}
	for (int i = 0; i < count; i++) {
		io_loc = io_list[i]->getIO();
		io_list[i]->incrementIO();
		io_list[i]->decrement(io_loc);
	}
}

//Test method to display array
void io::displayArray() {
	for (int i = 0; i < count; i++) {
		io_list[i]->displayArray();
	}
}

//Prints the Context Switch for IO Processes
void io::printContextSwitch() {
	cout << "IO Processes:\n";
	if (!isEmpty()) {
		for (int i = 0; i < count; i++) {
			io_list[i]->printIOContextSwitch();
		}
		cout << endl << endl;
	}
	else {
		cout << "Empty!\n\n";
	}
}

io::~io() {
	delete[] io_list;
}