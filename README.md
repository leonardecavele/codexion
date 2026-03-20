_This project has been created as part of the 42 curriculum by ldecavel._

### DESCRIPTION

Codexion aims to teach multithreading and the use of synchronization tools such as mutexes to handle its main challenges correctly.
These challenges include race conditions, deadlocks, and starvation. The program is based on a set of limited shared resources, called dongles, 
which must be safely coordinated between several coder threads. A coder must acquire two dongles in order to compile. After compiling, the coder 
releases both dongles and continues with debugging and refactoring. Once this cycle is complete, the coder immediately tries to acquire two dongles 
again and repeats the process until the required number of compilations has been completed. If a coder does not manage to compile again before the burnout 
deadline, the coder burns out and the program terminates.

### INSTRUCTIONS

To build the program :
```bash
make | make install
```

To delete the build directory :
```bash
make clean
```

To delete both the program and the build directory :
```bash
make fclean
```

To rebuild the program :
```bash
make re
```

### EXAMPLE USAGE

```
Usage:
  ./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler [debug]

Arguments:
  number_of_coders            number of coders
  time_to_burnout             time before burnout (ms)
  time_to_compile             time compiling (ms)
  time_to_debug               time debugging (ms)
  time_to_refactor            time refactoring (ms)
  number_of_compiles_required number of compiles required
  dongle_cooldown             dongle cooldown (ms)
  scheduler                   fifo|edf
  [debug]                     enable extra debug logs
```

### BLOCKING CASES HANDLED

Deadlocks are prevented by only allowing a coder to take both dongles when both are available, cooled down, and authorized by the scheduling policy, so a thread never keeps
one resource while waiting indefinitely for the second. Starvation is reduced through a waiting queue with explicit priorities, using either FIFO order or an urgency-based
policy based on each coder’s burnout deadline. Dongle cooldown is checked before reuse, which prevents invalid immediate reacquisition. Burnout is monitored by a dedicated 
thread that compares each coder’s last compile time against the burnout timeout and stops the simulation as soon as a coder expires. Finally, log messages are protected 
by a print mutex so outputs remain serialized and readable.

### THREAD SYNCHRONIZATION MECHANISMS

My implementation mainly relies on pthread_mutex_t to protect shared data and ensure that only one thread can modify critical state at a time. Different mutexes are used 
for different responsibilities, including dongle access, logging, queue management, and monitor-related values such as last_compile and the global stop flag.

The program also uses a pthread_cond_t to handle waiting in a safe and efficient way. When a coder cannot continue because the required dongles are not available, 
the thread is blocked on the condition variable instead of busy-waiting. It is then awakened when resources are released or when the simulation ends.

Shared resources must be accessed in a controlled way because several coder threads may try to use them at the same time. Dongle availability, queue state, 
and monitor flags are therefore always checked and updated under the appropriate mutex. This guarantees that resource ownership remains consistent throughout the execution.

Race conditions are prevented by locking the correct mutex before reading or modifying shared values. This applies to resource state, logging, and monitor data. 
As a result, two coders cannot safely claim the same dongles at the same time, and log messages remain readable and ordered.

Communication between coder threads and the monitor is achieved through protected shared flags and synchronized wake-ups. The monitor can safely detect
whether a coder has burned out or whether the simulation must stop, while coder threads can safely react to these changes without reading inconsistent state.

### RESOURCES

The links below helped me understand multithreading. AI helped with debugging and testing.

[Low Level: How does a Mutex even work?](https://youtu.be/1tZhmTnk-vc?si=N88K6muGCg-Qpk8h)  
[Portfolio Courses: Introduction To Threads](https://youtu.be/ldJ8WGZVXZk?si=VvE5k_M5YjynfirD)
[Wikipedia: Thread (computing)](https://en.wikipedia.org/wiki/Thread_(computing)#Single-threaded_vs_multithreaded_programs)
[Wikipedia: Deadlock (computer science)](https://en.wikipedia.org/wiki/Deadlock_(computer_science))
[Chatgpt](https://chatgpt.com)  
