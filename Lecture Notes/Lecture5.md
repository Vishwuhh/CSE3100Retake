# Lecture 6: Memory Allocation
- three pools of memory
  - static/global
  - stack 
  - heap
- each pool has different lifetimes, allocation and deallocation priorities
- memory
  - every process has an *address space*
  - executable code is at the bottom
  - statics and globals are above executable code
  - stack is on the other side of executables, and goes down towards executable
  - heap grows above statics and globals and goes towards the stack
- space between stack and heap is up for grabs by either 
## Static/Global Memory Pool
- where
  - all constants + string literals are held
  - global variables
  - all vars declared "static" are held 
- allocated when the program starts
- deallocated when the prorgam terminates
- **has fixed size**, so the compiler needs to know the size and make reservations for it 
## Stack
- where memory comes from for *local* variables in functions
- is automatic, so easier to manage
  - allocated automatically when entering, and deallocated automatically when leaving functions
    - **scope is just the function** and should not be used after function returns value
- default size with gcc is 2MB (megabytes)
- for larger arrays and deep recursion, may need to increase stack size
## Heap
- where memory comes from for "on-the-fly" manual allocations
- the programmer is in charge for allocation and deallocation
- they last for *as long as they are not freed*
## Manipulating Memory in C
> *In C, how do I get my functions to return multiple values (arrays)?*
```C
#include <stdio.h>
#include <stdlib.h>

int* AddThreeToArray(int* x) {
  int* solution = malloc(3*sizeof(int));
  for(int i = 0; i < 3; i++) {
    solution[i] = x[i] + 3;
  }
  return soluton;
}

int main() { // prints solution[0] = 4, solution[1] = 5, and solution[2] = 6 (each on a new line)
  int x[3] = {1, 2, 3};
  // function creates memory for solution internally
  int* solution = AddThreeToArray(x);
  for(int i = 0; i < 3; i++) {
    printf("solution[%d] = %d\n", i, solution[i]);
  }
}
```
## Malloc: Requesting Memory on the Heap
```C
#include <stdlib.h>
void* malloc(size_t size);
```
- size_t is an unsinged integer defined in <stdlib.h>, and represents sizes of objects in bytes
- if successful, call to malloc(n) returns a **generic pointer (void*)**
- if unsuccessful, NULL is returned 
### Generic Pointers: *void
- pointer to a memory block holding *un-typed* content
- for raw memory operations or in generic functions
- has automatic casting when assigned to other pointer types
```C
int * pox = malloc(6*sizeof(int));
```
- requires casting *before dereferencing for read/write*
```C
*(int *)pv; // use pv as an int*
```
- NULL is a special pointer value for initializations and error handling 
### Failed malloc() Calls
- calls to malloc() can fail if you are out of memory, and will return NULL and terminate
## Lingering Issues?
```C
#include <stdio.h>
#include <stdlib.h>

int* AddThreeToArray(int* x) {
  int* solution = malloc(3*sizeof(int)); // hard  coded variable
  for(int i = 0; i < 3; i++) { // hard coded variable
    solution[i] = x[i] + 3;
  }
  return soluton;
}

int main() { // prints solution[0] = 4, solution[1] = 5, and solution[2] = 6 (each on a new line)
  int x[3] = {1, 2, 3};
  // function creates memory for solution internally
  int* solution = AddThreeToArray(x);
  for(int i = 0; i < 3; i++) {
    printf("solution[%d] = %d\n", i, solution[i]);
  }
}
```
- **we still have hard coded variables**
### Problem Breakdown
- working with an array of a **pre-declared size**
- working with an array **created through malloc**
#### Case 1: Counting Elements in Pre-Delared Arrays
- need to know the type of data being stored when passing an array not declared through malloc
  - can divide total size of array by the size of one element of data --> leads to the **number of elements in array**
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
  // declare array
  int a[5];
  // get size of first element in array
  size_t sizeOneElem = sizeof(a[0]);
  // get total size of array
  size_t sizeArray = sizeof(a);
  // use formula
  int numElem = sizeArray / sizeOneElem; 
  // print num elements
  printf("Number of elements: %d", numElem);
}
```
- arrays are passed to functions as pointers, so the function **doesn't see the entire array, but sees a pointer to the first element**
#### Case 2: Working with Malloc Arrays
- malloc creates new memory by giving a pointer to a memory address, so **you need to keep track**
## Creating Arrays and Setting Values: calloc
```C
#include <stdlib.h>

void* calloc(size_t nmemb, size_t size);
```
- implemented in terms of malloc(), and also initializes the content of the array to zero
### Adjusting Array Size
```C
#include <stdlib.h>

void *realloc(void* ptr, size_t size);
```
- can be used to increease/decrease the allocated memory as needed
- before a call for realloc, p must be a pointer returned from a previous malloc/calloc/realloc call or NULL
  - if NULL, call is **equivalent to malloc(size)**
## Deallocation
```C
#include <stdlib.h>

void free(void *ptr)
```
- calls the library function "free", and frees the memory associated with the given pointer
  - **CANNOT FREE A POINTER TWICE**, set to NULL afterwards
## Rules for Memory
- rule 1: 
  - everything you requested should eventually be freed
- rule 2: 
  - only free what is allocated through malloc/calloc/realloc
- consequences of breaking the rules
  - memory leaks
  - undefined behavior and unexpected crashes
## How to make 2D Arrays?
- **POINTERS!**
- using an array of pointers, we can make a 2D array
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
  // specify the num of rows and columns
  int rows = 3;
  int columns = 2;
  // double star to indicate a pointer to a pointer
  int** array2D;
  array2D = malloc(sizeof(int*)*rows) // malloc the size of integer pointers multiplied by num rows
  for(int i = 0; i < rows; i++) {
    array2D[i] = malloc(sizeof(int) * columns); // for every row, make the malloc for the columns associated with it 
  }
  // fill in array
  for(int i = 0; i < rows; i++) {
    for(int j = 0; j < columns; j++) {
      array2D[i][j] = counter; 
      counter++; // increment counter
    }
  }
  // print the values
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < columns; j++) {
      printf("Value at [%d, %d] = %d\n", i, j, array2D[i][j]);e
    }
  }
}
```