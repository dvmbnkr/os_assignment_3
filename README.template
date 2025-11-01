# COMP SCI 2002 – Assignment 3: Concurrency (Parallel Merge Sort)

**Authors:** Devam Banker, Ishva Rathod, and Neel Patel
**Group Name:** Group 112

---

## Overview

The following C program is an implementation of a multi-threaded merge sort algorithm using the pthread library.
Its aim is to demonstrate inter-thread synchronisation and parallel performance improvement compared to traditional single-threaded merge sort.

A user can define the array size, the maximum recursion depth for thread creation (cutoff level), and a random seed to control array generation.
To achieve faster sorting for large datasets, additional threads are spawned recursively as the cutoff level increases, allowing subarrays to be handled concurrently.

---

## Manifest

* mergesort.c – Implements core sorting logic: `merge()`, `my_mergesort()`, `parallel_mergesort()`, and `buildArgs()`. Handles both serial and threaded recursion.
* mergesort.h – Header file declaring global variables, struct definitions, and function prototypes for `mergesort.c`.
* test-mergesort.c – Program that generates test data, calls sorting functions, measures performance, and verifies correctness.
* Makefile – Automates compilation using `gcc` with `-pthread` and warning flags.
* README.md – Documentation describing the project overview, features, testing methodology, and reflection.

---

## Building the Project

1. Open the terminal and navigate to the project directory.
2. Compile the code using the provided Makefile:

   ```bash
   make
   ```

   This produces an executable named `test-mergesort`.
3. To clean up all object and dependency files:

   ```bash
   make clean
   ```
4. **Requirements:**

   * GCC compiler supporting POSIX threads (`gcc version >= 9.0`)
   * Linux or macOS environment with pthread library available

---

## Features and Usage

### Features

* **Parallel Merge Sort:** Creates new threads recursively up to the cutoff level chosen by the user.
* **Cutoff Control:** Limits how many threads are made to keep performance balanced.
* **Recursive Merging:** Combines sorted subarrays efficiently using a temporary array.
* **Timing Display:** Shows how long the sort takes, making it easy to compare serial and parallel runs.
* **Result Check:** Automatically verifies that the final array is sorted correctly.

### Usage

Run the program as follows:

```bash
./test-mergesort <input_size> <cutoff_level> <seed>
```

**Example:**

```bash
./test-mergesort 100000000 3 1234
```

---

## Testing

### Correctness Testing

The first factor in testing our code was correctness — ensuring the array was sorted properly under all cutoff levels.

* Started by running the program with small arrays (`n = 10, 20, 100`) to easily inspect the output.
* Modified `printA()` and `printB()` to show only the first *n* elements, allowing visual verification that elements were in ascending order.
* Used the built-in `check_if_sorted()` function to automatically confirm correctness.
* Tested multiple cutoff levels (0, 1, 2, 3) and verified consistent results across all runs.
* Also tested with several random seeds to confirm that different data distributions still sorted correctly.

**Example Commands:**

```bash
./test-mergesort 20 0 1
./test-mergesort 100 2 5
./test-mergesort 100 3 42
```

**Expected Output:**

```
Sorting 100 elements took 0.00 seconds.
```

All arrays passed the sortedness check.

---

### Performance Testing

The second factor was performance — confirming that threading provided a measurable speed improvement.
Larger arrays were used to observe differences in runtime between the serial and parallel versions of merge sort.

The serial implementation (`cutoff = 0`) was compared against the parallel implementation with increasing cutoff levels (`1–6`).
For consistent results, the same random seed was used for each run, and the program’s printed timing results were recorded.

**Example Commands:**

```bash
./test-mergesort 10000000 0 1   # serial
./test-mergesort 10000000 2 1   # moderate threading
./test-mergesort 10000000 4 1   # higher threading
```

**Results Summary:**

| **Cutoff** | **Time (s)** |
| :--------: | :----------: |
|      0     |     1.53     |
|      1     |     0.79     |
|      2     |     0.44     |
|      3     |     0.31     |
|      4     |     0.31     |
|      5     |     0.30     |

