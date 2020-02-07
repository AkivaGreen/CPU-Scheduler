/*
Name: Akiva Green
Z#: Z23336001
Class: COP4601 - Computer Operating Systems
Due Date: October 8th, 2019

Assignment: CPU Scheduler
Description:
	This program simulates a CPU when given a list a processes as a text file. It reads the processes,
	saves it into dynamic arrays, then models them as a FCFS (non-preemptive), SJF (non-preemptive),
	Multilevel Feedback Queue (Q1-RR with Tq=5; Q2-RR with Tq=10; Q3-FCFS).
*/

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "program.h" //We will be calling a collections of processes a program for simplicity sake
#include "process.h" //Processes with their CPU Burst and IO Burst times
#include "queue.h" //Ready Queue made from a linked list. 
#include "io.h" //A List of Process that are in I/O

using namespace std;

void fcfs(program fcfs_prog, queue ready);
void sjf(program sjf_prog, queue ready);
void mlfq(program mlfq_prog, queue ready);
void halt();

int main() {
	program main_program;
	queue ready("Ready Queue", 0, main_program.getSize()+1);
	bool exit = false;
	int response;
	while (!exit) {
		system("CLS");
		cout << "Which program would you like to run?\n"
			<< "Please select from one of the following(1, 2, 3):\n"
			<< "1 - First Come First Serve (FCFS)\n"
			<< "2 - Shortest Job First (SJF)\n"
			<< "3 - Multilevel Feedback Queue (MLFQ)\n"
			<< "4 - Exit Program\n"
			<< "Please Enter Your Response: ";
		cin >> response;
		switch (response) {
		case 1:
			fcfs(main_program, ready);
			break;
		case 2:
			sjf(main_program, ready);
			break;
		case 3:
			mlfq(main_program, ready);
			break;
		default:
			exit = true;
			break;
		}
	}
}

void fcfs(program fcfs_prog, queue ready) {
	int count = fcfs_prog.getCount();
	int cpu_burst_loc;
	queue *readyPtr = &ready;
	process *fcfs_cpu;
	io fcfs_io(readyPtr, fcfs_prog.getSize() + 1);
	io *ioPtr = &fcfs_io;
	for (int i = 0; i < count; i++) {
		process *temp = &fcfs_prog.getData(i);
		ready.enqueue(temp);
	}
	//Simulation Starts
	while(!fcfs_prog.finished()) {
		fcfs_cpu = ready.dequeue(); //Get Process From Ready Queue
		if (fcfs_cpu == NULL) {
			fcfs_prog.incrementTime();
			fcfs_io.decrement();
		}
		else {
			cpu_burst_loc = fcfs_cpu->getCPU(); //Gets Next CPU Burst Location
			if (cpu_burst_loc == 0) {
				fcfs_cpu->setResponse(fcfs_prog.getTime());
			}
			while (fcfs_cpu->getData(cpu_burst_loc) > 0) {
				fcfs_cpu->decrement(cpu_burst_loc); //Decrements CPU Burst Time for Current CPU Process
				fcfs_io.decrement(); //Decrements IO Burst Time for All Processes in IO
											//Also Moves Finished IO Bursts to Ready Queue
				ready.incrementWaiting(); //Increments Waiting Time for All Processes in Ready Queue
				fcfs_cpu->incrementCPU(); //Increments Total CPU Time for Process
				fcfs_prog.incrementTime(); //Increments the time of the running program.
			}
			if (!fcfs_cpu->finished()) {
				fcfs_io.add(fcfs_cpu); //Add the Process to IO if there is still More Left
			}
			fcfs_prog.printContextSwitch("FCFS", fcfs_cpu, readyPtr, ioPtr);
		}
	}
	fcfs_prog.printEndReport("FCFS");
	halt();
}

