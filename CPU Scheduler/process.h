#pragma once
#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>

using namespace std;

class process {
public:
	process();
	process(const process & org);
	~process();
	string name;
	int getData(int loc);
	int getCount();
	void setCount(int new_count);
	int getSize();
	void setSize(int new_size);
	int getCPU(); //Get the current cpu_pos
	void setCPUPos(int new_cpu_pos);
	int getIO(); //Get the current io pos
	void setIOPos(int new_io_pos);
	int getCPUTotal(); 
	void setCPUTotal(int new_cpu_total);
	int getIOTotal(); 
	void setIOTotal(int new_io_total);
	void add(int data);
	int getWaiting(); 
	void setWaiting(int new_waiting);
	int getTurnaround(); 
	void setTurnaround(int new_turnaround); 
	int getResponse(); 
	void setResponse(int time); 
	bool isEmpty(); 
	bool isFull(); 
	bool isBurst(int loc); //Checks location value if it is burst location
	bool isIO(int loc); //Checks location value if it is io location
	bool finished(); //Returns true if process is finished (All values in array is 0)
	void incrementWaiting(); //Increments Total Waiting Time
	void incrementCPU(); //Increments Total CPU Time
	void incrementCPUPos(); //Increments CPU Array Position Value
	void incrementIO(); //Increments Total IO Time
	void incrementIOPos(); //Increments IO Array Position Value
	void decrement(int loc); //Decrement value at location
	void double_size(); 
	void displayArray(); //Test Method to Display Dynamic Array
	void printCPUContextSwitch(); //Used to Print Current Process
	void printIOContextSwitch(); //Used to Print IO Value of Process

private:
	int waiting; //Waiting Time
	int turnaround; //Turnaround Time
	int response; //Response time
	int cpu_total; //Total CPU Time
	int io_total; //Total IO Time
	int cpu_pos; //Current CPU Burst Location
	int io_pos; //Current IO Burst Location
	int count; //Number of Elements in Dynamic Array
	int size; //Size of Dynamic Array
	int *process_data; //Array of integer values that hold CPU and IO Burst Times
};

#endif