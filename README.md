# SIMx1
An event-driven digital SIMx1 simulator in C++. Circuits are built from primitive gate objects (AND/OR/XOR/NOT), wired together, and evaluated and it is the same model used inside RTL simulators.
Demonstrated with a 4-bit ripple-carry adder built entirely from gate primitives (no `+` operator involved).
Future additions for gui and mux-demux to be added.

## Build

```
mkdir build && cd build
cmake ..
make
```

## Run

```
./SIMx1_sim      # runs one example addition
./testbench      # exhaustively verifies all 256 input combinations
```

## Structure

- `include/wire.hpp`, `src/wire.cpp` signal-carrying wire (0/1)
- `include/gate.hpp`, `src/gate.cpp` Gate base class + AND/OR/XOR/NOT
- `include/circuit.hpp`, `src/circuit.cpp` netlist container, evaluates gates in insertion order
- `src/main.cpp` builds a 4-bit ripple-carry adder from full adders
- `tests/testbench.cpp` self-checking testbench, 100% input coverage