void sjf(program sjf_prog, queue ready) {
	int count = sjf_prog.getCount();
	int cpu_burst_loc;
	queue *readyPtr = &ready;
	process *sjf_cpu;
	io sjf_io(readyPtr, sjf_prog.getSize() + 1);
	io *ioPtr = &sjf_io;
	for (int i = 0; i < count; i++) {
		process *temp = &sjf_prog.getData(i);
		ready.enqueue(temp);
	}
	//Simulation Starts
	while (!sjf_prog.finished()) {
		ready.bubble_sort(); //This line orders the processes from shortest(front) to longest(back) burst time
		sjf_cpu = ready.dequeue(); //Get Process From Ready Queue
		if (sjf_cpu == NULL) {
			sjf_prog.incrementTime();
			sjf_io.decrement();
		}
		else {
			cpu_burst_loc = sjf_cpu->getCPU(); //Gets Next CPU Burst Location
			if (cpu_burst_loc == 0) {
				sjf_cpu->setResponse(sjf_prog.getTime());
			}
			while (!sjf_cpu->finished() && sjf_cpu->getData(cpu_burst_loc) > 0) {
				sjf_cpu->decrement(cpu_burst_loc); //Decrements CPU Burst Time for Current CPU Process
				sjf_io.decrement(); //Decrements IO Burst Time for All Processes in IO
											//Also Moves Finished IO Bursts to Ready Queue
				ready.incrementWaiting(); //Increments Waiting Time for All Processes in Ready Queue
				sjf_cpu->incrementCPU(); //Increments Total CPU Time for Process
				sjf_prog.incrementTime(); //Increments the time of the running program.
			}
			if (!sjf_cpu->finished()) {
				sjf_io.add(sjf_cpu); //Add the Process to IO if there is still More Left
			}
			sjf_prog.printContextSwitch("SJF", sjf_cpu, readyPtr, ioPtr);
		}
	}
	sjf_prog.printEndReport("SJF");
	halt();
}

