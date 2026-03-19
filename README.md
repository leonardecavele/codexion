_This project has been created as part of the 42 curriculum by ldecavel._

### DESCRIPTION

section that clearly presents the project, including its goal and a
brief overview

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

A “Blocking cases handled” section describing all the concurrency issues addressed in your solution (e.g., deadlock prevention and Coffman’s conditions, starvation prevention, cooldown handling, precise burnout detection, and log serialization).

### THREAD SYNCHRONIZATION MECHANISMS

xplaining the specific threading primitives used in your implementation (pthread_mutex_t, pthread_cond_t,
custom event implementation) and how they coordinate access to shared resources
(dongles, logging, monitor state). Include examples of how race conditions are
prevented and how thread-safe communication is achieved between coders and the
monitor.

### RESOURCES

The links below helped me understand constrained decoding. AI helped with debugging and testing.

[Aidan Cooper: Constrained Decoding](https://www.aidancooper.co.uk/constrained-decoding/)  
[Nvidia: Constrained Decoding](https://docs.nvidia.com/deeplearning/triton-inference-server/user-guide/docs/tutorials/Feature_Guide/Constrained_Decoding/README.html)  
[Michael Breendoerfer: Constrained Decoding](https://mbrenndoerfer.com/writing/constrained-decoding-structured-llm-outputhttps://mbrenndoerfer.com/writing/constrained-decoding-structured-llm-outputhttps://mbrenndoerfer.com/writing/constrained-decoding-structured-llm-outputhttps://mbrenndoerfer.com/writing/constrained-decoding-structured-llm-output)  
[Chatgpt](https://chatgpt.com)  
