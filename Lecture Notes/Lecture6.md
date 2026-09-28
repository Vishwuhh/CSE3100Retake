# Lecture 7: More on Memory, Pointers, and Structures
> In functions, why would we want to use malloc over '[]' approach?
- declaring arrays with square brackets has memory allocated from the stack, **which is a fixed size**
  - could run out of memory in a function call if we use a large [] array
- heap is **not a fixed size** and is also larger in general
- IMPORTANT: unlike malloc, we cannot take non-static stack vars outrside of the function call
> What will the following print out?
```C
#include <stdio.h>
#include <stdlib.h>

//try to return stack var
int* AddThreeArray(int* x) {
    int solution[3];
    for(int i = 0; i < 3; i++) {
        solution[i] = x[i] + 3;
    }
    return solution;
}

int main() {
    //create array of size 3
    int* x = malloc(sizeof(int) * 3);
    x[0] = 1;
    x[1] = 2;
    x[2] = 3;
    //call AddThreeArray
    int* solution = AddThreeArray(x);
    //print solution
    for(int i = 0; i < 3; i++) {
        printf("solution[%d] = %d\n", i, solution[i]);
    }
}
```
- returning stack variables from functions leads to unexpected outputs!!
```C
#include <stdio.h>
#include <stdlib.h>

//try to return stack var
int* AddThreeArray(int* x) {
    int* solution = malloc(sizeof(int)*3); // CHANGED LINE  
    for(int i = 0; i < 3; i++) {
        solution[i] = x[i] + 3;
    }
    return solution;
}

int main() {
    //create array of size 3
    int* x = malloc(sizeof(int) * 3);
    x[0] = 1;
    x[1] = 2;
    x[2] = 3;
    //call AddThreeArray
    int* solution = AddThreeArray(x);
    //print solution
    for(int i = 0; i < 3; i++) {
        printf("solution[%d] = %d\n", i, solution[i]);
    }
}
```
- this code will provide the expected output because malloc takes memory **from the heap, not stack**
  - heap memory can only be freed by the coder
> Does this mean we should *never* use [] arrays that come from the stack?
- **not that simple**
  - currently treating memory as one big block with different sections
  - realit is, stack memory can be accessed **faster** than heap memory
## Pointers are Addresses
- value of a pointer is a **byte address**
  - unsigned integer used to number the bytes in memory
  - [0x00000000 0xFFFFFFFF] range in 32 bit
- if a pointer is an integer, you can do arithemtic to compute other addresses
### Simple Pointer Example
```C
#include <stdio.h>
#include <stdlib.h>

int mian() {
    // create array of size 3
    int* x = malloc(sizeof(int) * 3);
    x[0] = 7;
    x[1] = 8;
    x[2] = 9;
    // create another pointer
    int* p = x + 1;
    printf("Value at pointer p=%d\n", *p)
}
```
- prints "Value at pointer p=8"
#### There may be some confusion...
- x is an int pointer to an address in memory
- p = x + 1
  - if we add 1 we should get 993 as the mem address, but we get 996 (value for 8) instead
## Adding a Pointer to an Integer
- adding an integer to a pointer results in a **pointer of the same type**
  - different from regular integer addition, since it the integer is **automatically scaled** by the size fo the type being pointer to
- assume p is a pointer to type T, and k is an integer
```C
(p + k) == (k + p) // valid expressions that evaluate to a pointer to type T
```
- byte address of above is equivalent to 
> (unsigned long)(address stored in p)+ k * sizeof(T)
### Can we see the addresses?
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
    // create array of size three
    int* x malloc(sizeof(int) * 3);
    x[0] = 777;
    x[1] = 888;
    x[2] = 999;
    // create another pointer
    int* p = x - 1;
    printf("Address x points to %x\n", x)
    printf("Address p points to %x\n", p)
}
```
- can print out the addresses of pointers
## Pointer Addition
- assume p is a pointer to an int, and has value of 1000
- p + 1 is **not** the next byte address, but the address of the **next item of the same type**
## Pointer Subtraction
- subtract one pointer from another, but both **must be of the same type**
  - result is the number of data items between two pointers (NOT NUM BYTES)