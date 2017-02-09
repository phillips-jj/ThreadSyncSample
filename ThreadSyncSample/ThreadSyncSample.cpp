// ThreadSyncSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;

#define NUM_THREADS 20
HANDLE* gEvent;


/*
This sample code demonstrates thread synchronization using multiple different synchronization objects (specifically thread handles and events).  It
spawns 20 threads which each print out when they start and when they finish.  One thread will set an event, while the main thread (which spawned all the
worker threads) waits for all 20 threads to complete, as well as for the event to be signalled.

This code also highlights the problems which can occur if threads are not synchronized.  The console is not thread-safe - when printing a message and the
worker thread's ID (as below), the ID will sometimes be printed after a bunch of other messages (run the sample a few times to see this issue at least once).

*/


void DoSomeWork(void* parameters)
{
	int id = *(int*)parameters;		//extract our ID from the void*
	cout << "Starting worker thread " << id << endl;
	Sleep(500);
	if (id == 17)	//let's just say that the 17th thread signals this event
	{
		cout << "Setting event on thread " << id << endl;
		
		//removing this line will result in a timeout, because the main thread is waiting for all objects to be signalled, and the event will never be
		SetEvent(*gEvent);	
	}
	
	cout << "Finished worker thread " << id << endl;
	_endthread();
}


int main()
{
	int ids[NUM_THREADS];					//array which holds the IDs for the threads - see why below
	HANDLE waitObjects[NUM_THREADS + 1];	//we're going to wait on 21 different objects - 20 threads plus 1 event
	
	gEvent = waitObjects + NUM_THREADS;
	*gEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	//no security, manual reset, unsignalled, unnamed

	for (int i = 0; i < NUM_THREADS; i++)
	{
		/*
			One might be tempted to just pass the address of i to the thread.  That is not thread-safe, as the value of i could change between the time
			it is passed to the worker thread, and the time which the worker thread reads it.  So we put each ID in an array element, and have each worker
			thread read theirs.
		*/
		ids[i] = i;

		waitObjects[i] = (HANDLE)_beginthread(DoSomeWork, 0, ids+i);	
	}

	DWORD dwWaitStatus = WaitForMultipleObjects(NUM_THREADS + 1, waitObjects, TRUE, 5000L); //wait for all 21 objects, for max 5 seconds
	switch (dwWaitStatus)
	{
	case WAIT_OBJECT_0:
		// Since the 3rd parameter of WaitForMultipleObjects is TRUE (i.e. wait for all objects to become signalled), this is the return value
		// which signifies all objects are signalled.
		cout << "Success!" << endl;
		break;
	case WAIT_TIMEOUT:
		cout << "Timeout!" << endl;
		break;
	default:
		cout << "Some unexpected value was returned from WaitForMultiple objects - " << dwWaitStatus << endl;
		break;
	}

	system("pause");
	return 0;
}

