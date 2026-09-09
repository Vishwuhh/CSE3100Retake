# Lecture 3: Flow of Control
- control flow and blocks
- if statements
- while/for statements
- switch statements + common mistakes

## Flow of Control
- statements executed sequentially, but we can have selective or repeated executions as well
  - located in blocks
    - statements enclosed by {}
> *What is the purpose of block statements?*
- use local variables that cannot be accessed outside of the block
```C
{
    // can declare variables as you use them
    int a;
    a = 2;
    int b;
    b = a*10; 
}
```
- blocks can be
  - empty
  - nested
  - used for branching or loop statements
  - used for defining variables at the beginning of blocks 
    - mix declarations and code together in c99
## Comparison and Logical Operators
- operators are used inside of if/else statements
- comparison operators compare two expressions (note the types of them)
```C
== != > < >= <=
```
- logical operators
```C
&& || !
```
- result is either 0 or 1 (in int type)
  - 0 = false
  - 1 = true
## Branching: if and if-else
```C
if(<expression>){
    <statement> // exp is zero
}

if(<expression>){
    <statement1> // exp is nonzero
} else{
    <statement2> // exp is zero
}
```
- if/else has at least two possible branches of behavior
### Example: Min
```C
int i, j, min;

if(i < j){
    min = i;
} else{
    min = j;
}
```
## Branching: if and if-else
- "exp" is usually a comparison or logical expression, but can be ANY expression in theory
- statements can be compound (blocks) or in other if statements
  - **beware of dangling else statements**
### Example: if-else with blocks
```C
int i, j, k;

if(i<j){
    k = i;
    printf("i is selected.\n");
} else{
    k = j;
    printf("j is selected.\n");
}
```
## Bad Coding: Dangling Else
```C
if (a) if (b) s1++; else s2++;
```
- assuming a is true and b is false, should we do s1++ or s2++?
```C
#include <stdio.h>

int main(void){
    // starting vars
    int s1 = 0;
    int s2 = 0;
    int a = 1;
    int b = 0;
    if (a) if (b) s1++; else s2++;
    printf("s1=%d\n", s1) // is zero
    printf("s2=%d\n", s2) // is one 
}
```
- **write more understandable code**
```C
#include <stdio.h>

int main(void){
    // starting vars
    int s1 = 0;
    int s2 = 0;
    int a = 1;
    int b = 0;
if(a){
    if(b){
        s1++;
    } else{
        s2++;
    }
}
printf("============\n");
printf("s1=%d\n", s1);
printf("s2=%d\n", s2);
```
## Ternary Operator
- takes **thre** expressions as operands
```C
exp1 ? exp2 : exp3
```
- exp1 is evaluated first
  - if exp1 is TRUE, exp2 is evaluated and its value is used in the ternary operation
  - if exp1 is FALSE, exp3 is evaluaated and its value is used instead