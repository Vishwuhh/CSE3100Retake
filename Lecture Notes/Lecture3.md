# Lecture 4: Functions and Global Variables
## The Importance of Functions
- readable code affects how easily development and proliferation of your code snippets happen
## Functions: Building Blocks of Larger Programs
- programming requires problem decomposition into manageable parts
- C programs are made of functions staring from main()
- functions are black boxes
  - specifies what computations will do
  - abstracts how the computational process works
  - takes inputs and produces outputs
## Writing Functions in C
```C
int AddInt(int x, int y) {
    int solution = x + y;
    return solution;
}
```
- int shows the variable type being returned, AddInt is name of function
- defines variables used in function inside parantheses
- can define variables inside of function if it leads to desired output
- return indicates which variable is passed out of the function when it ends 
  - can use **void** if the function isn't meant to return anything
```C
void AddInt(int x, int y) {
    int solution = x + y;
    printf("%d\n", solution)
}
```
- void can also be used to not pass anything into the function
## Function Prototypes
- can be defined in any order, so you can declare a function before first use
- function prototypes are often placed in header files and then reused
### Purpose of Function Prototypes
- tells compiler the number and type of arguments to be passed into the function and the type of value returned by the function
  - allow it to check the code more thoroughly 
## Types of Variables in C
- local variables
- static local variables
- global variables
- static global variables
### Local Variables
- only visible **inside function**, and value is **not retained** across function calls
### Static Local Variables
- not visible outside of the function, but **does retain** value across function calls
### Global Variables
- declared outside functions, and value is retained for the entire duration of program
### Global Static Variables
- declared outside of functions, and is retained throughout the program
  - visible only in functions defined in the same file following the variable declaration
### Be Careful about Static and Global Variables 
- "nice" functions *only depend on inputs*, but static and global variables can have side-effects
  - cannot understand the function without knowing **all** the code
## Function Call Context
- includes
  - copies of function arguments
  - local variables
  - return address
  - etc. 
- call contexts are managed using the **execution stack**
  - stack frame created automatically for each call and lasts for the duration of call
  - discarded when the function terminates
  - ***nothing in frame survives the call***
## Memory Organization
- every process has an address space
- execuatble code is on the bottom
- statics and globals are right above executables
- stack grows from the top down to the bottom
- heap grows from the bottom to the top
- the area in between is up for grabs by **both stack and heap**