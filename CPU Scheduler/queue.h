#pragma once
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "process.h"

using namespace std;

class queue {
public:
	queue();
	queue(string new_name, int new_tq, int new_size);
	queue(const queue & org);
	~queue();
	void enqueue(process *& value);
	process* dequeue();
	process* peek(); //Peek at the next process to be dequeued
	int getCount();
	int getSize();
	int getTq();
	bool isFull();
	bool isEmpty();
	void bubble_sort(); //Orders the processes from shortest(front) to longest(back) cpu burst time
	void incrementWaiting(); //Increments waiting time for all elements in dynamic array
	void displayArray(); //Test Method to print dynamic array
	void printContextSwitch(); //Print Queue (Ready) for context switch

private:
	string name;
	int Tq;
	int count;
	int size;
	process** q; //Dynamic pointer array of process pointers
};
#endif