The results showed a clear improvement in performance as the cutoff level increased, reaching up to five times faster than the serial version.
Beyond a cutoff of three, the runtime stabilised — suggesting the CPU cores were fully utilised and further thread creation added unnecessary overhead.
These results confirm that parallel merge sort achieved a significant speed-up while maintaining accuracy and stability across all levels.

---

## Known Bugs and Limitations

A few potential limitations were identified based on the starter code and the expected behaviour of multithreaded programs:

* **Diminishing returns at higher cutoff levels:** After around level 5, performance improvements tend to level off. This is expected due to thread overhead and hardware limits.
* **Array printing assumption:** The provided `printA()` and `printB()` functions assume at least 100 elements, as noted in the starter code comments.
* **Memory use on large arrays:** The program runs efficiently on our system (16 GB RAM), but very large input sizes could use substantial memory on smaller machines.

No major functional bugs were encountered during normal runs, and the program behaves as expected for typical input sizes.

---

## Reflection and Self-Assessment

### Learning and Understanding

This project deepened our understanding of concurrent programming and the divide-and-conquer strategy in practice.
We learned how cutoff thresholds help balance thread overhead and concurrency, and how recursion can be adapted for multithreaded execution.

Initially, it was challenging to grasp thread recursion, especially using `pthread_create()` and `pthread_join()`.
The indirect recursion model — where threads spawn other threads — required careful struct handling and clear logic flow.

### Development Process

Compared to the merge sort implementation covered in ADDS (Algorithm Design and Data Structures), this assignment required a different approach.
We had to consult external resources and slightly alter the algorithm to suit the requirements of the parallel version.
Implementing it with **pthreads**, a concept we were still becoming familiar with, added another layer of difficulty.

### Collaboration

Collaboration played a key role throughout development. Frequent discussions helped ensure everyone understood both the logic and concurrency principles behind the implementation.

---

## Sources Used

* **GeeksforGeeks.** (n.d.). *Merge Sort*. Retrieved from [https://www.geeksforgeeks.org/dsa/merge-sort/](https://www.geeksforgeeks.org/dsa/merge-sort/)
  → Used to understand the base merge sort algorithm and its divide-and-conquer structure, which informed the logic for `my_mergesort()` and `merge()`.

* **W3Schools.** (n.d.). *Merge Sort Algorithm*. Retrieved from [https://www.w3schools.com/dsa/dsa_algo_mergesort.php](https://www.w3schools.com/dsa/dsa_algo_mergesort.php)
  → Provided a beginner-friendly explanation of recursive sorting concepts, helping structure and document the algorithm’s flow.

* **GeeksforGeeks.** (n.d.). *Merge Sort using Multi-threading*. Retrieved from [https://www.geeksforgeeks.org/dsa/merge-sort-using-multi-threading/](https://www.geeksforgeeks.org/dsa/merge-sort-using-multi-threading/)
  → Helped clarify strategies for dividing work between threads, influencing the approach taken in `parallel_mergesort()`.

* **Stack Overflow.** (2020). *How to multithread the merge operation in merge sort?* Retrieved from [https://stackoverflow.com/questions/63584950/how-to-multithread-the-merge-operation-in-merge-sort](https://stackoverflow.com/questions/63584950/how-to-multithread-the-merge-operation-in-merge-sort)
  → Referenced to understand alternative multithreading techniques and why recursive thread spawning (instead of parallel merging) is more suitable for this project.

* **GitHub Copilot.** (2025). *AI pair-programming tool by GitHub.*
  → Used minimally for inline code suggestions and syntax correction during development. All generated code was manually reviewed, modified, and verified for correctness.

* **ChatGPT (OpenAI).** (2025). *Assisted with conceptual clarification, documentation structure, and README drafting.*
  → Used to understand pthread recursion concepts, concurrency debugging, and professional formatting for submission documentation.