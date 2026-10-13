*This project has been created as part of the 42 curriculum by mvasquez.*

# Codexion

## Description

Codexion is a concurrency simulation written in C using POSIX threads and
mutexes.

The simulation represents multiple coders sharing a circular set of USB
dongles. Each coder alternates between compiling, debugging and refactoring.

To compile, a coder must acquire two dongles simultaneously: the dongle on
their left and the dongle on their right. After compilation, both dongles are
released and remain unavailable for a configurable cooldown period.

The main objective of the project is to coordinate access to shared resources
while avoiding deadlocks, race conditions and starvation.

Two scheduling policies are supported:

- `fifo` — First In, First Out. Conflicting requests are prioritized according
  to their arrival order.
- `edf` — Earliest Deadline First. Conflicting requests are prioritized
  according to the coder's burnout deadline:

  `last_compile_start + time_to_burnout`

Requests that do not compete for the same dongles may be granted
independently, allowing non-conflicting coders to compile concurrently.

A dedicated monitor thread detects burnout and stops the simulation when a
coder reaches its deadline without starting a new compilation. The simulation
also ends successfully when every coder completes the required number of
compilations.

---

## Instructions

### Compilation

Enter the `coders` directory and run:

```bash
make
```

This builds the executable:

```bash
./codexion
```

The project is compiled with the usual 42 flags:

```text
-Wall -Wextra -Werror
```

Useful Makefile rules:

```bash
make
make clean
make fclean
make re
```

### Execution

Run the program with:

```bash
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown policy
```

Example:

```bash
./codexion 4 1000 200 100 150 5 50 edf
```

### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coder threads participating in the simulation. Must be greater than `0`. |
| `time_to_burnout` | Maximum time in milliseconds between compilation starts before a coder burns out. |
| `time_to_compile` | Time in milliseconds spent compiling. |
| `time_to_debug` | Time in milliseconds spent debugging. |
| `time_to_refactor` | Time in milliseconds spent refactoring. |
| `number_of_compiles_required` | Number of compilations every coder must complete before the simulation finishes successfully. |
| `dongle_cooldown` | Time in milliseconds during which a released dongle remains unavailable. |
| `policy` | Scheduling policy: `fifo` or `edf`. |

All arguments are mandatory.

Numeric arguments accept non-negative decimal values, while
`number_of_coders` must be strictly positive. Any invalid argument causes the
program to terminate with an error message.

---

## Simulation Model

Each coder owns references to two neighboring dongles in a circular
arrangement.

For four coders, the logical layout is:

```text
Coder 1 -> Dongle 0 + Dongle 1
Coder 2 -> Dongle 1 + Dongle 2
Coder 3 -> Dongle 2 + Dongle 3
Coder 4 -> Dongle 3 + Dongle 0
```

A normal coder cycle is:

```text
request dongles
      |
      v
   compiling
      |
      v
release dongles
      |
      v
   debugging
      |
      v
 refactoring
      |
      v
repeat while the simulation is active
```

A coder must acquire both required dongles before compiling. A partial
acquisition is never kept.

---

## Scheduling

Each coder owns one scheduler request containing:

- a pointer to the coder;
- an arrival order;
- a burnout deadline;
- an active flag.

Active requests are stored in a heap.

### FIFO

With `fifo`, conflicting requests are prioritized by arrival order.

A lower `arrival_order` means the request arrived earlier and therefore has
higher priority.

### EDF

With `edf`, conflicting requests are prioritized by their burnout deadline.

A request with the earlier deadline has higher priority.

Requests with equal EDF deadlines are considered equal in priority. This
allows independent requests that do not share dongles to proceed without
unnecessary serialization.

### Conflict-aware scheduling

A higher-priority request blocks another request only when both requests share
at least one dongle.

For example:

```text
Coder 1 -> D0 + D1
Coder 2 -> D1 + D2
Coder 3 -> D2 + D3
Coder 4 -> D3 + D0
```

Coder 1 and Coder 3 do not share a dongle, so both may compile concurrently
when their dongles are available.

---

## Thread Synchronization Mechanisms

The program uses several synchronization mechanisms, each with a specific
responsibility.

### Main simulation mutexes

- `print_mutex`
  - Protects log output.
  - Prevents messages from different threads from being printed over each
    other.

- `stop_mutex`
  - Protects the global `stopped` flag.
  - Allows coder threads and the monitor thread to safely detect or request
    simulation termination.

- `start_mutex`
  - Protects the shared start state, including `ready_count` and `started`.

- `start_cond`
  - Coordinates the common simulation start.
  - Coder threads first report that they are ready and then wait.
  - The main thread waits until all coders are ready, starts the simulation
    clock, sets `started`, and broadcasts the condition to release the waiting
    threads.

### Scheduler mutex

The scheduler owns a mutex protecting:

- the request heap;
- the arrival counter;
- active scheduler requests;
- grant and release operations coordinated through the scheduler.

While a coder is waiting for a grant, the scheduler mutex is temporarily
released so other coders can enter the scheduler and make progress.

### Coder state mutex

Every coder has its own `state_mutex`.

It protects coder-specific state such as:

- `last_compile_start`;
- `compile_count`.

This state is accessed by both coder threads and the monitor thread.
Compilation starts and burnout decisions acquire `stop_mutex` before `state_mutex`, recheck the
current deadline, and update the state before logging. This prevents a late
compilation from resetting an expired deadline and prevents the monitor from
stopping the simulation based on an outdated deadline.

### Dongle mutexes

