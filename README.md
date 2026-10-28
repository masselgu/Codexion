*This project has been created as part of the 42 curriculum by masselgu.*

# Codexion

## Description

Codexion is a concurrent programming project written in C using POSIX threads (`pthread`).

The project is inspired by the classic Dining Philosophers problem, but replaces philosophers and forks with **coders** and **USB dongles**. There are as many dongles as coders, arranged so that each coder shares one dongle with each neighboring coder.

Each coder repeatedly performs the following cycle:

1. Waits for the simulation to start.
2. Requests its two required dongles.
3. Compiles code while holding both dongles.
4. Releases the dongles.
5. Debugs.
6. Refactors.
7. Repeats until the simulation ends or the coder burns out.

A coder must obtain **both of its dongles simultaneously** before compiling.

Each dongle also has a **cooldown period** after being released. This prevents an immediately reusable dongle from being acquired before its cooldown has expired.

The simulation ends when either:

* a coder reaches its burnout deadline;
* all coders have completed the required number of compilations;
* or an internal initialization or thread-management error requires the simulation to stop safely.

The project focuses on:

* POSIX threads and mutexes;
* race-condition prevention;
* deadlock prevention;
* starvation and scheduling;
* resource arbitration;
* cooldown management;
* precise time-based termination;
* thread-safe logging;
* safe thread and resource cleanup.

## Features

### Multithreading

Each coder runs in its own `pthread` thread. A separate monitor thread supervises the simulation and checks for burnout and completion.

### FIFO and EDF scheduling

Waiting coders are stored in per-dongle priority queues.

Two scheduling modes are supported:

* `fifo` — First In, First Out. Requests are ordered according to their request time.
* `edf` — Earliest Deadline First. Requests are prioritized according to the coder's burnout deadline.

### Two-dongle resource acquisition

A coder needs both adjacent dongles before it can compile. The implementation checks the state of both dongles while their mutexes are held and only grants the resources when the coder is allowed to proceed.

### Dongle cooldown

After compilation, each dongle receives an `available_at` timestamp. The dongle cannot be acquired again until its cooldown has expired.

### Burnout monitoring

A dedicated monitor thread continuously checks coder deadlines using timestamps. If a coder exceeds its allowed time, the monitor stops the simulation.

### Thread-safe logging

All simulation output is serialized through a dedicated print mutex so that concurrent coder threads cannot interleave their output.

## Instructions

### Requirements

A POSIX-compatible environment with:

* `cc` or another C compiler;
* POSIX threads support;
* `make`;
* standard POSIX headers and functions such as `pthread`, `gettimeofday`, and `usleep`.

## Compilation

The project is compiled with `cc` using the following flags:

```bash
-Wall -Wextra -Werror -pthread
```

To compile the project:

```bash
make
```

To remove object files:

```bash
make clean
```

To remove object files and the executable:

```bash
make fclean
```

To rebuild the project from scratch:

```bash
make re
```


### Execution

The program expects the following arguments:

