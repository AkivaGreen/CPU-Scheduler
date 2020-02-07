#include <iostream>
#include <string>
#include "process.h"

using namespace std;

//Default Constructor
process::process() {
	waiting = 0;
	turnaround = 0;
	cpu_total = 0;
	io_total = 0;
	response = 0;
	cpu_pos = 0;
	io_pos = 1;
	count = 0;
	size = 5;
	process_data = new int[size];
}

process::process(const process & org) {
	waiting = org.waiting;
	turnaround = org.turnaround;
	cpu_total = org.cpu_total;
	io_total = org.io_total;
	response = org.response;
	cpu_pos = org.cpu_pos;
	io_pos = org.io_pos;
	count = org.count;
	size = org.size;
	name = org.name;
	process_data = new int[size];
	for (int i = 0; i < count; i++) {
		int temp = org.process_data[i];
		process_data[i] = temp;
	}
}

int process::getCount() {
	return count;
}

void process::setCount(int new_count) {
	count = new_count;
}

int process::getData(int loc) {
	if (loc < 0 || loc > count) {
		cout << "Bad location value!\n";
		return -1;
	}
	return process_data[loc];
}

int process::getSize() {
	return size;
}

void process::setSize(int new_size) {
	size = new_size;
}

int process::getCPU() {
	return cpu_pos;
}

void process::setCPUPos(int new_cpu_pos) {
	cpu_pos = new_cpu_pos;
}

int process::getIO() {
	return io_pos;
}

void process::setIOPos(int new_io_pos) {
	io_pos = new_io_pos;
}

int process::getCPUTotal() {
	return cpu_total;
}

void process::setCPUTotal(int new_cpu_total) {
	cpu_total = new_cpu_total;
}

int process::getIOTotal() {
	return io_total;
}

void process::setIOTotal(int new_io_total) {
	io_total = new_io_total;
}

int process::getWaiting() {
	return waiting;
}

void process::setWaiting(int new_waiting) {
	waiting = new_waiting;
}

int process::getTurnaround() {
	return (waiting + cpu_total + io_total);
}

void process::setTurnaround(int new_turnaround) {
	turnaround = new_turnaround;
}

int process::getResponse() {
	return response;
}

void process::setResponse(int time) {
	response = time;
}


void process::add(int data) {
	if (isFull())
		double_size();
	process_data[count] = data;
	count++;
}

bool process::isEmpty() {
	return count == 0;
}

bool process::isFull() {
	return count == size;
}

bool process::isBurst(int loc) {
	if (loc < 0 || loc > count) {
		cout << "Bad location value!\n";
		exit(1);
	}
	return (loc % 2 == 0);
}

bool process::isIO(int loc) {
	if (loc < 0 || loc > count) {
		cout << "Bad location value!\n";
		exit(1);
	}
	return (loc % 2 == 1);
}

bool process::finished() {
	bool finished = true;
	for(int i = 0; i < count; i++){
		if (process_data[i] > 0) {
			finished = false;
		}
	}
	return finished;
}

void process::incrementWaiting() {
	waiting++;
}

void process::incrementCPU() {
	cpu_total++;
}

void process::incrementCPUPos() {
	cpu_pos += 2;
}

void process::incrementIO() {
	io_total++;
}

void process::incrementIOPos() {
	io_pos += 2;
}

void process::decrement(int loc) {
	if (loc < 0 || loc > count) {
		cout << "Bad location value!\n";
		return;
	}
	process_data[loc]--;
}

void process::double_size() {
	size *= 2;
	int *temp = new int[size];

	for (int i = 0; i < count; i++)
	{
		temp[i] = process_data[i];
	}

	delete[] process_data;
	process_data = temp;
}

//Test Method to see the current Array of values
void process::displayArray() {
	cout << name << ":\n";
	for (int i = 0; i < count - 1; i++) {
		cout << process_data[i] << ", ";
	}
	cout << process_data[count-1] << endl;
}

//Prints Context Switch for CPU Burst
void process::printCPUContextSwitch() {
	if (finished()) {
		cout << "Current Process:\n";
		cout << name << ":\t" << "Finished!\n\n";
	}
	else {
		cout << "Current Process:\n";
		cout << name << ":\t" << process_data[getCPU()] << endl << endl;
	}
}

//Prints Context Switch for IO Burst
void process::printIOContextSwitch() {
	cout << name << ":\t" << process_data[getIO()] << endl;
}

process::~process() {
	delete[] process_data;
}