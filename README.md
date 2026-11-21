# ICS45C Homework 7

Welcome to Homework 7!

Templates and exceptions with an Array and a Matrix

## 📋 Instructions

Carefully follow all instructions on the course documents site [here](https://sites.google.com/view/45c-programming-in-cpp/homework-7).

## 🛠️ Technical Details

This repository contains the starter code for your C++ Homework 7.

- Build System: CMake
- Testing: Google Test (GTest)

Do not rename any files or the folders `src/` or `gtest/`.

## Directory Structure

```bash
├── CMakeLists.txt
├── CMakePresets.json
├── gtest
│   ├── array_gtests.cpp
│   ├── int_array_gtests.cpp
│   ├── gtestmain.cpp
│   └── matrix_gtests.cpp
└── src
    ├── alloc.cpp
    ├── alloc.hpp
    ├── array.hpp
    ├── int_array.hpp
    ├── matrix.hpp
    └── standard_main.cpp
```

## Build Instructions

```bash
# Produce the `build` folder with the presets provided for the homework:
cmake --preset default

# Build all targets at once:
cmake --build build

# Build only standard_main.cpp:
cmake --build build --target standard_main

# Build int_array gtests:
cmake --build build --target int_array_gtests

# Build array gtests:
cmake --build build --target array_gtests

# Build matrix gtests:
cmake --build build --target matrix_gtests
```

To run the above targets after compiling them:

```bash
./build/standard_main       # Runs the 'main' function from src/standard_main.cpp
./build/int_array_gtests    # Runs the 'int_array' gtests
./build/array_gtests        # Runs the 'array' gtests
./build/matrix_gtests       # Runs the 'matrix' gtests
```
## Valgrind
To use `valgrind` to check your code, use these instructions:

```bash
# Produce the `build_valgrind` folder with the presets provided for the homework:
cmake --preset valgrind

# Build all targets at once:
cmake --build build_valgrind

# Build only standard_main.cpp:
cmake --build build_valgrind --target standard_main

# Build array gtests:
cmake --build build_valgrind --target array_gtests

# Build matrix gtests:
cmake --build build_valgrind --target matrix_gtests
```

Then run the above targets with:

```bash
./build_valgrind/standard_main       # Runs the 'main' function from src/standard_main.cpp
./build_valgrind/array_gtests        # Runs the 'array' gtests
./build_valgrind/matrix_gtests       # Runs the 'matrix' gtests
```

## Submission

As with previous submissions, submit via `GitHub` via the following steps:

1. `git add .`
2. `git commit -a -m "Commit Message Here"` (incrementally `commit` each piece you get working)
3. `git push --set-upstream origin main` (when you are ready to submit to GradeScope, or need help from staff)

When you are ready, submit your `hw-7` repository to GradeScope.