```text
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

All time values are expressed in **milliseconds**.

The scheduler must be either:

```text
fifo
```

or:

```text
edf
```

### Example

```bash
./codexion 4 1500 200 200 200 3 100 fifo
```

This example creates:

* 4 coders;
* a 1500 ms burnout limit;
* 200 ms compilation time;
* 200 ms debugging time;
* 200 ms refactoring time;
* 3 required compilations per coder;
* a 100 ms dongle cooldown;
* FIFO scheduling.

An EDF example:

```bash
./codexion 5 800 200 200 200 3 50 edf
```

## Command-line arguments

| Argument                      | Description                                               |
| ----------------------------- | --------------------------------------------------------- |
| `number_of_coders`            | Number of coder threads and dongles                       |
| `time_to_burnout`             | Maximum allowed time before a coder burns out             |
| `time_to_compile`             | Time required to compile                                  |
| `time_to_debug`               | Time required to debug                                    |
| `time_to_refactor`            | Time required to refactor                                 |
| `number_of_compiles_required` | Number of successful compilations required for each coder |
| `dongle_cooldown`             | Time a dongle remains unavailable after being released    |
| `scheduler`                   | Scheduling policy: `fifo` or `edf`                        |

## Output

Simulation actions are printed in the following format:

```text
timestamp coder_id action
```

For example:

```text
0 1 is compiling
205 2 is debugging
406 1 is refactoring
```

The timestamp represents the elapsed time since the simulation started.

Logging is protected by a mutex to guarantee that complete log lines are printed atomically relative to other coder threads.

## Project structure

The implementation is divided into small source files according to responsibility:

```text
src/
├── main.c
├── coder.c
├── destroy.c
├── dongles.c
├── dongles_extra.c
├── cleanup.c
├── priority_queue.c
├── heap_extra.c
├── simulation_utils.c
├── init_utils.c
├── init.c
├── init_sync.c
├── monitor.c
├── parsing.c
├── simulation.c
├── time_utils.c
├── utils.c
└── codexion.h
```

### Main responsibilities

`main.c`
Parses input, initializes the simulation, starts it, and performs final cleanup.

`coder.c`
Contains coder initialization/waiting and the main coder routine.

`dongles.c`
Handles requesting, queuing, acquiring, compiling with, and releasing dongles.

`dongles_extra.c`
Contains helper logic for checking dongle availability and attempting simultaneous acquisition.

`priority_queue.c` and `heap_extra.c`
Implement the heap/priority-queue logic used to order coder requests.

`monitor.c`
Runs the monitoring thread and checks burnout and completion conditions.

`simulation.c`
Creates and starts simulation threads and coordinates simulation shutdown.

`init.c`, `init_utils.c`, `init_sync.c`
Handle initialization and destruction of coder, dongle, heap, and synchronization resources.

`parsing.c` and `simulation_utils.c`
Handle command-line validation and parsing helpers.

`time_utils.c`
Provides millisecond timing and interruptible waiting.

`utils.c`
Contains shared helpers such as thread-safe logging and stop-state access.

`destroy.c` and `cleanup.c`
Handle synchronization-resource destruction and allocated-memory cleanup.

## Blocking cases handled

Concurrency introduces several problems that must be controlled carefully. The implementation addresses the main blocking cases required by the project.

### Deadlock prevention and Coffman's conditions

A classic deadlock can occur when all four Coffman conditions are simultaneously present:

1. Mutual exclusion.
2. Hold and wait.
3. No preemption.
4. Circular wait.

The implementation intentionally breaks the **circular wait** condition.

Each coder needs two dongles, but the two dongle mutexes are always locked in the same deterministic order. The code determines a first and second dongle before trying to lock them:

```c
if (coder->left < coder->right)
{
    first = coder->left;
    second = coder->right;
}
else
{
    first = coder->right;
    second = coder->left;
}
```

The mutexes are then acquired in that order.

Because all coder threads follow the same ordering rule, one thread cannot lock resource A while another locks resource B and have both wait forever for the other resource in the opposite order.

In addition, a coder only successfully acquires the pair when both dongles satisfy the scheduling and availability conditions.

### Starvation prevention

The project uses a priority queue for waiting requests on each dongle.

The selected scheduling policy determines which coder has priority:

* `fifo` prioritizes earlier requests;
* `edf` prioritizes earlier deadlines.

A coder must be at the top of the relevant dongle queue before being allowed to take the resource.

This prevents arbitrary thread scheduling from being the only factor deciding which coder receives a dongle and provides deterministic resource arbitration.

### Dongle cooldown handling

A dongle is not immediately reusable after a compilation.

When a coder releases a dongle, the implementation records:

```c
dongle->available_at
```

using the current timestamp plus the configured cooldown:

```c
available_at = release_time + dongle_cooldown;
```

Before a coder can acquire the dongle, the scheduler checks whether the current time has reached `available_at`.

Therefore, a recently released dongle cannot be reused before its required cooldown has elapsed.

### Precise burnout detection

Burnout detection is handled by a dedicated monitor thread rather than relying only on coder threads.

Each coder records the start time of its latest compilation:

```c
last_compile_start
```

The monitor compares the current time against the coder's burnout deadline.

Because the monitor runs independently from the coder routines, burnout detection does not depend on a coder reaching a convenient point in its own execution.

The timing system uses `gettimeofday()` and millisecond timestamps, while the monitoring loop uses short sleeps to continuously re-check the simulation without using a completely busy CPU loop.

### Simulation completion detection

The monitor also checks whether every coder has completed the required number of compilations.

When all coders have reached the required count, the stop state is activated and the simulation can terminate cleanly.

### Log serialization

Multiple coder threads can attempt to print at exactly the same time.

To prevent interleaved or corrupted output, logging is protected by a dedicated mutex:

```c
pthread_mutex_lock(&coder->sim->print_mutex);
```

The complete timestamp and action line is printed while the mutex is held.

This guarantees serialized access to standard output.

### Safe termination

The simulation uses a shared stop flag protected by its own mutex.

Coder threads and the monitor do not access this shared state without synchronization.

Once the simulation is stopped, coder threads leave their work loops and the main thread joins the created threads before destroying synchronization objects and freeing dynamically allocated memory.

## Thread synchronization mechanisms

The implementation relies on POSIX synchronization primitives together with a custom event-style mechanism built from shared state and condition variables.

### `pthread_mutex_t`

Mutexes are used for several different synchronization purposes.

#### Dongle mutexes

Every dongle owns its own mutex:

```c
pthread_mutex_t d_mutex;
```

This protects access to the dongle's shared state, including:

* queue operations;
* availability state;
* acquisition state;
* release state.

A coder locks the relevant dongle mutexes before inspecting or changing their shared state.

This prevents two threads from simultaneously modifying the same dongle.

#### Coder mutex

Every coder has its own mutex:

```c
pthread_mutex_t c_mutex;
```

It protects coder-specific shared data such as:

* compilation count;
* request/deadline timestamps;
* compilation timing information.

For example, the compile counter is updated while holding the coder mutex:

```c
pthread_mutex_lock(&coder->c_mutex);
coder->compile_count++;
pthread_mutex_unlock(&coder->c_mutex);
```

This prevents data races between the coder thread and the monitor or other code reading the same state.

#### Stop mutex

The simulation contains a dedicated stop mutex:

```c
pthread_mutex_t stop_mutex;
```

The stop flag is accessed through the synchronized helper:

```c
should_stop(sim);
```

This ensures that reads and writes of the simulation termination state are synchronized between coder threads, the monitor, and shutdown code.

#### Print mutex

The print mutex:

```c
pthread_mutex_t print_mutex;
```

serializes all terminal output.

This avoids output lines being mixed together when several coder threads log actions concurrently.

#### Start mutex

The start mutex protects the simulation start state and coordinates access to the start condition variable.

### `pthread_cond_t`

The simulation uses:

```c
pthread_cond_t start_cond;
```

as part of a custom event-style startup mechanism.

The event is represented by:

```c
simulation_started
```

protected by:

```c
start_mutex
```

Coder threads wait until the simulation start event is signaled:

```c
pthread_mutex_lock(&sim->start_mutex);

