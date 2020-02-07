#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include<iostream>
#include "process.h"
#include "io.h"
#include "queue.h"

using namespace std;

class program {
public:
	program();
	program(const program & org); //Copy Constructor
	~program();
	int getCount();
	int getSize();
	int getTime();
	process & getData(int loc); //Gets process object from dynamic array at the 'loc' location
	double getAvgWaiting();
	double getAvgTurnaround();
	double getAvgResponse();
	double getCpuUtilization(); 
	void incrementTime(); //Increments the timeCounter
	bool isEmpty();
	bool isFull();
	bool finished(); //When All Process Have Finished
	void double_size();
	void displayArray(); //Prints out the Array to Be Viewed
	//Prints report on context switch
	void printContextSwitch(string prog_name, process *& currentPro, queue *& ready, io *& ioPros);
	void printContextSwitch(string prog_name, process *& currentPro, queue *& ready,
		queue *& queue1, queue *& queue2, queue *& queue3, io *& ioPros);
	void printEndReport(string prog_name); //Prints Report for End of Simulation

private:
	int timeCounter;
	int count;
	int size;
	process *program_data; //A Dynamic array of processes
};

#endif