void mlfq(program mlfq_prog, queue ready) {
	int count = mlfq_prog.getCount(); //Get Number of Processes in Program
	int Tq, cpu_burst_loc;
	bool interrupt = false;
	queue *readyPtr = &ready;
	queue queue1("Queue 1", 5, mlfq_prog.getSize()+1), queue2("Queue 2", 10, mlfq_prog.getSize()+1), 
		queue3("Queue 3", 0, mlfq_prog.getSize()+1);
	queue *q1Ptr = &queue1, *q2Ptr = &queue2, *q3Ptr = &queue3;
	process *mlfq_cpu;
	io mlfq_io(readyPtr, mlfq_prog.getSize() + 1);
	io *ioPtr = &mlfq_io;
	//Load the ready queue with processes
	for (int i = 0; i < count; i++) {
		process *temp = &mlfq_prog.getData(i);
		ready.enqueue(temp);
	}
	//Simulation Starts
	while (!mlfq_prog.finished()) {
		//If There is Nothing in Any Queue Continue To Decrement IO and Increment Time
		if (ready.peek() == NULL && queue1.peek() == NULL &&
			queue2.peek() == NULL && queue3.peek() == NULL) {
			mlfq_prog.incrementTime();
			mlfq_io.decrement();
		}
		//Resets Interrupt to False if There are elements in ready queue
		if (ready.peek() != NULL && interrupt)
			interrupt = false; 
		//Load Queue1 with Processes from Ready Queue
		while (ready.peek() != NULL) {
			process *temp = ready.dequeue();
			queue1.enqueue(temp);
		}
		while (queue1.peek() != NULL && !interrupt) {
			//Sets Time Quantum Value
			Tq = queue1.getTq(); 
			//Running Process set To Process object from Queue 1
			mlfq_cpu = queue1.dequeue(); 
			//Get Current CPU Burst array location
			cpu_burst_loc = mlfq_cpu->getCPU(); 
			//If CPU Burst Array Location = 0 Then Set the Response Time
			if (cpu_burst_loc == 0) { 
				mlfq_cpu->setResponse(mlfq_prog.getTime());
			}
			//Loop for Running Process with Time Quantum Limit
			for (int i = 0; i < Tq; i++) {
				//If Current CPU Burst is finished we break the loop
				if (mlfq_cpu->getData(cpu_burst_loc) == 0)
					break;
				mlfq_cpu->decrement(cpu_burst_loc); //Decrements CPU Burst Time for Current CPU Process
				mlfq_io.decrement(interrupt); //Decrements IO Burst Time for All Processes in IO
											//Also Moves Finished IO Bursts to Ready Queue
											//Sets bool value interrupt When Move IO to Ready Queue
				//Increments Waiting Time for All Processes in Every Queue
				ready.incrementWaiting();
				queue1.incrementWaiting();
				queue2.incrementWaiting();
				queue3.incrementWaiting();
				mlfq_cpu->incrementCPU(); //Increments Total CPU Time for Process
				mlfq_prog.incrementTime(); //Increments the time of the running program.
			}
			if (!mlfq_cpu->finished() && mlfq_cpu->getData(cpu_burst_loc) > 0) {
				queue2.enqueue(mlfq_cpu); //Added the process to Queue 2 Since it has yet to finish
			}
			else if(!mlfq_cpu->finished()) {
				mlfq_io.add(mlfq_cpu); //Add the Process to IO if there is still More Left
			}
			mlfq_prog.printContextSwitch("MLFQ", mlfq_cpu, readyPtr, q1Ptr, q2Ptr, q3Ptr, ioPtr);
		}
		while (queue2.peek() != NULL && !interrupt) {
			//Set Time Quantum
			Tq = queue2.getTq();
			//Get Process from Queue 2
			mlfq_cpu = queue2.dequeue();
			//Get CPU Bust Array Location of current process
			cpu_burst_loc = mlfq_cpu->getCPU();
			//Loop for Running Process with Time Quantum Limit
			for (int i = 0; i < Tq; i++) {
				if (mlfq_cpu->getData(cpu_burst_loc) == 0)
					break;
				mlfq_cpu->decrement(cpu_burst_loc); //Decrements CPU Burst Time for Current CPU Process
				mlfq_io.decrement(interrupt); //Decrements IO Burst Time for All Processes in IO
											//Also Moves Finished IO Bursts to Ready Queue
											//Sets bool value interrupt When Move IO to Ready Queue
				ready.incrementWaiting(); //Increments Waiting Time for All Processes in Every Queue
				queue1.incrementWaiting();
				queue2.incrementWaiting();
				queue3.incrementWaiting();
				mlfq_cpu->incrementCPU(); //Increments Total CPU Time for Process
				mlfq_prog.incrementTime(); //Increments the time of the running program.
			}
			if (!mlfq_cpu->finished() && mlfq_cpu->getData(cpu_burst_loc) > 0) {
				queue3.enqueue(mlfq_cpu); //Added the process to Queue 3 Since it has yet to finish
			}
			else if (!mlfq_cpu->finished()) {
				mlfq_io.add(mlfq_cpu); //Add the Process to IO if there is still More Left
			}
			mlfq_prog.printContextSwitch("MLFQ", mlfq_cpu, readyPtr, q1Ptr, q2Ptr, q3Ptr, ioPtr);
		}
		while (queue3.peek() != NULL && !interrupt) {
			mlfq_cpu = queue3.dequeue();
			cpu_burst_loc = mlfq_cpu->getCPU();
			while (mlfq_cpu->getData(cpu_burst_loc) > 0) {
				mlfq_cpu->decrement(cpu_burst_loc); //Decrements CPU Burst Time for Current CPU Process
				mlfq_io.decrement(interrupt); //Decrements IO Burst Time for All Processes in IO
											//Also Moves Finished IO Bursts to Ready Queue
											//Sets bool value interrupt When Move IO to Ready Queue
				ready.incrementWaiting(); //Increments Waiting Time for All Processes in Every Queue
				queue1.incrementWaiting();
				queue2.incrementWaiting();
				queue3.incrementWaiting();
				mlfq_cpu->incrementCPU(); //Increments Total CPU Time for Process
				mlfq_prog.incrementTime(); //Increments the time of the running program.
			}
			if (!mlfq_cpu->finished()) {
				mlfq_io.add(mlfq_cpu); //Add the Process to IO if there is still More Left
			}
			mlfq_prog.printContextSwitch("MLFQ", mlfq_cpu, readyPtr, q1Ptr, q2Ptr, q3Ptr, ioPtr);
		}
	}
	mlfq_prog.printEndReport("MLFQ");
	halt();
}

void halt() {
	char any_key;
	cout << "Enter any key to continue: ";
	cin >> any_key;
}