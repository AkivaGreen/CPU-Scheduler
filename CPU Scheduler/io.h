#pragma once
#ifndef IO_H
#define IO_H

#include<iostream>
#include "process.h"
#include "queue.h"

using namespace std;

class io {
public:
	io(); //Basic Constructor
	io(queue *& new_ready, int new_size); //Explicit Value Constructor
	~io(); //Destructor
	process & getData(int loc); //Get Process from dynamic process array
	void add(process *& org); //Add a Process to dynamic process array
	void remove(int loc); //Remove Process from dynamic process array
	bool isEmpty(); //Returns true if process dynamic array is empty
	bool isFull(); //Returns true if process dynamic array is full
	int search(); //Searches for Finished IO processes, returns -1 if it has not found any
	void decrement(); //Decrements IO Burst times for all processes in dynamic process array
	void decrement(bool & interrupt); //Decrements IO Burst times for all processes and flags interrupt
	void displayArray(); //Test Method to display dynamic array
	void printContextSwitch(); //Prints Context Switch Piece for IO

private:
	int count; //Number of Processes in Dynamic array
	int size; //Size of Dynamic Array
	process** io_list; //Dynamic pointer array of process pointers
	queue *ready; //Pointer to Ready Queue
};
#endif