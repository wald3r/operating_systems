//Mutex

User time (seconds): 0.18
System time (seconds): 0.39


Works like a semaphore, but you can only allow one thread to enter the critical area. 
Mutex performs a Context Switch, when he wants to enter a critical, already blocked, area. 

The Time stays constant. 



//Spinlock

User time (seconds): 2.60
System time (seconds): 0.00

Spinlock is some sort of an agressive mutex. If an area is blocked by another thread, a mutex starts to wait (Context Switch). A Spinlock doesn't give up and keeps spinning, till he can enter the area. (active waiting)

The User Time jumps up and down. It probably increases when the queue is empty and the producer is at the very end of the waiting line. 

Spinlocks are more efficient, when the Context Switch takes more time than the active waiting, otherwhise not.  




//Mutex + conditions

Advantage: Through the conditions you make sure that useless threads don't blockade the critical area. 
Example: 4 Consumer + 1 Producer. It can happen when the Queue is empty, the other 3 Consumer will still try to empty it and the produces has to wait. With a conditions you can address the producer directly. 


User time (seconds): 0.19
System time (seconds): 0.37

The Time stays constant. 
