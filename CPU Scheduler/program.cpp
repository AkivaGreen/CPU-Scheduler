#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "program.h"

using namespace std;

program::program() {
	count = 0;
	timeCounter = 0;
	size = 5;
	program_data = new process[size];
	ifstream in;
	in.open("program_data.txt");
	if (in.fail()) {
		cout << "The Input file failed to open correctly!\n";
		exit(1);
	}
	cout << "File Opened!\n";
	int tempInt;	
	while (!in.eof()) {
		if (isFull())
			double_size();
		in >> program_data[count].name;
		in.ignore(); //Ignore the space
		if (in.peek() == '{') {
			in.ignore();
		}
		while (in.peek() != '}') {
			if (in.peek() == ',') {
				in.ignore();
				in.ignore();
			}
			in >> tempInt;
			program_data[count].add(tempInt);
		}
		in.ignore();
		in.ignore();
		count++;
	}
	in.close();
}

//Copy Constructor
program::program(const program & org) {
	timeCounter = 0;
	count = org.count;
	size = org.size;
	program_data = new process[size];
	for (int i = 0; i < count; i++) {
		program_data[i].name = org.program_data[i].name;
		program_data[i].setWaiting(0);
		program_data[i].setTurnaround(0);
		program_data[i].setResponse(0);
		program_data[i].setCPUPos(0);
		program_data[i].setIOPos(1);
		program_data[i].setCPUTotal(0);
		program_data[i].setIOTotal(0);
		int org_count = org.program_data[i].getCount();
		for (int j = 0; j < org_count; j++) {
			program_data[i].add(org.program_data[i].getData(j));
		}
	}
}

int program::getCount() {
	return count;
}

double program::getAvgWaiting() {
	double avg_waiting = 0.0;
	for (int i = 0; i < count; i++) {
		avg_waiting += program_data[i].getWaiting();
	}
	avg_waiting /= count;
	return avg_waiting;
}

double program::getAvgTurnaround() {
	double avg_turnaround = 0.0;
	for (int i = 0; i < count; i++) {
		avg_turnaround += program_data[i].getTurnaround();
	}
	avg_turnaround /= count;
	return avg_turnaround;
}

double program::getAvgResponse() {
	double avg_response = 0.0;
	for (int i = 0; i < count; i++) {
		avg_response += program_data[i].getResponse();
	}
	avg_response /= count;
	return avg_response;
}

double program::getCpuUtilization() {
	double cpu_total = 0.0;
	double cpu_util;
	for (int i = 0; i < count; i++) {
		cpu_total += program_data[i].getCPUTotal();
	}
	cpu_util = (cpu_total / (double)timeCounter) * 100;
	return cpu_util;
}

int program::getSize() {
	return size;
}

int program::getTime() {
	return timeCounter;
}

void program::incrementTime() {
	timeCounter++;
}

bool program::isEmpty() {
	return count == 0;
}

bool program::isFull() {
	return count == size;
}

bool program::finished() {
	bool finished = true;
	for (int i = 0; i < count; i++) {
		if (!program_data[i].finished())
			finished = false;
	}
	return finished;
}

process & program::getData(int loc) {
	if (loc < 0 || loc > count) {
		cout << "Bad location value!\n";
		exit(1);
	}
	return program_data[loc];
}

void program::double_size() {
	size *= 2;
	process *temp = new process[size];

	for (int i = 0; i < count; i++)
	{
		temp[i].name = program_data[i].name;
		temp[i].setWaiting(program_data[i].getWaiting());
		temp[i].setTurnaround(program_data[i].getTurnaround());
		temp[i].setResponse(program_data[i].getResponse());
		temp[i].setCPUPos(program_data[i].getCPU());
		temp[i].setIOPos(program_data[i].getIO());
		temp[i].setCPUTotal(program_data[i].getCPUTotal());
		temp[i].setIOTotal(program_data[i].getIOTotal());
		int org_count = program_data[i].getCount();
		for (int j = 0; j < org_count; j++) {
			temp[i].add(program_data[i].getData(j));
		}
	}

	delete[] program_data;
	program_data = temp;
}

//Test Method to print dynamic array
void program::displayArray(){
	for (int i = 0; i < count; i++) {
		program_data[i].displayArray();
	}
}

//Prints context switch for program
void program::printContextSwitch(string prog_name, process *& currentPro, queue *& ready, io *& ioPros) {
	cout << "--------------------------------------------------------------------------" 
		<< "\n\n" << prog_name << "\n\n";
	cout << "Execution Time: " << timeCounter << endl << endl;
	currentPro->printCPUContextSwitch();
	ready->printContextSwitch();
	ioPros->printContextSwitch();
}

//Prints context switch for MLFQ
void program::printContextSwitch(string prog_name, process *& currentPro, queue *& ready,
	queue *& queue1, queue *& queue2, queue *& queue3, io *& ioPros) {
	cout << "--------------------------------------------------------------------------"
		<< "\n\n" << prog_name << "\n\n";
	cout << "Execution Time: " << timeCounter << endl << endl;
	currentPro->printCPUContextSwitch();
	ready->printContextSwitch();
	queue1->printContextSwitch();
	queue2->printContextSwitch();
	queue3->printContextSwitch();
	ioPros->printContextSwitch();
}

//Prints End of report for program when it has finished
void program::printEndReport(string prog_name) {
	cout << "--------------------------------------------------------------------------";
	cout << "\n\n" << prog_name << "\n\n";
	cout << "Total Execution Time: " << timeCounter << endl;
	cout << "CPU Utilization Rate: " << getCpuUtilization() << endl;
	cout << "Process Name:\t\tTw\t\tTtr\t\tTr\n";
	for (int i = 0; i < count; i++) {
		cout << program_data[i].name << ":\t\t\t" << program_data[i].getWaiting()
			<< "\t\t" << program_data[i].getTurnaround() << "\t\t"
			<< program_data[i].getResponse() << endl;
	}
	cout << "Averages:\t\t" << getAvgWaiting() << "\t\t"
		<< getAvgTurnaround() << "\t\t" << getAvgResponse() << endl << endl;
}

program::~program() {
	delete[] program_data;
}