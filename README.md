# BioDynaMo_examples

This repository serves as a simple introduction to new users of the BioDynaMo platform. The examples have been structured in a way to increase complexity and detail while also introducing the user to new concepts and approaches in agent-based modelling (ABM) via BioDynaMo.

## Examples outline

1) The first example (*ex01*) has the purpose to demonstrate how to establish the appropriate parameters for the simulation, create and add to the simulation engine a single agent (a biological cell) and then run our ABM simulation for a few time-steps, without in fact to anticipate from the simulation for anything really remarkable.
2) In the second example (*ex02*) it is demonstrated how to setup a uniformly distributed cloud of agents and add them to the simulation engine.
3) For the third example (*ex03*), a concept is introduced, the (biological) behavior of an agent (a biological cell), where the platform-established behavior of growth and division is utilized. In this simulation a cell grows and then every a few steps it divides, thus, leading to the creation of a new cell. Then that original cell continues to grow and divide and so on and so for, until a cloud of randomly positioned cells are created in the end.
4) In the fourth example (*ex04*), a user-defined (biological) behavior of an agent (a biological cell) is introduced that, opposed to the previous example, allows now for the inheritance of cell growth and division from the *mother* to the *daughter* cell, which in the end leads towards a much bigger cloud of randomly positioned cells in the end.
5) ...write a few things about *ex05*...
6) ...write a few things about *ex06*...
7) ...write a few things about *ex07*...
8) ...write a few things about *ex08*...
9) ...write a few things about *ex09*...
10) ...write a few things about *ex10*...
11) ...write a few things about *ex11*...


## BioDynaMo installation

To install the platform, one can execute for the following commands in order. Simply adapt the directories to your setting and the location of the C and C++ compilers.

```
cd /full/path/where/BioDynaMo/has/been/downloaded/
./prerequisites.sh all
rm -rf build; mkdir build; cd build
cmake -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ -Dparaview=on -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/full/path/where/to/install/BioDynaMo ..
. /full/path/where/BioDynaMo/has/been/downloaded//build/bin/thisbdm.sh
make -j 4 && make install
```

## References

Feel free to visit BioDynaMo's [website](https://www.biodynamo.org/) to learn more about the platform and the demos that come with it.

![Image](https://pbs.twimg.com/media/FD6LEW3X0AYisAa?format=png&name=small)
