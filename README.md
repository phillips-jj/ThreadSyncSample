# ThreadSyncSample
This sample code demonstrates thread synchronization using multiple different synchronization objects (specifically thread 
handles and events).  It spawns 20 threads which each print out when they start and when they finish.  One thread will 
set an event, while the main thread (which spawned all the worker threads) waits for all 20 threads to complete, as well 
as for the event to be signalled.

This code also highlights the problems which can occur if threads are not synchronized.  The console is not thread-safe - 
when printing a message and the worker thread's ID (as below), the ID will sometimes be printed after a bunch of other 
messages (run the sample a few times to see this issue at least once).
