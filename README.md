# Mallocing

Mallocing is an open-source, scalable library to intercept function calls of *malloc, calloc, realloc, free* in C and *new,delete* in C++
to place allocation objects on heterogeneous memory systems. It replaces the original functions of glibc with custom ones by using
the respective functions of the MEMKIND API. 
The library is compiled into a shared object (.so file) and then preloaded to the target executable using LD_PRELOAD.
The library is further extended to perform monitoring of the allocation patterns of the
target executable, by monitoring the size and the number of active allocated objects.


## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## File descriptions
In this repo, you will find a number of files, each one with each own functionality for placement on heterogeneous memory systems.

File descriptions:
- **custom_allocator.c** : This file is used to perform allocations either on DRAM or Optane. It includes the custom functions that are used to override the original
  glibc implementations. If the *dram* variable is set to 1, all allocations will be handled by DRAM, and if *dram=0* all allocations will be handled by optane.

- **random.c** : This file is used to perform allocations randomly. Each allocation is handled by either DRAM or Optane randomly.

- **round-robin.c** : This file is used to perform allocations on a round-robin way. The allocation data are placed in round-robin way
on each memory type.

- **monitor.c** : This file is used to perform allocations on DRAM and also monitor the allocation patterns during the execution of the
  target application. A background thread is used, which logs information about the allocated bytes and active allocated objects over time.

- **functions.cpp** : This file includes the new/delete functions from C++, which are simply redirected to the custom functions of the .c file (e.g. custom_allocator.c).

## Installation
Clone the repository:
```bash
git clone https://github.com/your-username/your-repo-name.git
```
Get inside the directory:
```bash
cd Mallocing
```

Then compile the desired file using the necessary flags, like this (example for algo.c):

```bash
g++ -shared -fPIC -o spmalloc.so custom_allocator.c functions.cpp -lmemkind
```

this command compiles the custom_allocator.c file, which includes the custom implementation for the C allocation and deallocation functions, and the functions.cpp 
file, which includes the respective C++ functions, into a shared object called spmalloc.so.

The flags used:
- **shared**:  This tells the compiler to create a shared library (also called a dynamic library).
- **fPIC**:  It generates code that can be loaded at any memory address without requiring modification.
- **lmemkind**:  Links against the memkind library (necessary for the memkind funcions).



## Usage
Run the target executable (e.g test) using LD_PRELOAD with the shared library:
```bash
LD_PRELOAD=/path/to/library/spmalloc.so /path/to/executable/test
```
If the library used was the one compiled from the the chunks.c file, which performs monitoring, a log file is produced.
This file displays the allocated bytes and active allocated objects over time 
for the execution of the target application.

Here's how the log file looks :

![Screenshot_3](https://github.com/user-attachments/assets/f55597e6-f2ff-414d-b3ed-032b2ef634b8)

The active objects value is the number of active allocation objects from the beginning of the execution until the current time stamp, and
the allocated bytes value is the number of allocated bytes in the current x second time stamp. In the example above x=0.25s, but this value is user defined and can be changed to fit the users preference.

## Features
- 🚀 Fast and efficient
- 🛠️ Easy to use
- ⚙️ Customizable and scalable

## Contributing
Contributions are welcome! Feel free to expand and experiment with this library.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Useful Sites
- Memkind API: https://pmem.io/memkind/manpages/memkind.3/

