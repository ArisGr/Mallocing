# Mallocing

Mallocing is an open-source, scalable library to intercept function calls of malloc, calloc, realloc, free in C and new,delete in C++
to place allocation objects on heterogeneous memory systems. It replaces the original functions of glibc with custom ones by using
the respective functions of the MEMKIND API. The library is compiled into shared object (.so file) and then preloaded to the target
executable using LD_PRELOAD.


## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## File descriptions
In this repo, you will find a number of files, each one with each own functionality for placement on heterogeneous systems.
## Installation
Clone the repository:
```bash
git clone https://github.com/your-username/your-repo-name.git
cd Mallocing
```

Then compile the desired file using the necessary flags, like this (example for custom_mallocs.c):

```bash
g++ -shared -fPIC -o libalgo.so algo.c new.cpp  -ldl -lmemkind
```

this command compiles the algo.c file, which includes the custom implementation for the C allocation and deallocation functions, and the new.cpp 
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
for the chunks.c file which performs monitoring, a log file is produced.
This file displays the allocated bytes and active allocated objects over time 
for the execution of the target application.

Here's how the log file looks :
![Screenshot](screenshot.png)

## Features
- 🚀 Fast and efficient
- 🛠️ Easy to use
- ⚙️ Customizable and scalable

## Contributing
Contributions are welcome! Please open an issue first to discuss what you'd like to change.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


## Acknowledgments
- [Some Useful Library](https://example.com)
- Thanks to [@person](https://github.com/person) for their support.
- Memkind API: https://pmem.io/memkind/manpages/memkind.3/

