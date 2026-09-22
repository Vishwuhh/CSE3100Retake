# Lecture 5: Arrays and Pointer Basics 
> What happens if you want to associate multiple values with a variable?
- **use an array**
```C
int main() {
    // create array size 4
    int x[4];
    // indexing starts at zero
    x[0] = 100;
}
```
- array created is as follows [100, 0, 0, 0]
```C
int main() {
    // create array size 4
    int x[4];
    // indexing starts at zero
    x[0] = 100;
    x[1] = 50;
    x[2] = 120;
    x[3] = 40;
    // can also print the elements of the array
    printf("The 0th value is: %d\n", x[0]); // prints 100 because it is the value at index 0
}
```
## Arrays in C
- linear, contiguous collection of things of the **same fixed type**
  - ex. array of characters, integers, doubles, arrays, etc. 
### Array Initialization
```C
// initialize array with list
int y[5] = {1, 2, 3, 4, 5};
// num elements is option iff all elements are listed
int z[] = {7, 8, 9, 10, 11};
// can specify elements less than what is initialized, rest of the elements are set to zero
int y[5] = {1, 2};
// in C99, b would have 1, 2, 0, 0, 5
int b[5] = {1, 2, [4] = 5};
```
### Arrays as Automatic Variables 
- can declare arrays inside *any function or block*
  - is destroyed when exiting from the function or block
- variable length arrays (VLAs in C99)
  - size of array depends on dunction arguments or other known values 
```C
int foo(int n, int k) {
    int x[n;]

    for(int i = 0; i < n; i++) {
        x[i] = 0;
    }
    ...
    return -1;
}
```
### Array Assignment
- **cannot** assign a whole array at once to another array, even when types match
### Strings are Arrays Too
- strings are *char array* that ends with a zero (null character)
- can be initalized with a list of characters or a string (double quoted literal)
```C
#include <stdio.h>
int main() {
  char s[6] = {'H', 'e', 'l', 'l', 'o', '\0'};
  char t[6] = "Hello";
  char u[] = "Hello";
  printf("Array is: %s\n", s);
}
```
### Arrays and Functions
- arrays can be passed into functions, with an exception
- calling convention in c
  - its by VALUE for all things *but arrays*
- arrays are passed to functions by reference
  - through pointers
- **functions cannot return arrays**
## What is Passing by Value?
```C
#include <stdio.h>
#include <stdlib.h>

int AddInt(int x, int y) {
  int z = x + y;
  x = 5; // sets the value of x 
  return z;
}

int main() {
  int x = 10;
  int y = 7;
  int sum = AddInt(x, y);
  printf("The value of x: %d\n", x); // prints 10
}
```
- in C for *primitive* datatypes, when passed to other functions, they are passed as independent copies 
  - primitive datatypes - int, float, double, long, char, etc. 
- passing by value = value is passed to the function but not the variable itself 
## Passing by Reference 
```C
#include <stdio.h>
#include <stdlib.h>

// retruns sum of last index
int addInt(int x[], int y[]){
  // assume array len of 2
  int sum[2];
  for(int i = 0; i < 2; i++) {
    sum[i] = x[i] + y[i];
  }
  x[0] = 5; // sets value of x 
  return sum[1];
}

int main(){
  // create two arrays
  int x[2] = {1, 2};
  int y[2] = {3, 4};
  // call sum function
  int sum = addInt(x, y);
  printf("The value of x: %d\n", x[0]);
}
```
- arrays are **not primitive**, so passing array to method does not give an independent copy 
  - you get reference to where the *original* array is stored in the memory
## Pointers and Memory in C
- when talking about where you live, people are talking about needed the *address of your house* and the *house itself*
- two things are important in C
  - value of variable
  - where the variable lives in memory
- the reference to the address of a variable in memory is a **pointer**
### Variables and Memory
- memory is an array of bytes
  - each byte is numbers with their address (an unsigned integer)
- every variable is kept in memroy, and is associated with two numbers
  - address and value stored at address
### Referencing and Dereferencing 
- & - referencing (getting the address of something)
- * - dereferencing (using the address of something)
#### Pointer Example with &
```C
#include <stdio.h>
#include <stdlib.h>

int main() {
  // declare some vars
  int x = 10;
  int y = 5;
  // declare a pointer to some place in the memory
  int* px;
  // get address of where x is locared
  px = &x;
  // at address where x is located, put 20 instead
  *px = 20;
  printf("The value of x %d\n", x);
}
```
- say x is stored at address 1020 and y is in address 1016
- the value px is stored in a different address 1012, and the value stored there is *the address of the value of x*
  - the value of px is used to change the value of x by using the stored address
### How do We Use Pointers with Arrays?
- we can use pointers to pass arrays as outputs
  - because a function can only return one variable, we can return a pointer to the **start of the array**
#### Pointer Example: Returning an Array
```C
#include <stdio.h>
#include <stdlib.h>

int* AddThreeToArray(int* x) {
  // assume array size is three
  static int z[3]; // new array for adding three
  for (int i = 0; i < 3; i++) {
    z[i] = x[i] + 3;
  }
  // return pointer of z
  return z;
}

int main() {
  int x[3] = {1, 2, 3};
  int *z = AddThreeToArray(x);
  // print the values of x
  for(int i = 0; i < 3; i++) {
    printf("z[%d]=%d\n", i, z[i]); // would print the values of z NOT x
  }
}
```
> Any problems with this approach?
- what happens if we want the function used multiple times?
  - z would get overwritten since we only have a **static variable**
## Pointer Declarations
```C
int* p;
int * p;
int *p;
```
- all of above are equivalent, but first makes the pointer declaration clear and the last one says what p points to is an integer