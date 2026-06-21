# VLib

A modern C++ learning project focused on implementing core data structures, memory management utilities, and numerical computing components from scratch.

The goal of VLib is not merely to reproduce the functionality of the C++ Standard Library, but to develop a deeper understanding of modern C++ concepts such as resource ownership, RAII, move semantics, templates, memory layout, and performance-oriented design.

## Features

### Smart Pointers

* Reference-counted smart pointer implementation
* Copy and move semantics
* Automatic resource management
* Ownership and lifetime tracking

### Containers

* Dynamic Vector implementation
* Iterator support
* Range-based for loop compatibility
* Bounds-checked access

### Linear Algebra

* Matrix implementation
* Dynamic dimensions
* Arithmetic operations
* Stream output support

### Planned Components

* Unique Pointer
* Shared Pointer
* Tensor
* Thread Pool
* Neural Network Components
* CUDA Backend Experiments
* Numerical Optimization Utilities

## Project Structure

```text
vlib/
├── include/
│   ├── memory/
│   ├── containers/
│   └── math/
├── src/
├── tests/
├── benchmarks/
└── docs/
```

## Concepts Explored

This project serves as a practical exploration of:

* RAII
* Rule of Zero
* Rule of Five
* Copy and Move Semantics
* Templates
* Dynamic Memory Management
* Cache-Friendly Data Structures
* Iterators
* Generic Programming
* High-Performance Computing Techniques

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Motivation

As machine learning and scientific computing systems continue to demand higher performance, understanding how data structures and numerical libraries are implemented becomes increasingly valuable.

VLib is a learning-oriented project aimed at bridging the gap between writing C++ code and understanding how modern numerical and machine learning frameworks are built internally.

## Current Progress

* [x] Smart Pointer
* [x] Vector
* [x] Matrix
* [ ] Unique Pointer
* [ ] Tensor
* [ ] Thread Pool
* [ ] Neural Network
* [ ] CUDA Experiments

## Author

Davinder Singh

PhD Researcher in Quantum Machine Learning

Building systems from first principles, one component at a time.