while (!sim->simulation_started)
    pthread_cond_wait(&sim->start_cond, &sim->start_mutex);

pthread_mutex_unlock(&sim->start_mutex);
```

The `while` loop is important because a condition variable does not itself store application state. The actual state is represented by `simulation_started`.

Once thread creation has completed, the simulation sets the flag and signals the waiting coder threads.

This creates a common synchronization point so coder threads do not begin their normal work before the simulation is ready.

### Custom event mechanism

Rather than relying on an external event library, the project implements a simple event-style synchronization mechanism using:

```text
simulation_started
        +
start_mutex
        +
start_cond
```

The mutex protects the event state, while the condition variable blocks coders until the event occurs.

The same design principle is used elsewhere in the program: shared state is protected by a mutex, and threads cooperate through explicit state changes.

### Priority queues

The project also uses custom heap-based priority queues as a synchronization-related scheduling mechanism.

A coder request is inserted into the heap associated with each required dongle.

The comparator decides which coder currently has priority:

```c
higher_priority(...)
```

A coder can only proceed when it is the appropriate request at the top of the queue and its dongles satisfy the remaining availability conditions.

This combines mutual exclusion from mutexes with deterministic scheduling from the priority queues.

## Race-condition prevention examples

### Protecting the stop flag

Without synchronization, one thread could read `stop_flag` while another is modifying it.

The project avoids this by using `stop_mutex` whenever the shared stop state is accessed.

### Protecting compilation counters

`compile_count` is read and modified by concurrent parts of the simulation.

The coder mutex protects updates:

```c
pthread_mutex_lock(&coder->c_mutex);
coder->compile_count++;
pthread_mutex_unlock(&coder->c_mutex);
```

This prevents lost updates and inconsistent reads.

### Protecting dongle state

A dongle contains multiple fields that must remain consistent:

```text
available_at
in_process
heap
```

These fields are accessed while holding the dongle's mutex.

This prevents two coder threads from both deciding that the same dongle is available and acquiring it simultaneously.

### Protecting logging

Without the print mutex, two threads could execute `printf()` at nearly the same time and produce mixed output.

The dedicated logging mutex ensures that each complete action is printed as one synchronized operation.

### Thread-safe communication between coders and the monitor

Coder threads update their timing and progress information under the appropriate coder mutexes.

The monitor reads that shared information using the same synchronization rules.

The simulation stop state is also shared through the protected `stop_flag`.

As a result, the monitor can safely communicate termination to the coder threads without unsynchronized access to shared state.

## Time management

The project uses `gettimeofday()` to obtain timestamps in milliseconds.

The main timing helper is:

```c
size_t get_time(void);
```

Waiting is implemented through:

```c
void wait_ms(size_t duration, t_sim *sim);
```

The wait function periodically checks the simulation stop state, allowing a thread to leave its wait early if the simulation has ended.

This is important because a fixed blocking sleep could delay shutdown after a burnout or completion event.

## Memory management

The program dynamically allocates arrays for:

* coders;
* dongles.

Allocated memory is released during cleanup through:

```c
free_simulation()
```

Synchronization primitives are destroyed separately through the corresponding destruction functions.

Thread creation failures and partial initialization failures are handled by cleanup paths so that already-created resources are released before exiting.

## Technical choices

### POSIX threads

`pthread_create()` is used to create coder and monitor threads.

`pthread_join()` is used during shutdown to wait for all created threads to finish before resources are destroyed.

### Per-dongle mutexes

Each dongle has its own mutex instead of using one global mutex for all dongles.

This reduces unnecessary contention and allows unrelated resources to be managed independently.

### Priority heap

A custom heap is used to manage waiting coder requests efficiently.

The priority decision is separated from the resource itself, allowing FIFO and EDF scheduling policies to use the same resource-management structure.

### Dedicated monitor thread

The monitor is separated from coder execution so that burnout and completion checks are not dependent on a specific coder reaching a particular instruction.

### Timestamp-based cooldowns

Cooldowns are represented by absolute timestamps (`available_at`) rather than by blind sleeping.

This allows the scheduler to check the real state of each dongle against the current time.

## Resources

The following references were useful for understanding and implementing the concurrency concepts involved in Codexion:

### POSIX threads

* `pthread_create()` — creating threads
  https://man7.org/linux/man-pages/man3/pthread_create.3.html

* `pthread_join()` — waiting for threads to terminate
  https://man7.org/linux/man-pages/man3/pthread_join.3.html

* `pthread_mutex_lock()` — mutex synchronization
  https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html

* `pthread_cond_wait()` — condition variables
  https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html

### Timing

* `gettimeofday()`
  https://man7.org/linux/man-pages/man2/gettimeofday.2.html

* `usleep()`
  https://man7.org/linux/man-pages/man3/usleep.3.html

### Concurrency and deadlocks

* E. G. Coffman, M. J. Elphick, A. Shoshani, *System Deadlocks*, ACM Computing Surveys, 1971.
* Allen B. Downey, *The Little Book of Semaphores*
  https://greenteapress.com/wp/semaphores/
* Silberschatz, Galvin, Gagne, *Operating System Concepts* — synchronization, deadlocks, scheduling, and condition variables.



### AI usage

They were used for:

* explaining POSIX thread concepts;
* understanding mutexes and condition variables;
* understanding FIFO and EDF scheduling;
* reviewing the synchronization logic;
* helping organize and improve the project structure;
* helping write and improve documentation.
