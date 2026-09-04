# Lecture 2: Expressions and Basic Data Types in C
- lecture overview
  - static and dynamic typing
  - expressions and operators
  - memory for data types and characters

## *What does an expression look like in Python and C?*
```Python
x = 5
s = "hello"
d = False
```
- Python
- **cannot** tell easily if each var holds a different value type
```C
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    int x = 5;
    char s[] = "hello";
    bool d = False;
}
```
- C
- **can** tell easily if each var holds a different data type (int, char, bool)

## Typing for Expressions
- C and other languages like it are *statically typed*
  - type checking done at compile-time
- Python and other languages are *dynamically typed*
  - type checking done at **run-time**
### ***What is the Advantage of Statically Typed Languages?***
- **you can find errors quicker**
```Python
import time
def MethodThatTakesLongTime():
    time.sleep(1)

x = [7, 8, 9]
index = 1.1 
print("Index value=", index)
#imagine there is some code that takes an hour to run
MethodThatTakesLongTime()
print("The value at index=", x[index])
```
- what happens if we do this in C?
```C
#include <stdio.g>
#include <stdbool.h>

int main(void) {
    int x[] = {1, 2, 3};
    double index = 1.1;
    printf("%d\n", x[index]);
}
```
- error checking is done by the compiler before the code ever runs for statically typed languages
  - ISSUE: less efficient to code

## Expressions in C
- **all** C expressions have a type
- adding a semicolon to an expression makes it a **statement**
### Basic Data Types
- int = integer
- char = single byte that stores a character in ASCII, and is an 8-bit integer
- float = floating point numbers
- difference between bit vs. byte?
  - 1 byte = 8 bits
  - bit = 1 or 0
### Constants of Basic Types 
```C
// constants **cannot** be changed
// char
'a', 'b', '/n'

// integer (the compiler stores them in binary)
200, -34
0x7fffFFFF // hexadecimal
07112 // octal

// floating point numbers (fractions)
3.1415, -0.34, 1.3E20
```
### Variables
- **all vars need to be declared and initalized BEFORE use**
- var declarations must specify type and name
  - compiler allocates memory based on the type given
  - valid names are masde of case-sensitive letters, digits, and underscores
    - CANNOT start with a digit though
  - variables can be initialized when declared or using seperate assignments
```C
char c;
int i, j, k = 1;
float f;
```
### Operators
- conventional arithmetic, bitwise, and logical operators
```C
+ - * / % // conventional arithmmetic
& | ~ ^ << >> // bitwise
&& || ! // logical
```
- pre and post increment/decrement (like in Java, C++, and other languages)
```C
i++ ++i j-- --j
c = i++ // c will take the value (i-1)
c = ++i // c will take the SAME value as i
```
#### ++ and -- Operators
```C
int i = 4;
int c = i++; // c = 4, but i = 5 (4+1) now
```
- sets var 'i' to 4
- sets var 'c' to the same value of i, which is 4
- adds 1 to i, which increments its value to 5
```C
int i = 4;
int c = ++i; // increments i and then sets c to i, so both are 5
```
- sets i to 4
- increments i *first*, then sets c to the value of the incremented i
- **both c and i are equal to 5**
### Precedence and Associativity
- determines which operation is done first
  - if operators have *same* preference, use associativity and parantheses
#### Assignment Operators
> LHS = Expression
- LHS is something written to something else (ex. var)'
  - both have compatible typs
  - value of expression is asigned to the LHS and becomes the value of the assignment operation
- compound assignment operators
```C
var op= expr 
var = var op expr // same as above
```
- NOTE: assignments ***ARE NOT*** statements, but are expressions with the equals sign being an **operator**
  - means that assignments can be chained and be used inside larger expressions
## Memory for Data Types and Characters
## Integer Data Types
```C
char
short int <-> short
int
long int <-> long
long long int <-> long long
// also includes unsigned versions of these
```
- their sizes depends on the CPU architecture and compiler 
### x86_64 sizes
