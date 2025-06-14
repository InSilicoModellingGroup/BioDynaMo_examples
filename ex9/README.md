# ex9

This repository contains a simple BioDynaMo simulation example.

Whenever you interact with this repository, make sure you have sourced BioDynaMo
correctly. If it's sourced, you'll see a `[bdm-1.X.YY]` in your terminal
(be aware that some terminals might not show it explicitly).
Anytime you open a new terminal window, you have to source it again.
```bash
. <path_to_biodynamo>/build/bin/thisbdm.sh
```

You can verify that BioDynaMo has been sourced correctly by running
`biodynamo demo`. In the following, we want to explain how to build, run, and
test your simulation.

## 1. Building the simulation and the tests

Option 1:
```bash
biodynamo build
```

Option 2:
```bash
mkdir build && cd build
cmake ..
make -j <number_of_processes_for_build>
```

## 2. Running the simulation

Before running the simulation, you need to build it. If you haven't done so,
please go back to step 1.

Option 1:
```bash
biodynamo run
```

Option 2:
```
./build/ex9
```