Every dongle has its own mutex.

Before inspecting or changing a pair of dongles, both mutexes are locked.
Dongles are always locked in ascending ID order.

For example:

```text
D0 before D3
D1 before D2
```

Using a consistent lock order prevents circular wait and therefore avoids
deadlocks when several coders compete for overlapping dongle pairs.

---

## Dongle Cooldown

When a coder finishes compiling, its two dongles are released.

Each dongle stores:

```text
cooldown_until
```

This is an absolute simulation timestamp rather than a countdown. Additions
that would exceed `LLONG_MAX` saturate at `LLONG_MAX` to prevent overflow
and premature dongle reuse.

If a dongle is released at `500 ms` with a cooldown of `50 ms`:

```text
cooldown_until = 550 ms
```

The dongle can only be taken again when both conditions are true:

```text
in_use == 0
current_time >= cooldown_until
```

---

## Burnout Monitoring

A dedicated monitor thread supervises every coder.

For each coder, the effective burnout deadline is based on:

```text
last_compile_start + time_to_burnout
```

The monitor periodically checks the current elapsed simulation time against
these deadlines.

If a coder reaches its deadline, the simulation is stopped and the burnout
event is logged.

The global stop state may also be set when every coder reaches the required
number of compilations.

---

## Timing

Simulation time is measured in milliseconds using `gettimeofday`.

The common start timestamp is recorded only after all coder threads are ready,
so every thread uses the same logical simulation origin.

Long actions such as compiling, debugging and refactoring use an interruptible
sleep helper. Instead of sleeping for the entire duration at once, the helper
sleeps in short intervals and repeatedly checks whether the simulation has
been stopped.

This allows threads to react quickly to burnout or successful simulation
completion.

---

## Blocking Cases Handled

The implementation explicitly handles several situations that could otherwise
leave resources or threads in an invalid state:

- invalid or missing program arguments;
- memory-allocation failures;
- mutex initialization failures;
- partial initialization cleanup;
- partial coder-thread creation failures;
- monitor-thread creation failure;
- duplicate active scheduler requests;
- attempts to use the same dongle as both members of a pair;
- unavailable dongles;
- dongles still in cooldown;
- conflicting higher-priority scheduler requests;
- expired scheduler deadlines;
- simulation termination while a coder is waiting or sleeping;
- safe release of acquired dongles before leaving a coder cycle.

If thread creation fails before the official simulation start, already-created
threads are awakened, told to stop and joined before cleanup continues.

---

## Output

Simulation events are printed with their elapsed timestamp and coder ID.

Typical events include:

```text
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
200 1 is debugging
300 1 is refactoring
```

A burnout event terminates the simulation.

Because the simulation is concurrent, the exact order in which independent
coders appear in the output may vary between executions.

---

## Project Structure

```text
coders/
├── Makefile
├── includes/
    ├── codexion.h
└── src/
    ├── cleanup/
    │   └── destroy_sim.c
    ├── dongle/
    │   ├── dongle.c
    │   └── dongle_pair.c
    ├── heap/
    │   ├── heap_compare.c
    │   ├── heap_pop.c
    │   ├── heap_push.c
    │   └── heap_remove.c
    ├── init/
    │   ├── init_coders.c
    │   ├── init_dongles.c
    │   ├── init_scheduler.c
    │   └── init_sim.c
    ├── log/
    │   └── log.c
    ├── monitor/
    │   └── monitor.c
    ├── parsing/
    │   ├── parse_config.c
    │   ├── parse_number.c
    │   └── parse_policy.c
    ├── scheduler/
    │   ├── scheduler_grant.c
    │   ├── scheduler_request.c
    │   └── scheduler_wait.c
    ├── state/
    │   ├── coder_state.c
    │   └── sim_state.c
    ├── threads/
    │   ├── coder_cycle.c
    │   ├── coder_routine.c
    │   ├── thread_control.c
    │   └── thread_join.c
    ├── time/
    │   ├── sim_sleep.c
    │   ├── sleep.c
    │   └── time.c
    └── main.c
```

---

## Testing

Useful checks during development include:

### Norminette

```bash
norminette
```

### Valgrind

```bash
valgrind --leak-check=full --show-leak-kinds=all \
./codexion 4 1000 200 100 150 5 50 edf
```

### Helgrind

```bash
valgrind --tool=helgrind \
./codexion 4 1000 200 100 150 5 50 edf
```

Because thread scheduling is nondeterministic, tests should validate
correctness properties rather than relying on one exact interleaving of log
messages.

---

## Resources

Resources used while working on the project included:

- the Codexion subject and project requirements;
- POSIX threads documentation and manual pages;
- C standard-library documentation;
- Valgrind and Helgrind documentation;
- 42 Norm documentation.

Useful manual pages include:

```bash
man pthread_create
man pthread_join
man pthread_mutex_init
man pthread_mutex_lock
man pthread_mutex_unlock
man pthread_cond_wait
man pthread_cond_broadcast
man gettimeofday
man usleep
```

---

## Use of AI

AI tools were used as a learning and review aid during development.

They were used to help:

- explain POSIX-thread synchronization concepts;
- reason about mutexes, condition variables and deadlock prevention;
- review scheduler and heap design;
- analyze possible race conditions and cleanup paths;
- improve documentation and README wording;
- propose testing strategies with Valgrind and Helgrind.

All generated suggestions were reviewed, adapted and tested before being
integrated into the project. The final implementation and its correctness
remain the responsibility of the author.

---

## Author

**mvasquez**