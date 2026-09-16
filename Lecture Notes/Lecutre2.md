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
- takes **three** expressions as operands
```C
exp1 ? exp2 : exp3
```
- exp1 is evaluated first
  - if exp1 is TRUE, exp2 is evaluated and its value is used in the ternary operation
  - if exp1 is FALSE, exp3 is evaluated and its value is used instead
```C
#include <stdio.h>

int main(void){
    int i = 5;
    int j = 1;
    int min + i < j ? i : j;
    printf("The value of min is: %d\n", min); // prints 1
    return 0;
}
```
### Multi-way branching using "else-if"
```C
// assume all variables are defined as int
...
if(i == 0){
    n0++;
} elif(i == 1){
    n1++;
} elif(i == 2){
    n2++;
} else{
    n_other++;
}
```
## While Loop
```C
int i = 0;
int sum = 0;

while(i<100){ 
    sum = sum + 1;
    i++
}

// equivalent to
while(i < 100) sum += i++;
```
## Do-While Loop
- checks condition **after** executing the loop body, so the statement is executed *at least once*
```C
int i = 0;
int sum = 0;

do{
    sum = sum + i;
    i++;
} while(i < 100);
```
## For Loop
- also called a counting loop, and has three expressions
- `initialization, condition, and increment
```C
exp1;
while(exp2){
    <stmt>
    exp3;
}
```
### Computing the Sum of [0, 99] Using For-loop
```C
int i, sum;
// first method
sum = 0;
for(i = 0; i < 100; i++) sum = sum + i;

// second method with initializations inside
for(sum = i = 0; i < 100; i++) sum += i;

// third method with comma operator
for(sum = 0, i = 0; i < 100; sum += i, i++)
```
### Compact For-loop
```C
#include <stdio.h>

int main(void){
    for(int i = 0; i < 5; i++){ // all vars for for-loop declared in the loop
        printf("i+%d\n", i);
    }
    return 0;
}
```
## Comma Operator
- takes two expressions
```C
exp1, exp2
```
- exp1 evaluated first, and then exp2 with exp2 being the **result of whole operation**
- has the lowest precedence out of all operations
- associated from left to right, with *order being important*
## Switch Statements
- also called selection statements
```C
swtich(<integer expression>) {
    case <int_const1>:
        <statements>
    case <int_const>:
    case <int_const>:
        <statements>
    default:
        <statements>
}
```
### Example of Switch Statements
```C
// assume all vars defined as integers
switch(i){
    case 0:
        n0++;
        break
    case 1: // no break for case 1, so contoinues
    case 2: 
    {
        // can put a block and define new variables
        int a = d + 10;
        n1 = a*10;
        break;
    }
}
```
### When to Use Switch Statements
- when using if else statements becomes highly inefficient and hard to read
## Break Statement
- for switch statements to prevent fallthrough into another case
- works in loops
  - stops execution immediatelt, control resumes at statement following the loop
## Continue Statement
- skips the rest of the **current loop iteration** and continues into the next one
- used within for, while, and do-while loops
  - appears in nested if/else loops as well, and applies to **innermost enclosing loop**
  - for loops goes to the evaluation of the increment expression