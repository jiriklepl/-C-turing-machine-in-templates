# C++ turing machine in templates

This is a simple demonstration of that C++ templates are fully capable of implementing a turing machine (this version is compatible with C++11 and newer versions).

## Folder map

- [machine.hpp](machine.hpp) - the implementation of the machine
- [standard.hpp](standard.hpp) - a standard library mini-implementation (we could use standard library, but that would make the demonstration less interesting)
- [main.cpp](main.cpp) - a simple example of an input accepted by the machine
- [main_reject.cpp](main_reject.cpp) - a simple example of an input rejected by the machine

## How to use

Description of a correct user-defined setup (input): (see [main.cpp](main.cpp) for an example)

- `Q` is the enum of states
- `Gamma` is the alphabet enum, it has to contain a value named `Blank`
- `Delta` is a struct template which takes `Display<Q, Gamma, Q State, Gamma Symbol>` instances:
  - each instance represents a single mapping of the delta function
  - the instances implement following static (constexpr) values:
    - write: the symbol (from Gamma) written on the place under the machine's head
    - shift: the direction (L, N, R) where the head moves after writing
    - to: the resulting state of the turing machine
- `Input` is an instance of `tape<Gamma, Gamma... Places>` and it is the initial state of the work tape
- `Fin` is an instance of `tape<Q, Q... States>` and it is the set of accepting states
- `Q0` is the initial state

(the actual names don't have to follow this description, with an exception of the `Blank` value)

## How to use the turing machine

```C++
run_turing_t<Q, Q0, Input, Fin, Delta>
```

- it returns a type: int if the machine accepts the input; if the machine rejects the input, the type is void and a compile-error is created
  - we can use this type in the definition of main() to create a valid c++ program

    ```C++
    run_turing_t<Q, Q0, Input, Fin, Delta> main() {}
    ```

see [https://en.wikipedia.org/wiki/Turing_machine](https://en.wikipedia.org/wiki/Turing_machine) for a pretty nice formal definition of turing machine

## How to run

Use your favorite c++ compiler to compile the input file (here compiling main.cpp, change it to main_reject.cpp to see an example of an input rejected by the machine; `-o /dev/null` added as we don't want the executable):

```bash
g++ main.cpp -o /dev/null
clang++ main.cpp -o /dev/null
nvcc main.cpp -o /dev/null
```
